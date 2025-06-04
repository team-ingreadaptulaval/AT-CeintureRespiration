#ifndef TEAMAT_IMUFUSION_H
#define TEAMAT_IMUFUSION_H




    #include "TeamATmath_v1.h"
    #include "TeamAT_Filters_v1_1.h"
    #include "math.h"


	typedef struct 
	{
		float uquat_last;
		float filtAccLowpass_last;//acc_lowpass_last
		float accelgyrData_limited_last[6];
		double accelgyrData_limited_filt[6];
		double accelgyrData_limited_gyrx_filt_Y[5];
		double accelgyrData_limited_gyry_filt_Y[5];
		double accelgyrData_limited_gyrz_filt_Y[5];
		double accelgyrData_limited_gyrx_filt_Z[5];
		double accelgyrData_limited_gyry_filt_Z[5];
		double accelgyrData_limited_gyrz_filt_Z[5];
		double accelgyrData_limited_gyrx_filt_u0;
		double accelgyrData_limited_gyry_filt_u0;
		double accelgyrData_limited_gyrz_filt_u0;
		float matRotZAccum_c[9];
		float matRotmFus_xy[9];
		float matRotFinal[9];
		float matRotZero[9];
		float matRotAbsolute[9];
	} elevationAlgoData_t;

	typedef struct 
	{
		float elevationRelative;
		float elevationAbsolute;
	} elevationResult_t;

	typedef struct//elevationMetrics->
	{
		float paramElevHigh; 		//?
		float paramElevLow;			//?
		float paramElevHigh_min;	//? enlever
		float paramElevHigh_max;	//? enlever
		
		float paramTimeFinite;		//? Input longueur de la période
		float paramTimeFinite_min;	//? enlever
		float paramTimeFinite_max;	//? enlever
		
		double elevTime_total;			 
		double elevTime_finite;
		double elevTime_finite_resetfeedback;
		int elevTime_finite_index;
		int elevTime_finite_lastElevState;
				
		int nbElev_total;
		int nbElev_finite;	
		int nbElev_finite_resetfeedback;
		int nvElev_finite_lastelevState;
					
		int elevCurrentState;
					
		int finiteData_maxsize;
		float elevTime_finite_data[500][2];
		float nbElev_finite_data[500][3];
		
		double resetValueFeedback_timing;
	} elevationMetrics_t;


	
    float TeamAT_IMUFusion_getElevationAccelero_atan(float accx,float accy,float accz);
	
	void acctorotmatrix(float rotmatrix[9], float acc[3]);
	
	void elevationAlgoData_init(elevationAlgoData_t *elevationAlgoData, float Ts);
	
    float TeamAT_IMUFusion_getElevationAccelGyro_oldfortest(float LSM_accelgyr_datarealtime[6], float Ts, int firstloop, float &uquat_last, float &acc_lowpass_last, float LSM_accelgyr_datarealtime_limited_last[6], double LSM_accelgyr_datarealtime_limited_filt[6], double LSM_accelgyr_datarealtime_limited_gyrx_filt_Y[5],double LSM_accelgyr_datarealtime_limited_gyry_filt_Y[5],double LSM_accelgyr_datarealtime_limited_gyrz_filt_Y[5],double LSM_accelgyr_datarealtime_limited_gyrx_filt_Z[5],double LSM_accelgyr_datarealtime_limited_gyry_filt_Z[5],double LSM_accelgyr_datarealtime_limited_gyrz_filt_Z[5],double &LSM_accelgyr_datarealtime_limited_gyrx_filt_u0,double &LSM_accelgyr_datarealtime_limited_gyry_filt_u0,double &LSM_accelgyr_datarealtime_limited_gyrz_filt_u0, float RotZaccum_c[9], float rotm_c_fus_xy[9], float Mat_Zero[9], float rotf[9], float rotf_raw[9], float &IMUFusion_Elevationvalue_raw);
    elevationResult_t TeamAT_IMUFusion_getElevationAccelGyro(elevationAlgoData_t *elevationAlgoData, float accelgyrData[6], float Ts, int flagfirst, int resetElevation);


	void ElevationMetrics_init(elevationMetrics_t *elevationMetrics);

    void TeamAT_IMUFusion_getElevationMetrics_oldfortest(float elevation, long &counter_elev, float Ts, float param_elevhigh, float param_elevlow, float param_temps_timingback, double &temps_elev_all,int &elev_state, int &nb_elev_total, int &temps_finite_index, int &temps_finite_lastelev, int maxdata75vectsize, float temps_finite_data75[500][2], double &temps_finite, int &elev_finite_lastelev, int &elev_finite_index,float elev_finite_data75[500][3], int reset, int resetfeedback, double &timingResetFeedback, double &temps_finite_feedback, int &elev_finite_feedback);
    void TeamAT_IMUFusion_getElevationMetrics(elevationMetrics_t *elevationMetrics, float elevation, long &counter_elev, float Ts, int reset, int resetfeedback);



#endif // TEAMAT_IMUFUSION_H
