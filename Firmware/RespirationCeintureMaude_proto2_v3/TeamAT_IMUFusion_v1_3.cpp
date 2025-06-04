
#include "TeamAT_IMUFusion_v1_3.h"
//#include "Arduino.h"
//#include "HardwareSerial.h"
//#include <QDebug>

float TeamAT_IMUFusion_getElevationAccelero_atan(float accx,float accy,float accz)
{

    float IMUFusion_Elevationvalue = atan2(sqrt(accx*accx + accz*accz),accy)*180/3.14159;

    return IMUFusion_Elevationvalue;
}


void elevationAlgoData_init(elevationAlgoData_t *elevationAlgoData, float Ts)
{
    elevationAlgoData->uquat_last = (1-Ts*4);
    elevationAlgoData->filtAccLowpass_last = 0.01;
	
	for (int i=0; i < 6; i++)
	{
		elevationAlgoData->accelgyrData_limited_last[i] = 0;
		elevationAlgoData->accelgyrData_limited_filt[i] = 0;
	}
	for (int i=0; i < 5; i++)
	{
		elevationAlgoData->accelgyrData_limited_gyrx_filt_Y[i] = 0;
		elevationAlgoData->accelgyrData_limited_gyry_filt_Y[i] = 0;
		elevationAlgoData->accelgyrData_limited_gyrz_filt_Y[i] = 0;
		elevationAlgoData->accelgyrData_limited_gyrx_filt_Z[i] = 0;
		elevationAlgoData->accelgyrData_limited_gyry_filt_Z[i] = 0;
		elevationAlgoData->accelgyrData_limited_gyrz_filt_Z[i] = 0;
	}
	
	elevationAlgoData->accelgyrData_limited_gyrx_filt_u0 = 0;
    elevationAlgoData->accelgyrData_limited_gyry_filt_u0 = 0;
    elevationAlgoData->accelgyrData_limited_gyrz_filt_u0 = 0;
	
	for (int i=0; i < 9; i++)
	{
		if (i == 0 || i == 4 || i==8)
		{
			elevationAlgoData->matRotZAccum_c[i] = 1;
			elevationAlgoData->matRotmFus_xy[i] = 1;
			elevationAlgoData->matRotFinal[i] = 1;
			elevationAlgoData->matRotZero[i] = 1;
			elevationAlgoData->matRotAbsolute[i] = 1;
		}
		else
		{
			elevationAlgoData->matRotZAccum_c[i] = 0;
			elevationAlgoData->matRotmFus_xy[i] = 0;
			elevationAlgoData->matRotFinal[i] = 0;
			elevationAlgoData->matRotZero[i] = 0;
			elevationAlgoData->matRotAbsolute[i] = 0;
		}
	}

	
	
}


elevationResult_t TeamAT_IMUFusion_getElevationAccelGyro(elevationAlgoData_t *elevationAlgoData, float accelgyrData[6], float Ts, int flagfirst, int resetElevation)
{
	
	// PARAMETRES
        float matRotShift = 0*PI/180;//tr
        float matRotDefAxeMat[9] = {cos(matRotShift), sin(matRotShift), 0, -sin(matRotShift), cos(matRotShift), 0, 0, 0, 1};
		
        float filtAccLowpass = Ts;//%juste pour initialiser, n'a pas d'effet   //%freq = 1/filtAccLowpass/2/pi. 0.01=15.91Hz    
        float filtAccLowpass_min = Ts;  
        float filtAccLowpass_max = 0.05;
        float filtAccLowpass_rate = 0.05;
        float filtAcc_rateLimiter = 60*Ts;//%40
        float filtGyr_rateLimiter = 30*Ts;//%20

        float accelNorme_gravFree_min = 1;
        float accelNorme_gravFree_max = 5;
        float uquat=(1-Ts*4);//%juste pour initialiser, n'a pas d'effet
        float uquat_min=(1-Ts*4);//%0.992à500Hz;    gyro*(1-Ts*4)+acc*Ts*4
        float uquat_max=(1-Ts*1.5);//%0.997à500Hz;  gyro*(1-Ts*1.5)+acc*Ts*1.5
        float uquat_rate = 0.005;

        double filtGyr_b[5];
        double filtGyr_a[5];
		
        if (Ts <= 0.011) // 100 Hz (10ms). low: 30  High: 0.001
        {
            filtGyr_b[0] = 0.391314695909236; filtGyr_b[1] = 0; filtGyr_b[2] = -0.782629391818472; filtGyr_b[3] = 0 ; filtGyr_b[4] = 0.391314695909236;
            filtGyr_a[0] = 1.000000000000000; filtGyr_a[1] = -1.630406109156383; filtGyr_a[2] = 0.456757210330416; filtGyr_a[3] = -0.022156992426718; filtGyr_a[4] = 0.195805897432362;
        }
        else if (Ts > 0.011 && Ts <= 0.014) //83Hz (12ms)
        {
            filtGyr_b[0] = 0.529936766134569; filtGyr_b[1] = 0; filtGyr_b[2] = -1.059873532269139; filtGyr_b[3] = 0 ; filtGyr_b[4] = 0.529936766134569;
            filtGyr_a[0] = 1.000000000000000; filtGyr_a[1] = -1.174672752788131; filtGyr_a[2] = -0.355820401953671; filtGyr_a[3] = 0.235885117297109; filtGyr_a[4] = 0.294608049495654;
        }
        else if (Ts > 0.014) //66Hz (15ms)
        {
            filtGyr_b[0] = 0.800538811070415; filtGyr_b[1] = 0; filtGyr_b[2] = -1.601077622140830; filtGyr_b[3] = 0 ; filtGyr_b[4] = 0.800538811070415;
            filtGyr_a[0] = 1.000000000000000; filtGyr_a[1] = -0.438849705213504; filtGyr_a[2] = -1.480607586084857; filtGyr_a[3] = 0.278191151911159; filtGyr_a[4] = 0.641266167831231;
        }
        else //défaut de 100Hz
        {
            filtGyr_b[0] = 0.391314695909236; filtGyr_b[1] = 0; filtGyr_b[2] = -0.782629391818472; filtGyr_b[3] = 0 ; filtGyr_b[4] = 0.391314695909236;
            filtGyr_a[0] = 1.000000000000000; filtGyr_a[1] = -1.630406109156383; filtGyr_a[2] = 0.456757210330416; filtGyr_a[3] = -0.022156992426718; filtGyr_a[4] = 0.195805897432362;
        }


		
	// RATE LIMITER
        float accelgyrData_limited[6];
		for (int k=0; k<6; k++)
		{
			accelgyrData_limited[k] = accelgyrData[k];
		}
    if (flagfirst == 1)
		{
			for (int k=0; k<3; k++)
			{
				if ((accelgyrData_limited[k] - elevationAlgoData->accelgyrData_limited_last[k] > filtAcc_rateLimiter))
				{
					accelgyrData_limited[k] = elevationAlgoData->accelgyrData_limited_last[k] + filtAcc_rateLimiter;
				}
				else if ((accelgyrData_limited[k] - elevationAlgoData->accelgyrData_limited_last[k] < -filtAcc_rateLimiter))
				{
					accelgyrData_limited[k] = elevationAlgoData->accelgyrData_limited_last[k] - filtAcc_rateLimiter;
				}
			}
			for (int k=3; k<6; k++)
			{
				if ((accelgyrData_limited[k] - elevationAlgoData->accelgyrData_limited_last[k] > filtGyr_rateLimiter))
				{
					accelgyrData_limited[k] = elevationAlgoData->accelgyrData_limited_last[k] + filtGyr_rateLimiter;
				}
				else if ((accelgyrData_limited[k] - elevationAlgoData->accelgyrData_limited_last[k] < -filtGyr_rateLimiter))
				{
					accelgyrData_limited[k] = elevationAlgoData->accelgyrData_limited_last[k] - filtGyr_rateLimiter;
				}
			}
		}
		for (int k=0; k<6; k++)
		{
			elevationAlgoData->accelgyrData_limited_last[k] = accelgyrData_limited[k];
		}

		
	// ALGO TRAITEMENT JUMPJUMP
        float accelNorme = sqrt(accelgyrData_limited[0]*accelgyrData_limited[0] + accelgyrData_limited[1]*accelgyrData_limited[1] + accelgyrData_limited[2]*accelgyrData_limited[2]);
        float accelNorme_gravFree = fabs(accelNorme-9.81);
		
		 if (accelNorme_gravFree > accelNorme_gravFree_max)
		 {
			 uquat = uquat_max;
			 filtAccLowpass = filtAccLowpass_max;
		 }
		 else if (accelNorme_gravFree < accelNorme_gravFree_min)
		 {
			 uquat = uquat_min;
			 filtAccLowpass = filtAccLowpass_min;
		 }
		 else
		 {
			 uquat = uquat_min + (uquat_max-uquat_min)*(accelNorme_gravFree-accelNorme_gravFree_min)/(accelNorme_gravFree_max-accelNorme_gravFree_min);
			 filtAccLowpass = filtAccLowpass_min + (filtAccLowpass_max-filtAccLowpass_min)*(accelNorme_gravFree-accelNorme_gravFree_min)/(accelNorme_gravFree_max-accelNorme_gravFree_min);
		 }

		 if (uquat - elevationAlgoData->uquat_last < -uquat_rate*Ts)
		 {
			 uquat = elevationAlgoData->uquat_last - uquat_rate*Ts;
		 }
		 elevationAlgoData->uquat_last = uquat;

		 if (filtAccLowpass - elevationAlgoData->filtAccLowpass_last < -filtAccLowpass_rate*Ts)
		 {
			filtAccLowpass = elevationAlgoData->filtAccLowpass_last - filtAccLowpass_rate*Ts;
		 }
		 elevationAlgoData->filtAccLowpass_last = filtAccLowpass;

		 	
	// FILTRES ACCEL-GYRO 
		for (int k=0; k<3; k++)
		{
            TeamAT_Filters_LowPass_Classicfirst_double((double)accelgyrData_limited[k], elevationAlgoData->accelgyrData_limited_filt[k], flagfirst, (double)filtAccLowpass, Ts);
		}
        elevationAlgoData->accelgyrData_limited_filt[3] = TeamAT_Filters_ab_vsimple_o4_double(filtGyr_b,filtGyr_a,(double)accelgyrData_limited[3],elevationAlgoData->accelgyrData_limited_gyrx_filt_Y,elevationAlgoData->accelgyrData_limited_gyrx_filt_Z,elevationAlgoData->accelgyrData_limited_gyrx_filt_u0,flagfirst,2);
        elevationAlgoData->accelgyrData_limited_filt[4] = TeamAT_Filters_ab_vsimple_o4_double(filtGyr_b,filtGyr_a,(double)accelgyrData_limited[4],elevationAlgoData->accelgyrData_limited_gyry_filt_Y,elevationAlgoData->accelgyrData_limited_gyry_filt_Z,elevationAlgoData->accelgyrData_limited_gyry_filt_u0,flagfirst,2);
        elevationAlgoData->accelgyrData_limited_filt[5] = TeamAT_Filters_ab_vsimple_o4_double(filtGyr_b,filtGyr_a,(double)accelgyrData_limited[5],elevationAlgoData->accelgyrData_limited_gyrz_filt_Y,elevationAlgoData->accelgyrData_limited_gyrz_filt_Z,elevationAlgoData->accelgyrData_limited_gyrz_filt_u0,flagfirst,2);

	
		
	// FUSION	
        float matRot_c_a_xy[9];
        float accelgyrData_limited_filt_acc[3];
        accelgyrData_limited_filt_acc[0] = (float)elevationAlgoData->accelgyrData_limited_filt[0];
        accelgyrData_limited_filt_acc[1] = (float)elevationAlgoData->accelgyrData_limited_filt[1];
        accelgyrData_limited_filt_acc[2] = (float)elevationAlgoData->accelgyrData_limited_filt[2];
		
		// Initialisation
        float matRot_fusion1[9];
        if (flagfirst == 0)
		    {
    			acctorotmatrix(matRot_fusion1, accelgyrData_limited_filt_acc);
    			for (int k=0;k<9;k++)
    			{
    				elevationAlgoData->matRotmFus_xy[k] = matRot_fusion1[k];
    			}
    			elevationAlgoData->matRotZAccum_c[0] = 1;elevationAlgoData->matRotZAccum_c[1] = 0;elevationAlgoData->matRotZAccum_c[2] = 0;elevationAlgoData->matRotZAccum_c[3] = 0;elevationAlgoData->matRotZAccum_c[4] = 1;elevationAlgoData->matRotZAccum_c[5] = 0;elevationAlgoData->matRotZAccum_c[6] = 0;elevationAlgoData->matRotZAccum_c[7] = 0;elevationAlgoData->matRotZAccum_c[8] = 1;
		  }
		else 
		{
			// else ca garde ceux de lautre pas de temps
		}
		
		
		// Combinaison accel-gyro
			// Accelero
				acctorotmatrix(matRot_c_a_xy, accelgyrData_limited_filt_acc);
				//On re-aligne la rotation Z sur l'ancien repere obtenu
                    float matRot_c_a_xy_transp[9];
                    TransposeMatrix(matRot_c_a_xy, matRot_c_a_xy_transp, 3, 3);
                    float Qtemp[9];
                    MatMultiply(elevationAlgoData->matRotmFus_xy, matRot_c_a_xy_transp, Qtemp, 3,3,3);

                    float phitemp=0;
                    if (!isnan(Qtemp[3]) && !isinf(Qtemp[3]) && !isnan(Qtemp[0]) && !isinf(Qtemp[0]))
                    {
						phitemp = atan2(Qtemp[3],Qtemp[0]);
					}
					else
					{
						phitemp = 0;
					}
                    float Rtemp2[9];
					Rtemp2[0] = cos(phitemp); Rtemp2[1] = -sin(phitemp); Rtemp2[2] = 0;
					Rtemp2[3] = sin(phitemp); Rtemp2[4] = cos(phitemp); Rtemp2[5] = 0;
					Rtemp2[6] = 0; Rtemp2[7] = 0; Rtemp2[8] = 1;
                    float Rtemp2_transp[9];
                    TransposeMatrix(Rtemp2, Rtemp2_transp, 3, 3);
                    float Qtemp2[9];
					MatMultiply_3x3(Rtemp2_transp, Qtemp, Qtemp2);
                    float matRot_c_a_xy2[9];
					MatMultiply_3x3(Rtemp2, matRot_c_a_xy, matRot_c_a_xy2);

		
					
			// Gyroscope
                float accelgyrData_limited_filt_gyro[3];
                accelgyrData_limited_filt_gyro[0] = (float)elevationAlgoData->accelgyrData_limited_filt[3]*Ts;
                accelgyrData_limited_filt_gyro[1] = (float)elevationAlgoData->accelgyrData_limited_filt[4]*Ts;
                accelgyrData_limited_filt_gyro[2] = (float)elevationAlgoData->accelgyrData_limited_filt[5]*Ts;
                float accelgyrData_limited_filt_gyro2[3];
				MatMultiply(elevationAlgoData->matRotmFus_xy, accelgyrData_limited_filt_gyro, accelgyrData_limited_filt_gyro2, 3,1,3);
                float rpsi = accelgyrData_limited_filt_gyro2[0];
                float rtheta = accelgyrData_limited_filt_gyro2[1];
                float rphi = accelgyrData_limited_filt_gyro2[2];
                float Rz[9] = {cos(rphi), -sin(rphi), 0, sin(rphi), cos(rphi), 0, 0, 0, 1};
                float Ry[9] = {cos(rtheta), 0, sin(rtheta), 0, 1, 0, -sin(rtheta), 0, cos(rtheta)};
                float Rx[9] = {1, 0, 0, 0, cos(rpsi), -sin(rpsi), 0, sin(rpsi), cos(rpsi)};
                float rotm_c_g_xy_temp1[9];
				MatMultiply_3x3(Ry, Rx, rotm_c_g_xy_temp1);
                float rotm_c_g_xy[9];
				MatMultiply_3x3(rotm_c_g_xy_temp1, elevationAlgoData->matRotmFus_xy, rotm_c_g_xy);


	
		// Fusion
			float quat_acc[4];float quat_acc2[4];
			float quat_gyr[4];float quat_gyr2[4];
					MatRotationToQuat(quat_acc,matRot_c_a_xy2);
					MatRotationToQuat(quat_gyr,rotm_c_g_xy);
					
			Normalize4(quat_acc, quat_acc2);
			Normalize4(quat_gyr, quat_gyr2);

			float quat_pos = DotProduct(quat_acc2, quat_gyr2, 4);
			float quat_neg = -1*quat_pos;
					
			if (acosf(quat_neg) < acosf(quat_pos))
			{
				quat_gyr2[0] = -quat_gyr2[0];
				quat_gyr2[1] = -quat_gyr2[1];
				quat_gyr2[2] = -quat_gyr2[2];
				quat_gyr2[3] = -quat_gyr2[3];
			}


			float quatmultquat = DotProduct(quat_acc2, quat_gyr2, 4);
			if (quatmultquat > 1)
			{
				quatmultquat = 1;
			}
			else if (quatmultquat < -1)
			{
				quatmultquat = -1;
			}
			float theta_quat = acosf(quatmultquat);

			float theta_fus[4];
			if (fabs(theta_quat) < 0.000001 ||(fabs(theta_quat-PI) < 0.000001) || (fabs(theta_quat+PI) < 0.000001))
			{
				  theta_fus[0] = quat_acc2[0];
				  theta_fus[1] = quat_acc2[1];
				  theta_fus[2] = quat_acc2[2];
				  theta_fus[3] = quat_acc2[3];
			}
			else
			{
				float sinthetaQuatinv = 1 / sinf(theta_quat);
				float sin1muQuat = sinf((1-uquat)*theta_quat);
				float sinuQuatthetaQuat = sinf(uquat*theta_quat);

				theta_fus[0] = (sin1muQuat * quat_acc2[0] + sinuQuatthetaQuat * quat_gyr2[0]) * sinthetaQuatinv;
				theta_fus[1] = (sin1muQuat * quat_acc2[1] + sinuQuatthetaQuat * quat_gyr2[1]) * sinthetaQuatinv;
				theta_fus[2] = (sin1muQuat * quat_acc2[2] + sinuQuatthetaQuat * quat_gyr2[2]) * sinthetaQuatinv;
				theta_fus[3] = (sin1muQuat * quat_acc2[3] + sinuQuatthetaQuat * quat_gyr2[3]) * sinthetaQuatinv;
			}
			float theta_fus2[4];
			Normalize4(theta_fus,theta_fus2);
			quatToMatRotation(elevationAlgoData->matRotmFus_xy, theta_fus2);

					
			float RotZaccum_c_temp[9];
			VectCopy(RotZaccum_c_temp,elevationAlgoData->matRotZAccum_c, 9);
			MatMultiply_3x3(RotZaccum_c_temp, Rz, elevationAlgoData->matRotZAccum_c);
			MatMultiply_3x3(elevationAlgoData->matRotZAccum_c, elevationAlgoData->matRotmFus_xy, matRot_fusion1);
					
					
			if (flagfirst == 0)
			{
				VectCopy(elevationAlgoData->matRotZero,matRot_fusion1,9);
			}
			float matRotZero_transp[9];
			float matRotDefAxeMat_transp[9];
			TransposeMatrix_3x3(elevationAlgoData->matRotZero, matRotZero_transp);
			TransposeMatrix_3x3(matRotDefAxeMat, matRotDefAxeMat_transp);

			// Matrice finale avec zéro d'orientation qui se fait au premier pas de temps
				float rotf_temp1[9];
				float rotf_temp2[9];
				MatMultiply_3x3(matRotDefAxeMat, matRot_fusion1, rotf_temp1);
				MatMultiply_3x3(rotf_temp1, matRotZero_transp, rotf_temp2);
				MatMultiply_3x3(rotf_temp2, matRotDefAxeMat_transp, elevationAlgoData->matRotFinal);//rotf = matRotDefAxeMat * matRot_fusion1 * Mat_zero' * matRotDefAxeMat';
					
			// Matrice finale sans zéro d'orientation (orientation brute)
				float rotf_temp1_raw[9];
				MatMultiply_3x3(matRotDefAxeMat, matRot_fusion1, rotf_temp1_raw);
				MatMultiply_3x3(rotf_temp1_raw, matRotDefAxeMat_transp, elevationAlgoData->matRotAbsolute);//rotf = matRotDefAxeMat * matRot_fusion1 * matRotDefAxeMat';


		elevationResult_t elevationResult;
		elevationResult.elevationRelative = MatToEulerTTzelev(elevationAlgoData->matRotFinal);
		elevationResult.elevationAbsolute = MatToEulerTTzelev(elevationAlgoData->matRotAbsolute);

	
	return elevationResult;
	
}


float TeamAT_IMUFusion_getElevationAccelGyro_oldfortest(float LSM_accelgyr_datarealtime[6], float Ts, int flagfirst, float &uquat_last, float &acc_lowpass_last, float LSM_accelgyr_datarealtime_limited_last[6], double LSM_accelgyr_datarealtime_limited_filt[6], double LSM_accelgyr_datarealtime_limited_gyrx_filt_Y[5],double LSM_accelgyr_datarealtime_limited_gyry_filt_Y[5],double LSM_accelgyr_datarealtime_limited_gyrz_filt_Y[5],double LSM_accelgyr_datarealtime_limited_gyrx_filt_Z[5],    double LSM_accelgyr_datarealtime_limited_gyry_filt_Z[5],double LSM_accelgyr_datarealtime_limited_gyrz_filt_Z[5],double &LSM_accelgyr_datarealtime_limited_gyrx_filt_u0,double &LSM_accelgyr_datarealtime_limited_gyry_filt_u0,double &LSM_accelgyr_datarealtime_limited_gyrz_filt_u0, float RotZaccum_c[9], float rotm_c_fus_xy[9], float Mat_Zero[9], float rotf[9], float rotf_raw[9], float &IMUFusion_Elevationvalue_raw)
{


	//long time1 = micros();


	// PARAMETRES
        float tr = 0*PI/180;
        float Mat_defaxemat[9] = {cos(tr), sin(tr), 0, -sin(tr), cos(tr), 0, 0, 0, 1};
		
        float acc_lowpass = Ts;//%juste pour initialiser, n'a pas d'effet   //%freq = 1/acc_lowpass/2/pi. 0.01=15.91Hz
        float ratelimiter = 60*Ts;//%40
        float ratelimitergyro = 30*Ts;//%20

        float diffnorm_param_min = 1;
        float diffnorm_param_max = 5;
        float uquat=(1-Ts*4);//%juste pour initialiser, n'a pas d'effet
        float uquat_min=(1-Ts*4);//%0.992à500Hz;    gyro*(1-Ts*4)+acc*Ts*4
        float uquat_max=(1-Ts*1.5);//%0.997à500Hz;  gyro*(1-Ts*1.5)+acc*Ts*1.5
        float uquat_rate = 0.005;
        float acc_lowpass_min=Ts;
        float acc_lowpass_max=0.05;
        float acc_lowpass_rate = 0.05;


        double b_gyr[5];
        double a_gyr[5];
		
        if (Ts <= 0.011) // 100 Hz (10ms). low: 30  High: 0.001
        {
            b_gyr[0] = 0.391314695909236; b_gyr[1] = 0; b_gyr[2] = -0.782629391818472; b_gyr[3] = 0 ; b_gyr[4] = 0.391314695909236;
            a_gyr[0] = 1.000000000000000; a_gyr[1] = -1.630406109156383; a_gyr[2] = 0.456757210330416; a_gyr[3] = -0.022156992426718; a_gyr[4] = 0.195805897432362;
        }
        else if (Ts > 0.011 && Ts <= 0.014) //83Hz (12ms)
        {
            b_gyr[0] = 0.529936766134569; b_gyr[1] = 0; b_gyr[2] = -1.059873532269139; b_gyr[3] = 0 ; b_gyr[4] = 0.529936766134569;
            a_gyr[0] = 1.000000000000000; a_gyr[1] = -1.174672752788131; a_gyr[2] = -0.355820401953671; a_gyr[3] = 0.235885117297109; a_gyr[4] = 0.294608049495654;
        }
        else if (Ts > 0.014) //66Hz (15ms)
        {
            b_gyr[0] = 0.800538811070415; b_gyr[1] = 0; b_gyr[2] = -1.601077622140830; b_gyr[3] = 0 ; b_gyr[4] = 0.800538811070415;
            a_gyr[0] = 1.000000000000000; a_gyr[1] = -0.438849705213504; a_gyr[2] = -1.480607586084857; a_gyr[3] = 0.278191151911159; a_gyr[4] = 0.641266167831231;
        }
        else //défaut de 100Hz
        {
            b_gyr[0] = 0.391314695909236; b_gyr[1] = 0; b_gyr[2] = -0.782629391818472; b_gyr[3] = 0 ; b_gyr[4] = 0.391314695909236;
            a_gyr[0] = 1.000000000000000; a_gyr[1] = -1.630406109156383; a_gyr[2] = 0.456757210330416; a_gyr[3] = -0.022156992426718; a_gyr[4] = 0.195805897432362;
        }







        float norme = 1;

	//Serial.print(micros()-time1);Serial.print(";");
	//time1 = micros();
		
	// RATE LIMITER
        float LSM_accelgyr_datarealtime_limited[6];
		for (int k=0; k<6; k++)
		{
			LSM_accelgyr_datarealtime_limited[k] = LSM_accelgyr_datarealtime[k];
		}
        if (flagfirst == 1)
		{
			for (int k=0; k<3; k++)
			{
				if ((LSM_accelgyr_datarealtime_limited[k] - LSM_accelgyr_datarealtime_limited_last[k] > ratelimiter))
				{
					LSM_accelgyr_datarealtime_limited[k] = LSM_accelgyr_datarealtime_limited_last[k] + ratelimiter;
				}
				else if ((LSM_accelgyr_datarealtime_limited[k] - LSM_accelgyr_datarealtime_limited_last[k] < -ratelimiter))
				{
					LSM_accelgyr_datarealtime_limited[k] = LSM_accelgyr_datarealtime_limited_last[k] - ratelimiter;
				}
			}
			for (int k=3; k<6; k++)
			{
				if ((LSM_accelgyr_datarealtime_limited[k] - LSM_accelgyr_datarealtime_limited_last[k] > ratelimitergyro))
				{
					LSM_accelgyr_datarealtime_limited[k] = LSM_accelgyr_datarealtime_limited_last[k] + ratelimitergyro;
				}
				else if ((LSM_accelgyr_datarealtime_limited[k] - LSM_accelgyr_datarealtime_limited_last[k] < -ratelimitergyro))
				{
					LSM_accelgyr_datarealtime_limited[k] = LSM_accelgyr_datarealtime_limited_last[k] - ratelimitergyro;
				}
			}
		}
		for (int k=0; k<6; k++)
		{
			LSM_accelgyr_datarealtime_limited_last[k] = LSM_accelgyr_datarealtime_limited[k];
		}

		
	//Serial.print(micros()-time1);Serial.print(";");
	//time1 = micros();
	
	// ALGO TRAITEMENT JUMPJUMP
        norme = sqrt(LSM_accelgyr_datarealtime_limited[0]*LSM_accelgyr_datarealtime_limited[0] + LSM_accelgyr_datarealtime_limited[1]*LSM_accelgyr_datarealtime_limited[1] + LSM_accelgyr_datarealtime_limited[2]*LSM_accelgyr_datarealtime_limited[2]);
        float diffnorme = fabs(norme-9.81);
		
		 if (diffnorme > diffnorm_param_max)
		 {
			 uquat = uquat_max;
			 acc_lowpass = acc_lowpass_max;
		 }
		 else if (diffnorme < diffnorm_param_min)
		 {
			 uquat = uquat_min;
			 acc_lowpass = acc_lowpass_min;
		 }
		 else
		 {
			 uquat = uquat_min + (uquat_max-uquat_min)*(diffnorme-diffnorm_param_min)/(diffnorm_param_max-diffnorm_param_min);
			 acc_lowpass = acc_lowpass_min + (acc_lowpass_max-acc_lowpass_min)*(diffnorme-diffnorm_param_min)/(diffnorm_param_max-diffnorm_param_min);
		 }

		 if (uquat - uquat_last < -uquat_rate*Ts)
		 {
			 uquat = uquat_last - uquat_rate*Ts;
		 }
		 uquat_last = uquat;

		 if (acc_lowpass - acc_lowpass_last < -acc_lowpass_rate*Ts)
		 {
			acc_lowpass = acc_lowpass_last - acc_lowpass_rate*Ts;
		 }
		 acc_lowpass_last = acc_lowpass;

		 
		 
	//Serial.print(micros()-time1);Serial.print(";");
	//time1 = micros();
	
	
	
	// FILTRES ACCEL-GYRO 
		for (int k=0; k<3; k++)
		{
            TeamAT_Filters_LowPass_Classicfirst_double((double)LSM_accelgyr_datarealtime_limited[k], LSM_accelgyr_datarealtime_limited_filt[k], flagfirst, (double)acc_lowpass, Ts);
		}
        LSM_accelgyr_datarealtime_limited_filt[3] = TeamAT_Filters_ab_vsimple_o4_double(b_gyr,a_gyr,(double)LSM_accelgyr_datarealtime_limited[3],LSM_accelgyr_datarealtime_limited_gyrx_filt_Y,LSM_accelgyr_datarealtime_limited_gyrx_filt_Z,LSM_accelgyr_datarealtime_limited_gyrx_filt_u0,flagfirst,2);
        LSM_accelgyr_datarealtime_limited_filt[4] = TeamAT_Filters_ab_vsimple_o4_double(b_gyr,a_gyr,(double)LSM_accelgyr_datarealtime_limited[4],LSM_accelgyr_datarealtime_limited_gyry_filt_Y,LSM_accelgyr_datarealtime_limited_gyry_filt_Z,LSM_accelgyr_datarealtime_limited_gyry_filt_u0,flagfirst,2);
        LSM_accelgyr_datarealtime_limited_filt[5] = TeamAT_Filters_ab_vsimple_o4_double(b_gyr,a_gyr,(double)LSM_accelgyr_datarealtime_limited[5],LSM_accelgyr_datarealtime_limited_gyrz_filt_Y,LSM_accelgyr_datarealtime_limited_gyrz_filt_Z,LSM_accelgyr_datarealtime_limited_gyrz_filt_u0,flagfirst,2);

        //qDebug() << LSM_accelgyr_datarealtime_limited_filt[3];
		
	//Serial.print(micros()-time1);Serial.print(";");
	//time1 = micros();
		
		
	// FUSION	
        float rotm_c_a_xy[9];
        float LSM_accelgyr_datarealtime_limited_filt_acc[3];
        LSM_accelgyr_datarealtime_limited_filt_acc[0] = (float)LSM_accelgyr_datarealtime_limited_filt[0];
        LSM_accelgyr_datarealtime_limited_filt_acc[1] = (float)LSM_accelgyr_datarealtime_limited_filt[1];
        LSM_accelgyr_datarealtime_limited_filt_acc[2] = (float)LSM_accelgyr_datarealtime_limited_filt[2];
		
		// Initialisation
        float rotm3[9];
        if (flagfirst == 0)
		{
			acctorotmatrix(rotm3, LSM_accelgyr_datarealtime_limited_filt_acc);
			for (int k=0;k<9;k++)
			{
				rotm_c_fus_xy[k] = rotm3[k];
			}
			RotZaccum_c[0] = 1;RotZaccum_c[1] = 0;RotZaccum_c[2] = 0;RotZaccum_c[3] = 0;RotZaccum_c[4] = 1;RotZaccum_c[5] = 0;RotZaccum_c[6] = 0;RotZaccum_c[7] = 0;RotZaccum_c[8] = 1;
		}
		else 
		{
			// else ca garde ceux de lautre pas de temps
		}
		
		
	//Serial.print(micros()-time1);Serial.print(";");
	//time1 = micros();
		
		// Combinaison accel-gyro
			// Accelero
				acctorotmatrix(rotm_c_a_xy, LSM_accelgyr_datarealtime_limited_filt_acc);
				//On re-aligne la rotation Z sur l'ancien repere obtenu
                    float rotm_c_a_xy_transp[9];
                    TransposeMatrix(rotm_c_a_xy, rotm_c_a_xy_transp, 3, 3);
                    float Qtemp[9];
                    MatMultiply(rotm_c_fus_xy, rotm_c_a_xy_transp, Qtemp, 3,3,3);

                    float phitemp=0;
                    if (!isnan(Qtemp[3]) && !isinf(Qtemp[3]) && !isnan(Qtemp[0]) && !isinf(Qtemp[0]))
                    {
						phitemp = atan2(Qtemp[3],Qtemp[0]);
					}
					else
					{
						phitemp = 0;
					}
                    float Rtemp2[9];
					Rtemp2[0] = cos(phitemp); Rtemp2[1] = -sin(phitemp); Rtemp2[2] = 0;
					Rtemp2[3] = sin(phitemp); Rtemp2[4] = cos(phitemp); Rtemp2[5] = 0;
					Rtemp2[6] = 0; Rtemp2[7] = 0; Rtemp2[8] = 1;
                    float Rtemp2_transp[9];
                    TransposeMatrix(Rtemp2, Rtemp2_transp, 3, 3);
                    float Qtemp2[9];
					MatMultiply_3x3(Rtemp2_transp, Qtemp, Qtemp2);
                    float rotm_c_a_xy2[9];
					MatMultiply_3x3(Rtemp2, rotm_c_a_xy, rotm_c_a_xy2);

	//Serial.print(micros()-time1);Serial.print(";");
	//time1 = micros();			
					
			// Gyroscope
                float LSM_accelgyr_datarealtime_limited_filt_gyro[3];
                LSM_accelgyr_datarealtime_limited_filt_gyro[0] = (float)LSM_accelgyr_datarealtime_limited_filt[3]*Ts;
                LSM_accelgyr_datarealtime_limited_filt_gyro[1] = (float)LSM_accelgyr_datarealtime_limited_filt[4]*Ts;
                LSM_accelgyr_datarealtime_limited_filt_gyro[2] = (float)LSM_accelgyr_datarealtime_limited_filt[5]*Ts;
                float LSM_accelgyr_datarealtime_limited_filt_gyro2[3];
				MatMultiply(rotm_c_fus_xy, LSM_accelgyr_datarealtime_limited_filt_gyro, LSM_accelgyr_datarealtime_limited_filt_gyro2, 3,1,3);
                float rpsi = LSM_accelgyr_datarealtime_limited_filt_gyro2[0];
                float rtheta = LSM_accelgyr_datarealtime_limited_filt_gyro2[1];
                float rphi = LSM_accelgyr_datarealtime_limited_filt_gyro2[2];
                float Rz[9] = {cos(rphi), -sin(rphi), 0, sin(rphi), cos(rphi), 0, 0, 0, 1};
                float Ry[9] = {cos(rtheta), 0, sin(rtheta), 0, 1, 0, -sin(rtheta), 0, cos(rtheta)};
                float Rx[9] = {1, 0, 0, 0, cos(rpsi), -sin(rpsi), 0, sin(rpsi), cos(rpsi)};
                float rotm_c_g_xy_temp1[9];
				MatMultiply_3x3(Ry, Rx, rotm_c_g_xy_temp1);
                float rotm_c_g_xy[9];
				MatMultiply_3x3(rotm_c_g_xy_temp1, rotm_c_fus_xy, rotm_c_g_xy);

	//Serial.print(micros()-time1);Serial.print(";");
	//time1 = micros();
	
			// Fusion
        float quat_acc[4];float quat_acc2[4];
        float quat_gyr[4];float quat_gyr2[4];
				MatRotationToQuat(quat_acc,rotm_c_a_xy2);
				MatRotationToQuat(quat_gyr,rotm_c_g_xy);
				
        Normalize4(quat_acc, quat_acc2);
        Normalize4(quat_gyr, quat_gyr2);

        float quat_pos = DotProduct(quat_acc2, quat_gyr2, 4);
        float quat_neg = -1*quat_pos;
				
        if (acosf(quat_neg) < acosf(quat_pos))
        {
            quat_gyr2[0] = -quat_gyr2[0];
            quat_gyr2[1] = -quat_gyr2[1];
            quat_gyr2[2] = -quat_gyr2[2];
            quat_gyr2[3] = -quat_gyr2[3];
        }


        float quatmultquat = DotProduct(quat_acc2, quat_gyr2, 4);
        if (quatmultquat > 1)
        {
            quatmultquat = 1;
        }
        else if (quatmultquat < -1)
        {
            quatmultquat = -1;
        }
        float theta_quat = acosf(quatmultquat);

        float theta_fus[4];
        if (fabs(theta_quat) < 0.000001 ||(fabs(theta_quat-PI) < 0.000001) || (fabs(theta_quat+PI) < 0.000001))
        {
              theta_fus[0] = quat_acc2[0];
              theta_fus[1] = quat_acc2[1];
              theta_fus[2] = quat_acc2[2];
              theta_fus[3] = quat_acc2[3];
        }
        else
        {
            float sinthetaQuatinv = 1 / sinf(theta_quat);
            float sin1muQuat = sinf((1-uquat)*theta_quat);
            float sinuQuatthetaQuat = sinf(uquat*theta_quat);

            theta_fus[0] = (sin1muQuat * quat_acc2[0] + sinuQuatthetaQuat * quat_gyr2[0]) * sinthetaQuatinv;
            theta_fus[1] = (sin1muQuat * quat_acc2[1] + sinuQuatthetaQuat * quat_gyr2[1]) * sinthetaQuatinv;
            theta_fus[2] = (sin1muQuat * quat_acc2[2] + sinuQuatthetaQuat * quat_gyr2[2]) * sinthetaQuatinv;
            theta_fus[3] = (sin1muQuat * quat_acc2[3] + sinuQuatthetaQuat * quat_gyr2[3]) * sinthetaQuatinv;
        }
        float theta_fus2[4];
        Normalize4(theta_fus,theta_fus2);
        quatToMatRotation(rotm_c_fus_xy, theta_fus2);

				
        float RotZaccum_c_temp[9];
        VectCopy(RotZaccum_c_temp,RotZaccum_c, 9);
        MatMultiply_3x3(RotZaccum_c_temp, Rz, RotZaccum_c);
        MatMultiply_3x3(RotZaccum_c, rotm_c_fus_xy, rotm3);
				
				
	//Serial.print(micros()-time1);Serial.print(";");
	//time1 = micros();
				
        //float Mat_zero[9];
        if (flagfirst == 0)
        {
            VectCopy(Mat_Zero,rotm3,9);
        }
        float Mat_zero_transp[9];
        float Mat_defaxemat_transp[9];
        TransposeMatrix_3x3(Mat_Zero, Mat_zero_transp);
        TransposeMatrix_3x3(Mat_defaxemat, Mat_defaxemat_transp);

        // Matrice finale avec zéro d'orientation qui se fait au premier pas de temps
            float rotf_temp1[9];
            float rotf_temp2[9];
            MatMultiply_3x3(Mat_defaxemat, rotm3, rotf_temp1);
            MatMultiply_3x3(rotf_temp1, Mat_zero_transp, rotf_temp2);
            MatMultiply_3x3(rotf_temp2, Mat_defaxemat_transp, rotf);//rotf = Mat_defaxemat * rotm3 * Mat_zero' * Mat_defaxemat';
				
        // Matrice finale sans zéro d'orientation (orientation brute)
            float rotf_temp1_raw[9];
            MatMultiply_3x3(Mat_defaxemat, rotm3, rotf_temp1_raw);
            MatMultiply_3x3(rotf_temp1_raw, Mat_defaxemat_transp, rotf_raw);//rotf = Mat_defaxemat * rotm3 * Mat_defaxemat';


            float IMUFusion_Elevationvalue = MatToEulerTTzelev(rotf);//en return
            IMUFusion_Elevationvalue_raw = MatToEulerTTzelev(rotf_raw);//dans la fonction

	
    return IMUFusion_Elevationvalue;
}


void acctorotmatrix(float rotmatrix[9], float acc[3])
{
	
    float accnorm[3];
	Normalize3(acc, accnorm);
	
    float m31 = accnorm[0];
    float m32 = accnorm[1];
    float m33 = accnorm[2];
    
    float m21 = 0;
    
    float m11 = sqrt(1-accnorm[0]*accnorm[0]);
    float m22 = 0;
    if (fabs(m11)-0.001 < 0)
	{
        m22=1;
	}
    else
	{
        m22=accnorm[2]/m11;
    }
    
    float m13 = -accnorm[0]*m22;
    
    float m23 = 0;
    if (fabs(m11)-0.001 < 0)
	{
       m23 = sqrt(fabs((1-m13*m13-accnorm[2]*accnorm[2])));//% absolu pour erreur numérique
	}
    else
	{
       m23 = -accnorm[1]/m11; 
    }
    
    float m12=0;
    if (fabs(accnorm[2])-0.001 < 0)
	{
        m12=m23*accnorm[0]; //%pourrait prendre lui toujours ?
	}
    else
	{
        m12=m13*accnorm[1]/accnorm[2];
    }
    
	rotmatrix[0] = m11; rotmatrix[1] = m12; rotmatrix[2] = m13;
    rotmatrix[3] = m21; rotmatrix[4] = m22; rotmatrix[5] = m23;
	rotmatrix[6] = m31; rotmatrix[7] = m32; rotmatrix[8] = m33;
	
}




void ElevationMetrics_init(elevationMetrics_t *elevationMetrics)
{
		elevationMetrics->paramElevHigh = 70; 		
		elevationMetrics->paramElevLow = elevationMetrics->paramElevHigh/2;			
		elevationMetrics->paramElevHigh_min = 30;	
		elevationMetrics->paramElevHigh_max = 135;	
		
		elevationMetrics->paramTimeFinite = 120;		
		elevationMetrics->paramTimeFinite_min = 60;	
		elevationMetrics->paramTimeFinite_max = 60*20;	
		
		elevationMetrics->elevTime_total = 0;
		elevationMetrics->elevTime_finite= 0;
		elevationMetrics->elevTime_finite_resetfeedback= 0;
		elevationMetrics->elevTime_finite_index= 0;
		elevationMetrics->elevTime_finite_lastElevState= 0;
				
		elevationMetrics->nbElev_total= 0;
		elevationMetrics->nbElev_finite= 0;	
		elevationMetrics->nbElev_finite_resetfeedback= 0;
		elevationMetrics->nvElev_finite_lastelevState= 0;
					
		elevationMetrics->elevCurrentState = 0;
					
		elevationMetrics->finiteData_maxsize = 500;
        for (int i = 0; i<elevationMetrics->finiteData_maxsize; i++)
        {
            elevationMetrics->elevTime_finite_data[i][0] = 0;
            elevationMetrics->elevTime_finite_data[i][1] = 0;
            elevationMetrics->nbElev_finite_data[i][0] = 0;
            elevationMetrics->nbElev_finite_data[i][1] = 0;
			elevationMetrics->nbElev_finite_data[i][2] = 0;
        }
		
		elevationMetrics->resetValueFeedback_timing = 0;
}


void TeamAT_IMUFusion_getElevationMetrics(elevationMetrics_t *elevationMetrics, float elevation, long &counter_elev, float Ts, int reset, int resetfeedback)
{
    double timing = (double)(counter_elev*Ts);


	// Reset Metrics
		if (reset == 1)
		{
			counter_elev=0;
			elevationMetrics->elevTime_total = 0;
			elevationMetrics->elevCurrentState = 0;
			elevationMetrics->nbElev_total = 0;
			elevationMetrics->elevTime_finite_index = 0;
			elevationMetrics->elevTime_finite_lastElevState = 0;
			elevationMetrics->elevTime_finite = 0;
			elevationMetrics->nvElev_finite_lastelevState = 0;
			elevationMetrics->nbElev_finite = 0;

			for (int i = 0; i<elevationMetrics->finiteData_maxsize; i++)
			{
				elevationMetrics->elevTime_finite_data[i][0] = 0;
				elevationMetrics->elevTime_finite_data[i][1] = 0;
				elevationMetrics->nbElev_finite_data[i][0] = 0;
				elevationMetrics->nbElev_finite_data[i][1] = 0;
				elevationMetrics->nbElev_finite_data[i][2] = 0;
			}
			
			elevationMetrics->resetValueFeedback_timing = 0;
		}
		
		if (resetfeedback == 1)
		{
			elevationMetrics->resetValueFeedback_timing = timing;
			resetfeedback = 0;
		}

	
    // Temps elev total
        if (elevation >= elevationMetrics->paramElevHigh)
        {
            elevationMetrics->elevTime_total = elevationMetrics->elevTime_total + Ts;
        }

    // Nb elevation totale
        if (elevation >= elevationMetrics->paramElevHigh && elevationMetrics->elevCurrentState == 0)
        {
            elevationMetrics->elevCurrentState = 1;
            elevationMetrics->nbElev_total = elevationMetrics->nbElev_total+1;
            elevationMetrics->elevTime_total = elevationMetrics->elevTime_total - Ts;
        }
        else if ((elevation <= elevationMetrics->paramElevLow) && elevationMetrics->elevCurrentState == 1)
        {
            elevationMetrics->elevCurrentState = 0;
        }

    // TEMPS DERNIERES MINUTES (FINITE)
        // Remplir vecteur de data

            if (elevation >= elevationMetrics->paramElevHigh && elevationMetrics->elevTime_finite_lastElevState == 0)
            {
                if (elevationMetrics->elevTime_finite_index < elevationMetrics->finiteData_maxsize)
                {
                    elevationMetrics->elevTime_finite_index = elevationMetrics->elevTime_finite_index+1;
                }
               elevationMetrics->elevTime_finite_lastElevState = 1;
               elevationMetrics->elevTime_finite_data[elevationMetrics->elevTime_finite_index-1][0] = timing;
               elevationMetrics->elevTime_finite_data[elevationMetrics->elevTime_finite_index-1][1] = timing;
            }
            else if (elevation >= elevationMetrics->paramElevHigh)
            {
               elevationMetrics->elevTime_finite_data[elevationMetrics->elevTime_finite_index-1][1] = timing;
            }
            else if (elevation < elevationMetrics->paramElevHigh)
            {
                elevationMetrics->elevTime_finite_lastElevState = 0;
            }


            // Deleter points passes dates
                if (elevationMetrics->elevTime_finite_index > 0)
                {
                    //for (int j = elevationMetrics->elevTime_finite_index; j>=1; j--)
                    //{
                       int j=1;
                       if (elevationMetrics->elevTime_finite_data[j-1][1] < (timing - elevationMetrics->paramTimeFinite))
                       {
                           for (int k=j-1; k<=elevationMetrics->elevTime_finite_index-2; k++)
                           {
                               elevationMetrics->elevTime_finite_data[k][0] = elevationMetrics->elevTime_finite_data[k+1][0];
                               elevationMetrics->elevTime_finite_data[k][1] = elevationMetrics->elevTime_finite_data[k+1][1];
                           }
                           elevationMetrics->elevTime_finite_data[elevationMetrics->elevTime_finite_index-1][0] = 0;
                           elevationMetrics->elevTime_finite_data[elevationMetrics->elevTime_finite_index-1][1] = 0;
                           elevationMetrics->elevTime_finite_index = elevationMetrics->elevTime_finite_index - 1;
                       }
                       else if (elevationMetrics->elevTime_finite_data[j-1][0] < timing - elevationMetrics->paramTimeFinite)
                       {
                           elevationMetrics->elevTime_finite_data[j-1][0] = timing - elevationMetrics->paramTimeFinite;
                       }
                   //}
                }


            elevationMetrics->elevTime_finite=0;
			elevationMetrics->elevTime_finite_resetfeedback = 0;
            for (int j = 0; j < elevationMetrics->elevTime_finite_index; j++)
            {
                elevationMetrics->elevTime_finite = elevationMetrics->elevTime_finite + elevationMetrics->elevTime_finite_data[j][1] - elevationMetrics->elevTime_finite_data[j][0];
				
				if (elevationMetrics->elevTime_finite_data[j][1] > elevationMetrics->resetValueFeedback_timing)
				{
					if (elevationMetrics->elevTime_finite_data[j][0] < elevationMetrics->resetValueFeedback_timing)
					{
						elevationMetrics->elevTime_finite_resetfeedback = elevationMetrics->elevTime_finite_resetfeedback + elevationMetrics->elevTime_finite_data[j][1] - elevationMetrics->resetValueFeedback_timing;
					}
					else
					{
						elevationMetrics->elevTime_finite_resetfeedback = elevationMetrics->elevTime_finite_resetfeedback + elevationMetrics->elevTime_finite_data[j][1] - elevationMetrics->elevTime_finite_data[j][0];
					}
				}
            }


        // ELEVATION DERNIERE MINUTE
            if (elevation >= elevationMetrics->paramElevHigh && elevationMetrics->nvElev_finite_lastelevState == 0)
            {
                if (elevationMetrics->nbElev_finite < elevationMetrics->finiteData_maxsize)
                {
                    elevationMetrics->nbElev_finite = elevationMetrics->nbElev_finite + 1;
                }
                elevationMetrics->nvElev_finite_lastelevState = 1;
				elevationMetrics->nbElev_finite_data[elevationMetrics->nbElev_finite-1][0] = timing;
                elevationMetrics->nbElev_finite_data[elevationMetrics->nbElev_finite-1][1] = timing;
                elevationMetrics->nbElev_finite_data[elevationMetrics->nbElev_finite-1][2] = 1;
            }
            else if (elevation >= elevationMetrics->paramElevHigh && elevationMetrics->nvElev_finite_lastelevState == 1)
            {
                elevationMetrics->nbElev_finite_data[elevationMetrics->nbElev_finite-1][1] = timing;
                elevationMetrics->nbElev_finite_data[elevationMetrics->nbElev_finite-1][2] = 1;
            }
            else if (elevation <= elevationMetrics->paramElevLow && elevationMetrics->nvElev_finite_lastelevState == 1)
            {
                elevationMetrics->nvElev_finite_lastelevState = 0;
                elevationMetrics->nbElev_finite_data[elevationMetrics->nbElev_finite-1][2] = 0;
            }

            // Deleter points passes dates
                if (elevationMetrics->nbElev_finite > 0)
                {
                    //for (int j=elevationMetrics->nbElev_finite; j>=1; j--)
                    //{
                    int j=1;
                       if ((elevationMetrics->nbElev_finite_data[j-1][1] < timing-elevationMetrics->paramTimeFinite) && elevationMetrics->nbElev_finite_data[j-1][2]==0)
                       {
                           for (int k=j-1;k<=elevationMetrics->nbElev_finite-2;k++)
                           {
							   elevationMetrics->nbElev_finite_data[k][0] = elevationMetrics->nbElev_finite_data[k+1][0];
                               elevationMetrics->nbElev_finite_data[k][1] = elevationMetrics->nbElev_finite_data[k+1][1];
                               elevationMetrics->nbElev_finite_data[k][2] = elevationMetrics->nbElev_finite_data[k+1][2];
                           }
						   elevationMetrics->nbElev_finite_data[elevationMetrics->nbElev_finite-1][0] = 0;
                           elevationMetrics->nbElev_finite_data[elevationMetrics->nbElev_finite-1][1] = 0;
                           elevationMetrics->nbElev_finite_data[elevationMetrics->nbElev_finite-1][2] = 0;
                           elevationMetrics->nbElev_finite = elevationMetrics->nbElev_finite - 1;
                       }
                   //}
                }
				
				
			elevationMetrics->nbElev_finite_resetfeedback = 0;
            for (int j = 0; j < elevationMetrics->nbElev_finite; j++)
            {
                if (elevationMetrics->nbElev_finite_data[j][0] > elevationMetrics->resetValueFeedback_timing)
				{
					elevationMetrics->nbElev_finite_resetfeedback = elevationMetrics->nbElev_finite_resetfeedback + 1;
				}
            }
				
				

}


void TeamAT_IMUFusion_getElevationMetrics_oldfortest(float elevation, long &counter_elev, float Ts, float param_elevhigh, float param_elevlow, float param_temps_timingback, double &temps_elev_all,int &elev_state, int &nb_elev_total, int &temps_finite_index, int &temps_finite_lastelev, int maxdata75vectsize, float temps_finite_data75[500][2], double &temps_finite, int &elev_finite_lastelev, int &elev_finite_index,float elev_finite_data75[500][3], int reset, int resetfeedback, double &timingResetFeedback, double &temps_finite_feedback, int &elev_finite_feedback)
{
    double timing = (double)(counter_elev*Ts);

    if (reset == 1)
    {
        counter_elev=0;
        temps_elev_all = 0;
        elev_state = 0;
        nb_elev_total = 0;
        temps_finite_index = 0;
        temps_finite_lastelev = 0;
        temps_finite = 0;
        elev_finite_lastelev = 0;
        elev_finite_index = 0;

        for (int i = 0; i<maxdata75vectsize; i++)
        {
            temps_finite_data75[i][0] = 0;
            temps_finite_data75[i][1] = 0;
            elev_finite_data75[i][0] = 0;
            elev_finite_data75[i][1] = 0;
			elev_finite_data75[i][2] = 0;
        }
		
		timingResetFeedback = 0;
    }
	
	if (resetfeedback == 1)
	{
		timingResetFeedback = timing;
		resetfeedback = 0;
	}

	
	

    // Temps elev total
        if (elevation >= param_elevhigh)
        {
            temps_elev_all = temps_elev_all + Ts;
        }

    // Nb elevation totale
        if (elevation >= param_elevhigh && elev_state == 0)
        {
            elev_state = 1;
            nb_elev_total = nb_elev_total+1;
            temps_elev_all = temps_elev_all - Ts;
        }
        else if ((elevation <= param_elevlow) && elev_state == 1)
        {
            elev_state = 0;
        }



    // TEMPS DERNIERE MINUTE
        // Remplir vecteur de data

            if (elevation >= param_elevhigh && temps_finite_lastelev == 0)
            {
                if (temps_finite_index < maxdata75vectsize)
                {
                    temps_finite_index = temps_finite_index+1;
                }
               temps_finite_lastelev = 1;
               temps_finite_data75[temps_finite_index-1][0] = timing;
               temps_finite_data75[temps_finite_index-1][1] = timing;
            }
            else if (elevation >= param_elevhigh)
            {
               temps_finite_data75[temps_finite_index-1][1] = timing;
            }
            else if (elevation < param_elevhigh)
            {
                temps_finite_lastelev = 0;
            }



            // Deleter points passes dates
                if (temps_finite_index > 0)
                {
                    //for (int j = temps_finite_index; j>=1; j--)
                    //{
                       int j=1;
                       if (temps_finite_data75[j-1][1] < (timing - param_temps_timingback))
                       {
                           for (int k=j-1; k<=temps_finite_index-2; k++)
                           {
                               temps_finite_data75[k][0] = temps_finite_data75[k+1][0];
                               temps_finite_data75[k][1] = temps_finite_data75[k+1][1];
                           }
                           temps_finite_data75[temps_finite_index-1][0] = 0;
                           temps_finite_data75[temps_finite_index-1][1] = 0;
                           temps_finite_index = temps_finite_index - 1;
                       }
                       else if (temps_finite_data75[j-1][0] < timing - param_temps_timingback)
                       {
                           temps_finite_data75[j-1][0] = timing - param_temps_timingback;
                       }
                   //}
                }



            temps_finite=0;
			temps_finite_feedback = 0;
            for (int j = 0; j < temps_finite_index; j++)
            {
                temps_finite = temps_finite + temps_finite_data75[j][1] - temps_finite_data75[j][0];
				
				if (temps_finite_data75[j][1] > timingResetFeedback)
				{
					if (temps_finite_data75[j][0] < timingResetFeedback)
					{
						temps_finite_feedback = temps_finite_feedback + temps_finite_data75[j][1] - timingResetFeedback;
					}
					else
					{
						temps_finite_feedback = temps_finite_feedback + temps_finite_data75[j][1] - temps_finite_data75[j][0];
					}
				}
            }



        // ELEVATION DERNIERE MINUTE
            if (elevation >= param_elevhigh && elev_finite_lastelev == 0)
            {
                if (elev_finite_index < maxdata75vectsize)
                {
                    elev_finite_index = elev_finite_index + 1;
                }
                elev_finite_lastelev = 1;
				elev_finite_data75[elev_finite_index-1][0] = timing;
                elev_finite_data75[elev_finite_index-1][1] = timing;
                elev_finite_data75[elev_finite_index-1][2] = 1;
            }
            else if (elevation >= param_elevhigh && elev_finite_lastelev == 1)
            {
                elev_finite_data75[elev_finite_index-1][1] = timing;
                elev_finite_data75[elev_finite_index-1][2] = 1;
            }
            else if (elevation <= param_elevlow && elev_finite_lastelev == 1)
            {
                elev_finite_lastelev = 0;
                elev_finite_data75[elev_finite_index-1][2] = 0;
            }

            // Deleter points passes dates
                if (elev_finite_index > 0)
                {
                    //for (int j=elev_finite_index; j>=1; j--)
                    //{
                    int j=1;
                       if ((elev_finite_data75[j-1][1] < timing-param_temps_timingback) && elev_finite_data75[j-1][2]==0)
                       {
                           for (int k=j-1;k<=elev_finite_index-2;k++)
                           {
							   elev_finite_data75[k][0] = elev_finite_data75[k+1][0];
                               elev_finite_data75[k][1] = elev_finite_data75[k+1][1];
                               elev_finite_data75[k][2] = elev_finite_data75[k+1][2];
                           }
						   elev_finite_data75[elev_finite_index-1][0] = 0;
                           elev_finite_data75[elev_finite_index-1][1] = 0;
                           elev_finite_data75[elev_finite_index-1][2] = 0;
                           elev_finite_index = elev_finite_index - 1;
                       }
                   //}
                }
				
				
			elev_finite_feedback = 0;
            for (int j = 0; j < elev_finite_index; j++)
            {
                if (elev_finite_data75[j][0] > timingResetFeedback)
				{
					elev_finite_feedback = elev_finite_feedback + 1;
				}
            }
				
				

}
