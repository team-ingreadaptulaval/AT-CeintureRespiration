#include "TeamATmath_v1.h"


float Norm3(float vector[3])
{
	float result = 0.0f;

	float x,y,z;

	x = vector[0] * vector[0];
	y = vector[1] * vector[1];
	z = vector[2] * vector[2];

    result = sqrtf( x + y + z );

	return result;
}

float Norm4(float vector[4])
{
	float result = 0.0f;

	float x,y,z,a;

	x = vector[0] * vector[0];
	y = vector[1] * vector[1];
	z = vector[2] * vector[2];
	a = vector[3] * vector[3];

    result = sqrtf( x + y + z + a );

	return result;
}


void Normalize3(float in[3], float out[3])
{
  float Norme=sqrtf(in[0]*in[0] + in[1]*in[1] + in[2]*in[2]);
  float Norme_inv = 1/Norme;
  if (Norme>0.00001)
  {
   out[0]=in[0] * Norme_inv;
   out[1]=in[1] * Norme_inv;
   out[2]=in[2] * Norme_inv;
  }
  else
  {
   out[0]=0;
   out[1]=0;
   out[2]=0;
  }
}


void Normalize4(float in[4], float out[4])
{
  float Norme=sqrtf(in[0]*in[0] + in[1]*in[1] + in[2]*in[2] + in[3]*in[3]);
  float Norme_inv = 1/Norme;
  if (Norme>0.00001)
  {
   out[0]=in[0] * Norme_inv;
   out[1]=in[1] * Norme_inv;
   out[2]=in[2] * Norme_inv;
   out[3]=in[3] * Norme_inv;
  }
  else
  {
   out[0]=0;
   out[1]=0;
   out[2]=0;
   out[3]=0;
  }
}



void VectCopy(float* v1, float* v2, int size)
{
	for (int i = 0; i < size; i++)
	{
		v1[i] = v2[i];
	}
}

void VectAdd(float* v1, float* v2, float* result, int size)
{
	for (int i = 0; i < size; i++)
	{
		result[i] = v1[i] + v2[i];
	}
}

void VectSubs(float* v1, float* v2, float* result, int size)
{
	for (int i = 0; i < size; i++)
	{
		result[i] = v1[i] - v2[i];
	}
}

void VectMultScalar(float* v, float s, float* result, int size)
{
	for (int i = 0; i < size; i++)
	{
		result[i] = v[i] * s;
	}
}




float DotProduct(float* vectorA, float* vectorB, int size)
{
	float result = 0;

	for(int i = 0; i < size; i++)
	{
		result += vectorA[i] * vectorB[i];
	}

	return result;
}

void CrossProduct(float vectorA[3], float vectorB[3], float result[3])
{
	result[0] = (vectorA[1] * vectorB[2]) - (vectorB[1] * vectorA[2]);
	result[1] = -(vectorA[0] * vectorB[2]) + (vectorB[0] * vectorA[2]);
	result[2] = (vectorA[0] * vectorB[1]) - (vectorB[0] * vectorA[1]);
}

void MatMultiply(float* T1, float* T2, float* result, int m, int n, int p)
{
	float sum = 0.0f;

	for(int i = 0; i < m; i++)
	{
		for(int j = 0; j < n; j++)
		{
			sum=0;
			for (int e = 0; e < p; e++)
			{
				sum += T1[e + (i * p)] * T2[j + (e * n)];
			}
			result[j + (i * n)] = sum;
		}
	}
}

void MatMultiply_3x3(float T1[9], float T2[9], float result[9])
{
	/*float sum = 0.0f;

	for(int i = 0; i < 3; i++)
	{
		for(int j = 0; j < 3; j++)
		{
			sum=0;
			for (int e = 0; e < 3; e++)
			{
				sum += T1[e + (i * 3)] * T2[j + (e * 3)];
			}
			result[j + (i * 3)] = sum;
		}
	}*/
	
	//float sum = 0.0f;
	result[0] = T1[0]*T2[0] + T1[1]*T2[3] + T1[2]*T2[6];
	result[1] = T1[0]*T2[1] + T1[1]*T2[4] + T1[2]*T2[7];
	result[2] = T1[0]*T2[2] + T1[1]*T2[5] + T1[2]*T2[8];
	result[3] = T1[3]*T2[0] + T1[4]*T2[3] + T1[5]*T2[6];
	result[4] = T1[3]*T2[1] + T1[4]*T2[4] + T1[5]*T2[7];
	result[5] = T1[3]*T2[2] + T1[4]*T2[5] + T1[5]*T2[8];
	result[6] = T1[6]*T2[0] + T1[7]*T2[3] + T1[8]*T2[6];
	result[7] = T1[6]*T2[1] + T1[7]*T2[4] + T1[8]*T2[7];
	result[8] = T1[6]*T2[2] + T1[7]*T2[5] + T1[8]*T2[8];
	
	
}




void TransposeMatrix(float* matrix, float* transposed, int m, int n)
{
	for(int i = 0; i < n; i++)
	{
		for(int j = 0; j < m; j++)
		{
			transposed[i * m + j] = matrix[j * n + i];
		}
	}
}


void TransposeMatrix_3x3(float matrix[9], float transposed[9])
{
	/*for(int i = 0; i < 3; i++)
	{
		for(int j = 0; j < 3; j++)
		{
			transposed[i * 3 + j] = matrix[j * 3 + i];
		}
	}*/
   transposed[0] = matrix[0];
   transposed[1] = matrix[3];
   transposed[2] = matrix[6];
   transposed[3] = matrix[1];
   transposed[4] = matrix[4];
   transposed[5] = matrix[7];
   transposed[6] = matrix[2];
   transposed[7] = matrix[5];
   transposed[8] = matrix[8];
}




void Inv3X3(const float x[9], float Fv[9])
{
	float t1;
	float t11;
	float t13;
	float t14;
	float t15;
	float t17;
	float t18;
	float t2;
	float t20;
	float t21;
	float t23;
	float t26;
	float t4;
	float t5;
	float t8;
	float t9;

	t1 = x[4];
	t2 = x[8];
	t4 = x[7];
	t5 = x[5];
	t8 = x[0];
	t9 = t8*t1;
	t11 = t8*t4;
	t13 = x[1];
	t14 = x[3];
	t15 = t13*t14;
	t17 = x[6];
	t18 = t13*t17;
	t20 = x[2];
	t21 = t20*t14;
	t23 = t20*t17;
	t26 = 1 / (t9*t2-t11*t5-t15*t2+t18*t5+t21*t4-t23*t1);
	Fv[0] = (t1*t2-t4*t5)*t26;
	Fv[1] = -(t13*t2-t4*t20)*t26;
	Fv[2] = (t13*t5-t1*t20)*t26;
	Fv[3] = -(t14*t2-t17*t5)*t26;
	Fv[4] = (t8*t2-t23)*t26;
	Fv[5] = -(t8*t5-t21)*t26;
	Fv[6] = (t14*t4-t17*t1)*t26;
	Fv[7] = -(t11-t18)*t26;
	Fv[8] = (t9-t15)*t26;
}



void Inv6X6(const float * matrice, float * matriceOut)
{
	float M1[10], M2[10], M3[10];
	float A[10], B[10], C[10], D[10];
	float A_[10];
	float A_inv[10], Schur[10], Schur_inv[10], CAB[10], CA[10], AB[10];

	short i;


	A[0] = matrice[0];	A[1] = matrice[1];	A[2] = matrice[2];
	A[3] = matrice[6];	A[4] = matrice[7];	A[5] = matrice[8];
	A[6] = matrice[12];	A[7] = matrice[13];	A[8] = matrice[14];

	B[0] = matrice[3];	B[1] = matrice[4];	B[2] = matrice[5];
	B[3] = matrice[9];	B[4] = matrice[10];	B[5] = matrice[11];
	B[6] = matrice[15];	B[7] = matrice[16];	B[8] = matrice[17];

	C[0] = matrice[18];	C[1] = matrice[19];	C[2] = matrice[20];
	C[3] = matrice[24];	C[4] = matrice[25];	C[5] = matrice[26];
	C[6] = matrice[30];	C[7] = matrice[31];	C[8] = matrice[32];

	D[0] = matrice[21];	D[1] = matrice[22];	D[2] = matrice[23];
	D[3] = matrice[27];	D[4] = matrice[28];	D[5] = matrice[29];
	D[6] = matrice[33];	D[7] = matrice[34];	D[8] = matrice[35];

	Inv3X3(A, A_inv);

	MatMultiply(C, A_inv, CA, 3, 3, 3);
	MatMultiply(CA, B, CAB, 3, 3, 3);

	for(i=0; i<9; i++)
		Schur[i] = D[i] - CAB[i];

	Inv3X3(Schur, Schur_inv);

	//Calcul de la première sous matrice A
	MatMultiply(A_inv, B, AB, 3, 3, 3);
	MatMultiply(AB, Schur_inv, M2, 3, 3, 3);
	MatMultiply(M2, C, M1, 3, 3, 3);
	MatMultiply(M1, A_inv, M2, 3, 3, 3);

	for(i=0; i<9; i++)
		A_[i] = A_inv[i] + M2[i];

	//Calcul de la deuxième sous matrice B
	MatMultiply(AB, Schur_inv, M1, 3, 3, 3);

	//La sous matrice B est égal a -M1//


	//Calcul de la troisième sous matrice C
	MatMultiply(Schur_inv, C, M3, 3, 3, 3);
	MatMultiply(M3, A_inv, M2, 3, 3, 3);


	//la sous matrice C est égale a -M2//


	//Calcul de la quatrième sous matrice D//
	//La sous matrice D est directement Schur_inv.//

	matriceOut[0] = A_[0];	matriceOut[1] = A_[1];	matriceOut[2] = A_[2];
	matriceOut[6] = A_[3];	matriceOut[7] = A_[4];	matriceOut[8] = A_[5];
	matriceOut[12] = A_[6];	matriceOut[13] = A_[7];	matriceOut[14] = A_[8];

	matriceOut[3] = -M1[0];		matriceOut[4] = -M1[1];		matriceOut[5] = -M1[2];
	matriceOut[9] = -M1[3];		matriceOut[10] = -M1[4];	matriceOut[11] = -M1[5];
	matriceOut[15] = -M1[6];	matriceOut[16] = -M1[7];	matriceOut[17] = -M1[8];

	matriceOut[18] = -M2[0];	matriceOut[19] = -M2[1];	matriceOut[20] = -M2[2];
	matriceOut[24] = -M2[3];	matriceOut[25] = -M2[4];	matriceOut[26] = -M2[5];
	matriceOut[30] = -M2[6];	matriceOut[31] = -M2[7];	matriceOut[32] = -M2[8];

	matriceOut[21] = Schur_inv[0];	matriceOut[22] = Schur_inv[1];	matriceOut[23] = Schur_inv[2];
	matriceOut[27] = Schur_inv[3];	matriceOut[28] = Schur_inv[4];	matriceOut[29] = Schur_inv[5];
	matriceOut[33] = Schur_inv[6];	matriceOut[34] = Schur_inv[7];	matriceOut[35] = Schur_inv[8];
}



void MatRotationToQuat(float q[4], float R[9])
{
	float traceR = R[0]+R[4]+R[8];
	float a0 = 0;
	if ((1+traceR) < 0)
	{
        a0 = 0.5*sqrtf(-1-traceR);
	}
    else
	{
        a0 = 0.5*sqrtf(1+traceR);
    }
	
	float a1 = 1+R[0]-R[4]-R[8];
	float a2 = 1-R[0]+R[4]-R[8];
	float a3 = 1-R[0]-R[4]+R[8];
	
    if (a1<0)
	{
        a1=0;
	}
    if (a2<0)
	{
        a2=0;
    }
    if (a3<0)
	{
        a3=0;
    }
    
    a1 = 0.5*sqrtf(a1);
    a2 = 0.5*sqrtf(a2);
    a3 = 0.5*sqrtf(a3);

	
    float signR_a1 = 0;
    float signR_a2 = 0;
    float signR_a3 = 0;
	
	if (R[7]-R[5] >= 0)
	{
		signR_a1 = 1;
	}
	else
	{
		signR_a1 = -1;
	}
	
	if (R[2]-R[6] >= 0)
	{
		signR_a2 = 1;
	}
	else
	{
		signR_a2 = -1;
	}
	
	if (R[3]-R[1] >= 0)
	{
		signR_a3 = 1;
	}
	else
	{
		signR_a3 = -1;
	}
	
    a1 = -fabs(a1)*signR_a1;
    a2 = -fabs(a2)*signR_a2;
    a3 = -fabs(a3)*signR_a3;
    
	
	q[0] = a0;
	q[1] = a1;
	q[2] = a2;
	q[3] = a3;
	
	
}






void quatToMatRotation(float R[9], float q[4])
{
	R[0] = 1 - 2*q[2]*q[2] - 2*q[3]*q[3];
	R[1] = 2*q[1]*q[2] + 2*q[0]*q[3];
	R[2] = 2*q[1]*q[3] - 2*q[0]*q[2];
	R[3] = 2*q[1]*q[2] - 2*q[0]*q[3];
	R[4] = 1 - 2*q[1]*q[1] - 2*q[3]*q[3];
	R[5] = 2*q[2]*q[3] + 2*q[0]*q[1];
	R[6] = 2*q[1]*q[3] + 2*q[0]*q[2];
	R[7] = 2*q[2]*q[3] - 2*q[0]*q[1];
    R[8] = 1 - 2*q[1]*q[1] - 2*q[2]*q[2];
}




void MatToEuler_TeamAT(float R[9], float angles[3], int norme, int branch)
{
	if (norme == 0)	//TTz
	{
        float TTz_phi_1 = atan2f(R[2],-R[5]);
        float TTz_theta_1 = atan2f(sinf(TTz_phi_1)*R[2]-cosf(TTz_phi_1)*R[5],R[8]);
        float TTz_sigma_1 = atan2f(-cosf(TTz_phi_1)*R[1]-sinf(TTz_phi_1)*R[4],cosf(TTz_phi_1)*R[0]+sinf(TTz_phi_1)*R[3])+TTz_phi_1;

        float TTz_phi_2 = atan2f(-R[2],R[5]);
        float TTz_theta_2 = atan2f(sinf(TTz_phi_2)*R[2]-cosf(TTz_phi_2)*R[5],R[8]);
        float TTz_sigma_2 = atan2f(-cosf(TTz_phi_2)*R[1]-sinf(TTz_phi_2)*R[4],cosf(TTz_phi_2)*R[0]+sinf(TTz_phi_2)*R[3])+TTz_phi_2;

        if (branch==1)
		{
            angles[0] = TTz_phi_1;
            angles[1] = TTz_theta_1;
            angles[2] = TTz_sigma_1;
		}
        else if (branch==2)
		{
            angles[0] = TTz_phi_2;
            angles[1] = TTz_theta_2;
            angles[2] = TTz_sigma_2;
		}
        else
		{
            if (TTz_theta_1 < 0 || TTz_theta_1 > PI)
			{
                angles[0] = TTz_phi_2;
                angles[1] = TTz_theta_2;
                angles[2] = TTz_sigma_2;
			}
            else
			{
                angles[0] = TTz_phi_1;
                angles[1] = TTz_theta_1;
                angles[2] = TTz_sigma_1;
            }
        }
	}
	else
	{
		angles[0] = 0;
		angles[1] = 0;
		angles[2] = 0;
	}
}

float MatToEulerTTzelev(float R[9])
{

        float TTz_phi_1 = atan2f(R[2],-R[5]);
        float TTz_theta_1 = atan2f(sinf(TTz_phi_1)*R[2]-cosf(TTz_phi_1)*R[5],R[8]);

       return TTz_theta_1;
	   
}




