#include "ekf.h"
#include "config.h"

void EKF_Init(EKF* ekf , float init_soc)
{
    ekf->x = init_soc;
    ekf->P = 1.0f;     
    ekf->Q = 0.001f;   //过程噪声，经验值
    ekf->R = 0.01f;    //测量噪声，经验值
}

float EKF_Update(EKF* ekf , float current , float z)
{
    float dt = 1.0f / 3600.0f;
    float x_pred = ekf->x - (current / BATTERY_CAPACITY_AH) * dt * 100.0f;
    float P_pred = ekf->P + ekf->Q;

    float K = P_pred / (P_pred + ekf->R);
    ekf->x = x_pred + K * (z - x_pred);
    ekf->P = (1.0f - K) * P_pred;

    if(ekf->x > 100.0f) ekf->x = 100.0f;
    if(ekf->x < 0.0f) ekf->x = 0.0f;

    return ekf->x;
}