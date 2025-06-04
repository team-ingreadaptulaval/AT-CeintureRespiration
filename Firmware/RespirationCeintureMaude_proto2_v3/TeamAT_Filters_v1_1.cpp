#include "TeamAT_Filters_v1_1.h"
//#include <QDebug>

float TeamAT_Filters_LowPass_Classicfirst(float u,float &u_filt_m1, int flagfirst, float filt, float Ts)
{//Created by ACampeau-Lecours, 2020-06-05

    float u_filt = 0;

    if (flagfirst==0)
    {
        u_filt = u;
        u_filt_m1 = u;
    }

    u_filt = u_filt_m1*(1-Ts/filt)+u*Ts/filt;
    u_filt_m1 = u_filt;

    return u_filt;
}

double TeamAT_Filters_LowPass_Classicfirst_double(double u,double &u_filt_m1, int flagfirst, double filt, double Ts)
{//Created by ACampeau-Lecours, 2020-06-05

    double u_filt = 0;

    if (flagfirst==0)
    {
        u_filt = u;
        u_filt_m1 = u;
    }

    u_filt = u_filt_m1*(1-Ts/filt)+u*Ts/filt;
    u_filt_m1 = u_filt;

    return u_filt;
}

float TeamAT_Filters_HighPass_Classicfirst(float u,float &u_m1,float &u_filt_m1,int init_val,int flagfirst,float filt,float Ts)
{//Created by ACampeau-Lecours, 2020-06-05

    float u_filt = 0;

    if (flagfirst==0)
    {
        u_m1 = u;
        if (init_val==0)//Ici il faut faire un choix dépendemment de l'application. Si on prend vfm1=0, le signal part à zéro.
        {
            u_filt_m1 = 0;
            u_filt = 0;
        }
        else//ou ici le signal part à la valeur initiale du signal.
        {
            u_filt_m1 = u;
            u_filt = u;
        }
    }

    u_filt = u_filt_m1*(1-Ts/filt) + u - u_m1;
    u_filt_m1 = u_filt;
    u_m1 = u;

    return u_filt;
}


double TeamAT_Filters_HighPass_Classicfirst_double(double u,double &u_m1,double &u_filt_m1,int init_val,int flagfirst,double filt,double Ts)
{//Created by ACampeau-Lecours, 2020-06-05

    double u_filt = 0;

    if (flagfirst==0)
    {
        u_m1 = u;
        if (init_val==0)//Ici il faut faire un choix dépendemment de l'application. Si on prend vfm1=0, le signal part à zéro.
        {
            u_filt_m1 = 0;
            u_filt = 0;
        }
        else//ou ici le signal part à la valeur initiale du signal.
        {
            u_filt_m1 = u;
            u_filt = u;
        }
    }

    u_filt = u_filt_m1*(1-Ts/filt) + u - u_m1;
    u_filt_m1 = u_filt;
    u_m1 = u;

    return u_filt;
}


double TeamAT_Filters_ab_vsimple_o2_double(double b[3],double a[3],double u,double Y[3],double Z[3],double &u0,int firstloop,int init_val)
{//Created by MBoyer, 2019. Adapted to C++ ACampeau-Lecours, 2020-06-05
    int order = 2;
    double Y_temp[3];
    double Z_temp[3];

    double u_filt = 0;

    if (firstloop == 0)
    {
        u0 = u;
    }

    if (init_val == 0)
    {
        Y[0] = u;
    }
    else if (init_val == 1)
    {
        Y[0]= u - u0;
    }
    else
    {
        Y[0]= u - u0;
    }


    Z[0] = (b[0]*Y[0] + b[1]*Y[1] + b[2]*Y[2]) - (a[1]*Z[1] + a[2]*Z[2]);


     for (int i=0;i<order+1;i++)
     {
         Y_temp[i] = Y[i];
         Z_temp[i] = Z[i];
     }
     for (int i=1;i<order+1;i++)
     {
         Y[i] = Y_temp[i-1];
         Z[i] = Z_temp[i-1];
     }


     //qDebug() << Y[0] << Y[1] << Y[2] << Z[0] << Z[1] << Z[2];

    if (init_val==0)
    {
        u_filt = Z[0];
    }
    else if (init_val==1)
    {
        u_filt = Z[0] + u0;
    }
    else
    {
        u_filt = Z[0];
    }

    return u_filt;

}

double TeamAT_Filters_ab_vsimple_o4_double(double b[5],double a[5],double u,double Y[5],double Z[5],double &u0,int firstloop,int init_val)
{//Created by MBoyer, 2019. Adapted to C++ ACampeau-Lecours, 2020-06-05
    int order = 4;
    double Y_temp[5];
    double Z_temp[5];
    double u_filt = 0;

    if (firstloop == 0)
    {
        u0 = u;
    }
    if (init_val == 0)
    {
        Y[0] = u;
    }
    else if (init_val == 1)
    {
        Y[0]= u - u0;
    }
    else
    {
        Y[0]= u - u0;
    }


    Z[0] = (b[0]*Y[0] + b[1]*Y[1] + b[2]*Y[2] + b[3]*Y[3] + b[4]*Y[4]) - (a[1]*Z[1] + a[2]*Z[2] + a[3]*Z[3] + a[4]*Z[4]);


    for (int i=0;i<order+1;i++)
    {
        Y_temp[i] = Y[i];
        Z_temp[i] = Z[i];
    }
    for (int i=1;i<order+1;i++)
    {
        Y[i] = Y_temp[i-1];
        Z[i] = Z_temp[i-1];
    }


    if (init_val==0)
    {
        u_filt = Z[0];
    }
    else if (init_val==1)
    {
        u_filt = Z[0] + u0;
    }
    else
    {
        u_filt = Z[0];
    }

    return u_filt;

}



float TeamAT_Filters_ab_vsimple_o2(float b[3],float a[3],float u,float Y[3],float Z[3],float &u0,int firstloop,int init_val)
{//Created by MBoyer, 2019. Adapted to C++ ACampeau-Lecours, 2020-06-05
    int order = 2;
    float Y_temp[3];
    float Z_temp[3];

    float u_filt = 0;

    if (firstloop == 0)
    {
        u0 = u;
    }

    if (init_val == 0)
    {
        Y[0] = u;
    }
    else if (init_val == 1)
    {
        Y[0]= u - u0;
    }
    else
    {
        Y[0]= u - u0;
    }


    Z[0] = (b[0]*Y[0] + b[1]*Y[1] + b[2]*Y[2]) - (a[1]*Z[1] + a[2]*Z[2]);


     for (int i=0;i<order+1;i++)
     {
         Y_temp[i] = Y[i];
         Z_temp[i] = Z[i];
     }
     for (int i=1;i<order+1;i++)
     {
         Y[i] = Y_temp[i-1];
         Z[i] = Z_temp[i-1];
     }


     //qDebug() << Y[0] << Y[1] << Y[2] << Z[0] << Z[1] << Z[2];

    if (init_val==0)
    {
        u_filt = Z[0];
    }
    else if (init_val==1)
    {
        u_filt = Z[0] + u0;
    }
    else
    {
        u_filt = Z[0];
    }

    return u_filt;

}

float TeamAT_Filters_ab_vsimple_o4(float b[5],float a[5],float u,float Y[5],float Z[5],float &u0,int firstloop,int init_val)
{//Created by MBoyer, 2019. Adapted to C++ ACampeau-Lecours, 2020-06-05
    int order = 4;
    float Y_temp[5];
    float Z_temp[5];
    float u_filt = 0;

    if (firstloop == 0)
    {
        u0 = u;
    }
    if (init_val == 0)
    {
        Y[0] = u;
    }
    else if (init_val == 1)
    {
        Y[0]= u - u0;
    }
    else
    {
        Y[0]= u - u0;
    }


    Z[0] = (b[0]*Y[0] + b[1]*Y[1] + b[2]*Y[2] + b[3]*Y[3] + b[4]*Y[4]) - (a[1]*Z[1] + a[2]*Z[2] + a[3]*Z[3] + a[4]*Z[4]);


    for (int i=0;i<order+1;i++)
    {
        Y_temp[i] = Y[i];
        Z_temp[i] = Z[i];
    }
    for (int i=1;i<order+1;i++)
    {
        Y[i] = Y_temp[i-1];
        Z[i] = Z_temp[i-1];
    }


    if (init_val==0)
    {
        u_filt = Z[0];
    }
    else if (init_val==1)
    {
        u_filt = Z[0] + u0;
    }
    else
    {
        u_filt = Z[0];
    }

    return u_filt;

}




double TeamAT_Filters_ab_vsimple_o8(double b[9],double a[9],double u,double Y[9],double Z[9],double &u0,int firstloop,int init_val)
{//Created by MBoyer, 2019. Adapted to C++ ACampeau-Lecours, 2020-06-05
    int order = 8;
    double Y_temp[9];
    double Z_temp[9];
    double u_filt = 0;

    if (firstloop == 0)
    {
        u0 = u;
    }
    if (init_val == 0)
    {
        Y[0] = u;
    }
    else if (init_val == 1)
    {
        Y[0]= u - u0;
    }
    else
    {
        Y[0]= u - u0;
    }


    Z[0] = (b[0]*Y[0] + b[1]*Y[1] + b[2]*Y[2] + b[3]*Y[3] + b[4]*Y[4] + b[5]*Y[5] + b[6]*Y[6] + b[7]*Y[7] + b[8]*Y[8]) - (a[1]*Z[1] + a[2]*Z[2] + a[3]*Z[3] + a[4]*Z[4] + a[5]*Z[5] + a[6]*Z[6] + a[7]*Z[7] + a[8]*Z[8]);


    for (int i=0;i<order+1;i++)
    {
        Y_temp[i] = Y[i];
        Z_temp[i] = Z[i];
    }
    for (int i=1;i<order+1;i++)
    {
        Y[i] = Y_temp[i-1];
        Z[i] = Z_temp[i-1];
    }


    if (init_val==0)
    {
        u_filt = Z[0];
    }
    else if (init_val==1)
    {
        u_filt = Z[0] + u0;
    }
    else
    {
        u_filt = Z[0];
    }

    return u_filt;

}

double TeamAT_Filters_ab_vsimple_o16(double b[17],double a[17],double u,double Y[17],double Z[17],double &u0,int firstloop,int init_val)
{//Created by MBoyer, 2019. Adapted to C++ ACampeau-Lecours, 2020-06-05
    int order = 16;
    double Y_temp[17];
    double Z_temp[17];
    double u_filt = 0;


    if (firstloop == 0)
    {
        u0 = u;
    }
    if (init_val == 0)
    {
        Y[0] = u;
    }
    else if (init_val == 1)
    {
        Y[0]= u - u0;
    }
    else
    {
        Y[0]= u - u0;
    }


    Z[0] = (b[0]*Y[0] + b[1]*Y[1] + b[2]*Y[2] + b[3]*Y[3] + b[4]*Y[4] + b[5]*Y[5] + b[6]*Y[6] + b[7]*Y[7] + b[8]*Y[8] + b[9]*Y[9] + b[10]*Y[10] + b[11]*Y[11] + b[12]*Y[12] + b[13]*Y[13] + b[14]*Y[14] + b[15]*Y[15] + b[16]*Y[16]) - (a[1]*Z[1] + a[2]*Z[2] + a[3]*Z[3] + a[4]*Z[4] + a[5]*Z[5] + a[6]*Z[6] + a[7]*Z[7] + a[8]*Z[8] + a[9]*Z[9] + a[10]*Z[10] + a[11]*Z[11] + a[12]*Z[12] + a[13]*Z[13] + a[14]*Z[14] + a[15]*Z[15] + a[16]*Z[16]);


    for (int i=0;i<order+1;i++)
    {
        Y_temp[i] = Y[i];
        Z_temp[i] = Z[i];
    }
    for (int i=1;i<order+1;i++)
    {
        Y[i] = Y_temp[i-1];
        Z[i] = Z_temp[i-1];
    }


    if (init_val==0)
    {
        u_filt = Z[0];
    }
    else if (init_val==1)
    {
        u_filt = Z[0] + u0;
    }
    else
    {
        u_filt = Z[0];
    }

    return u_filt;

}



















/*void filter(int ord, double *a, double *b, int np, double *x, double *y)
{
    int i,j;
    y[0]=b[0] * x[0];
    for (i=1;i<ord+1;i++)
    {
     y[i]=0.0;
     for (j=0;j<i+1;j++)
      y[i]=y[i]+b[j]*x[i-j];
     for (j=0;j<i;j++)
      y[i]=y[i]-a[j+1]*y[i-j-1];
    }
    for (i=ord+1;i<np+1;i++)
    {
     y[i]=0.0;
     for (j=0;j<ord+1;j++)
      y[i]=y[i]+b[j]*x[i-j];
     for (j=0;j<ord;j++)
      y[i]=y[i]-a[j+1]*y[i-j-1];
    }
}

double *ComputeLP(int FilterOrder)
{
    double *NumCoeffs;
    int m;
    int i;

    NumCoeffs = (double *)calloc(FilterOrder+1, sizeof(double));
    if(NumCoeffs == NULL) return(NULL);

    NumCoeffs[0] = 1;
    NumCoeffs[1] = FilterOrder;
    m = FilterOrder/2;
    for(i=2; i <= m; ++i)
    {
     NumCoeffs[i] =(double) (FilterOrder-i+1)*NumCoeffs[i-1]/i;
     NumCoeffs[FilterOrder-i]= NumCoeffs[i];
    }
    NumCoeffs[FilterOrder-1] = FilterOrder;
    NumCoeffs[FilterOrder] = 1;

    return NumCoeffs;
}

double *ComputeHP(int FilterOrder)
{
    double *NumCoeffs;
    int i;

    NumCoeffs = ComputeLP(FilterOrder);
    if(NumCoeffs == NULL) return(NULL);

    for(i = 0; i <= FilterOrder; ++i)
     if(i % 2) NumCoeffs[i] = -NumCoeffs[i];

    return NumCoeffs;
}

double *TrinomialMultiply(int FilterOrder, double *b, double *c)
{
    int i, j;
    double *RetVal;

    RetVal = (double *)calloc(4 * FilterOrder, sizeof(double));
    if(RetVal == NULL) return(NULL);

    RetVal[2] = c[0];
    RetVal[3] = c[1];
    RetVal[0] = b[0];
    RetVal[1] = b[1];

    for(i = 1; i < FilterOrder; ++i)
    {
     RetVal[2*(2*i+1)] += c[2*i] * RetVal[2*(2*i-1)] - c[2*i+1] * RetVal[2*(2*i-1)+1];
     RetVal[2*(2*i+1)+1] += c[2*i] * RetVal[2*(2*i-1)+1] + c[2*i+1] * RetVal[2*(2*i-1)];

     for(j = 2*i; j > 1; --j)
     {
      RetVal[2*j] += b[2*i] * RetVal[2*(j-1)] - b[2*i+1] * RetVal[2*(j-1)+1] +
       c[2*i] * RetVal[2*(j-2)] - c[2*i+1] * RetVal[2*(j-2)+1];
      RetVal[2*j+1] += b[2*i] * RetVal[2*(j-1)+1] + b[2*i+1] * RetVal[2*(j-1)] +
       c[2*i] * RetVal[2*(j-2)+1] + c[2*i+1] * RetVal[2*(j-2)];
     }

     RetVal[2] += b[2*i] * RetVal[0] - b[2*i+1] * RetVal[1] + c[2*i];
     RetVal[3] += b[2*i] * RetVal[1] + b[2*i+1] * RetVal[0] + c[2*i+1];
     RetVal[0] += b[2*i];
     RetVal[1] += b[2*i+1];
    }
    return RetVal;
}

double *ComputeNumCoeffs(int FilterOrder,double Lcutoff, double Ucutoff, double *DenC)
{
    double *TCoeffs;
    double *NumCoeffs;
    std::complex<double> *NormalizedKernel;
    double Numbers[11]={0,1,2,3,4,5,6,7,8,9,10};
    int i;

    NumCoeffs = (double *)calloc(2*FilterOrder+1, sizeof(double));
    if(NumCoeffs == NULL) return(NULL);

    NormalizedKernel = (std::complex<double> *)calloc(2*FilterOrder+1, sizeof(std::complex<double>));
    if(NormalizedKernel == NULL) return(NULL);

    TCoeffs = ComputeHP(FilterOrder);
    if(TCoeffs == NULL) return(NULL);

    for(i = 0; i < FilterOrder; ++i)
    {
     NumCoeffs[2*i] = TCoeffs[i];
     NumCoeffs[2*i+1] = 0.0;
    }
    NumCoeffs[2*FilterOrder] = TCoeffs[FilterOrder];
    double cp[2];
    //double Bw;
    double Wn;
    cp[0] = 2*2.0*tan(PI * Lcutoff/ 2.0);
    cp[1] = 2*2.0*tan(PI * Ucutoff/2.0);

    //Bw = cp[1] - cp[0];
    //center frequency
    Wn = sqrt(cp[0]*cp[1]);
    Wn = 2*atan2(Wn,4);
    //double kern;
    const std::complex<double> result = std::complex<double>(-1,0);

    for(int k = 0; k<2*FilterOrder+1; k++)
    {
     NormalizedKernel[k] = std::exp(-sqrt(result)*Wn*Numbers[k]);
    }
    double b=0;
    double den=0;
    for(int d = 0; d<2*FilterOrder+1; d++)
    {
     b+=real(NormalizedKernel[d]*NumCoeffs[d]);
     den+=real(NormalizedKernel[d]*DenC[d]);
    }
    for(int c = 0; c<2*FilterOrder+1; c++)
    {
     NumCoeffs[c]=(NumCoeffs[c]*den)/b;
    }

    free(TCoeffs);
    return NumCoeffs;
}

double *ComputeDenCoeffs(int FilterOrder, double Lcutoff, double Ucutoff)
{
    int k;   // loop variables
    double theta;  // PI * (Ucutoff - Lcutoff)/2.0
    double cp;  // cosine of phi
    double st;  // sine of theta
    double ct;  // cosine of theta
    double s2t;  // sine of 2*theta
    double c2t;  // cosine 0f 2*theta
    double *RCoeffs;  // z^-2 coefficients
    double *TCoeffs;  // z^-1 coefficients
    double *DenomCoeffs;  // dk coefficients
    double PoleAngle;  // pole angle
    double SinPoleAngle;  // sine of pole angle
    double CosPoleAngle;  // cosine of pole angle
    double a;   // workspace variables

    cp = cos(PI * (Ucutoff + Lcutoff)/2.0);
    theta = PI * (Ucutoff - Lcutoff)/2.0;
    st = sin(theta);
    ct = cos(theta);
    s2t = 2.0*st*ct;  // sine of 2*theta
    c2t = 2.0*ct*ct - 1.0; // cosine of 2*theta

    RCoeffs = (double *)calloc(2 * FilterOrder, sizeof(double));
    TCoeffs = (double *)calloc(2 * FilterOrder, sizeof(double));

    for(k = 0; k < FilterOrder; ++k)
    {
     PoleAngle = PI * (double)(2*k+1)/(double)(2*FilterOrder);
     SinPoleAngle = sin(PoleAngle);
     CosPoleAngle = cos(PoleAngle);
     a = 1.0 + s2t*SinPoleAngle;
     RCoeffs[2*k] = c2t/a;
     RCoeffs[2*k+1] = s2t*CosPoleAngle/a;
     TCoeffs[2*k] = -2.0*cp*(ct+st*SinPoleAngle)/a;
     TCoeffs[2*k+1] = -2.0*cp*st*CosPoleAngle/a;
    }

    DenomCoeffs = TrinomialMultiply(FilterOrder, TCoeffs, RCoeffs);
    free(TCoeffs);
    free(RCoeffs);

    DenomCoeffs[1] = DenomCoeffs[0];
    DenomCoeffs[0] = 1.0;
    for(k = 3; k <= 2*FilterOrder; ++k)
     DenomCoeffs[k] = DenomCoeffs[2*k-2];


    return DenomCoeffs;
}
main:
    double FrequencyBands[2] = {fcuthigh/(fs/2.0),fcutlow/(fs/2.0)};//these values are as a ratio of f/fs, where fs is sampling rate, and f is cutoff frequency

    double *DenC = 0;
    double *NumC = 0;
    DenC = ComputeDenCoeffs(FiltOrd, FrequencyBands[0], FrequencyBands[1]);
    //for(int k = 0; k<2*FiltOrd+1; k++)
    //{
    // printf("DenC is: %lf\n", DenC[k]);
    //}
    //printf("\n");

    NumC = ComputeNumCoeffs(FiltOrd,FrequencyBands[0],FrequencyBands[1],DenC);
    //for(int k = 0; k<2*FiltOrd+1; k++)
    //{
    // printf("NumC is: %lf\n", NumC[k]);
    //}


    double y[5];
    double x[5]={1,2,3,4,5};
    filter(5, DenC, NumC, 5, x, y);
*/



