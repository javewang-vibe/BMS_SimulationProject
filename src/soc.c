#include "soc.h"
#include "ocv.h"
#include "battery.h"
#include "ekf.h"
#include <stdio.h>

static EKF ekf;

void BMS_InitSOC(BmsData* bms)
{
    EKF_Init(&ekf,bms->soc);
}

void BMS_UpdateSOC(BmsData* bms)
{
    float pack_voltage = BMS_GetPackVoltage(bms);
    float avg_cell_voltage = pack_voltage / CELL_NUM;
    float z = OCV_LookupSOC(avg_cell_voltage);
    
    bms->soc = EKF_Update(&ekf,bms->current,z);
    printf("[EKF] K=%.3f | Pred=%.4f%% | OCV=%.4f%% | Fused=%.4f%%\n",ekf.P / (ekf.P + ekf.R),ekf.x,z,bms->soc);
}
