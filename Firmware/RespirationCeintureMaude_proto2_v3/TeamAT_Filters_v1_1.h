#ifndef TEAMATFILTERS_H
#define TEAMATFILTERS_H



    // include
    #include <math.h>

    float TeamAT_Filters_LowPass_Classicfirst(float u,float &u_filt_m1, int flagfirst, float filt, float Ts);
	double TeamAT_Filters_LowPass_Classicfirst_double(double u,double &u_filt_m1, int flagfirst, double filt, double Ts);

    float TeamAT_Filters_HighPass_Classicfirst(float u,float &u_m1,float &u_filt_m1,int init_val,int flagfirst,float filt,float Ts);
	double TeamAT_Filters_HighPass_Classicfirst_double(double u,double &u_m1,double &u_filt_m1,int init_val,int flagfirst,double filt,double Ts);

    double TeamAT_Filters_ab_vsimple_o2_double(double b[3],double a[3],double u,double Y[3],double Z[3],double &u0,int firstloop,int init_val);
    double TeamAT_Filters_ab_vsimple_o4_double(double b[5],double a[5],double u,double Y[5],double Z[5],double &u0,int firstloop,int init_val);
	float TeamAT_Filters_ab_vsimple_o2(float b[3],float a[3],float u,float Y[3],float Z[3],float &u0,int firstloop,int init_val);
    float TeamAT_Filters_ab_vsimple_o4(float b[5],float a[5],float u,float Y[5],float Z[5],float &u0,int firstloop,int init_val);
    double TeamAT_Filters_ab_vsimple_o8(double b[9],double a[9],double u,double Y[9],double Z[9],double &u0,int firstloop,int init_val);
    double TeamAT_Filters_ab_vsimple_o16(double b[17],double a[17],double u,double Y[17],double Z[17],double &u0,int firstloop,int init_val);





    /* Algo qui calcule les coefficients de a et b comme Matlab. Mais demande des allocations de mémoire dynamique et complexe.
    void filter(int ord, double *a, double *b, int np, double *x, double *y);
    double *ComputeLP(int FilterOrder);
    double *ComputeHP(int FilterOrder);
    double *TrinomialMultiply(int FilterOrder, double *b, double *c);
    double *ComputeNumCoeffs(int FilterOrder,double Lcutoff, double Ucutoff, double *DenC);
    double *ComputeDenCoeffs(int FilterOrder, double Lcutoff, double Ucutoff);*/









#endif // TEAMATFILTERS_H

