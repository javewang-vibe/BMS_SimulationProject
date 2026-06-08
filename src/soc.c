#include "soc.h"
#include "ocv.h"
#include "battery.h"
#include <stdio.h>
#include "ekf.h"

static EKF ekf;

void BMS_InitSOC(BmsData* bms)
{
    EKF_Init(&ekf,bms->est.soc);
}

void BMS_UpdateSOC(BmsData* bms)
{
    float pack_voltage = BMS_GetPackVoltage(bms);
    float avg_cell_voltage = pack_voltage / CELL_NUM;
    float avg_v_compensated = avg_cell_voltage + bms->raw.current * 0.05f;
    float z = OCV_LookupSOC(avg_v_compensated);

    bms->est.soc = EKF_Update(&ekf,bms->raw.current,z);
    printf("[EKF] K=%.3f | Pred=%.4f%% | OCV=%.4f%% | Fused=%.4f%%\n",ekf.P/(ekf.P + ekf.R),ekf.x,z,bms->est.soc);
}
