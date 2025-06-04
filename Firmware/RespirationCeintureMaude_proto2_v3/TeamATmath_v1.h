#ifndef TEAMATMATH_H_
#define TEAMATMATH_H_

#include "math.h"

#define PI			3.1415926536


float Norm3(float vector[3]);
float Norm4(float vector[4]);
void Normalize3(float in[3], float out[3]);
void Normalize4(float in[4], float out[4]);

void VectCopy(float* v1, float* v2, int size);
void VectAdd(float* v1, float* v2, float* result, int size);
void VectSubs(float* v1, float* v2, float* result, int size);
void VectMultScalar(float* v, float s, float* result, int size);

float DotProduct(float* vectorA, float* vectorB, int size);
void CrossProduct(float vectorA[3], float vectorB[3], float result[3]);
void MatMultiply(float* T1, float* T2, float* result, int m, int n, int p);
void MatMultiply_3x3(float* T1, float* T2, float* result);

void TransposeMatrix(float *matrix, float *transposed,int m, int n);
void TransposeMatrix_3x3(float matrix[9], float transposed[9]);
void Inv3X3(const float x[9], float Fv[9]);
void Inv6X6(const float * matrice, float * matriceOut);

void MatRotationToQuat(float q[4], float R[9]);
void quatToMatRotation(float R[9], float q[4]);
void MatToEuler_TeamAT(float R[9], float angles[3], int norme, int branch);
float MatToEulerTTzelev(float R[9]);

#endif