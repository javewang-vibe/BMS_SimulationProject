#ifndef EKF_H
#define EKF_H

typedef struct 
{
    float x;
    float P;
    float Q;
    float R;
} EKF;

void EKF_Init(EKF* ekf , float init_soc);
float EKF_Update(EKF* ekf , float current , float z);

#endif