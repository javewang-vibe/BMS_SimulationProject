#include "soc.h"
#include "ocv.h"
#include "battery.h"
#include <stdio.h>

#define CAPACITY 100.0f
#define CURRENT_THRESHOLD 0.05f

void BMS_UpdateSOC(BmsData* bms)
{
    float dt = 1.0f / 3600.0f;
    float delta_soc = (bms->current / CAPACITY) * dt * 100.0f;
    bms->soc -= delta_soc;

    if(bms->current > -CURRENT_THRESHOLD && bms->current < CURRENT_THRESHOLD)
    {
        float pack_voltage = BMS_GetPackVoltage(bms);
        float avg_cell_voltage = pack_voltage / CELL_NUM;
        float soc_ocv = OCV_LookupSOC(avg_cell_voltage);
        float soc_before = bms->soc;
        bms->soc = 0.7f * bms->soc + 0.3f * soc_ocv;
        printf("[SOC] Coulomb=%.2f%% OCV=%.2f%% Fused=%.2f%%\n",soc_before,soc_ocv,bms->soc);
    }

    if(bms->soc > 100.0f)   bms->soc = 100.0f;
    if(bms->soc < 0.0f)     bms->soc = 0.0f;
}
