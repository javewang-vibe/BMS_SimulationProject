#include <stdio.h>
#include <stdlib.h>
#include "battery.h"
#include "ocv.h"
#include "rc_model.h"
#include "config.h"

void BMS_Init(BmsData* bms)
{
    for(int i = 0; i < CELL_NUM; i++)
    {
        bms->est.soc_cell[i] = 50.0f;
        bms->raw.cellvoltage[i] = 3.78f;
        RcModel_Init(&bms->rc[i]);
    }

    bms->raw.temperature = 25.0f;
    bms->raw.current = 0.0f;
    bms->est.soc = 50.0f;
    bms->safety.state = BMS_STANDBY;
    bms->safety.fault = BMS_FAULT_NONE;
    bms->safety.fault_latch = BMS_FAULT_NONE;
    bms->safety.fault_reset_request = 0;
    bms->safety.fault_timer = 0;
}

void BMS_ReadSensor(BmsData* bms)
{
    bms->raw.current = 0.3f + ((rand() % 21) - 10) * 0.005f;
    bms->raw.temperature += 0.02f + ((rand() % 11) - 5) * 0.005f;
    if(bms->raw.temperature > 70.0f) bms->raw.temperature = 70.0f;
    if(bms->raw.temperature < -10.0f) bms->raw.temperature = -10.0f;
    float dt = 1.0f / 3600.0f;
    for(int i = 0; i < CELL_NUM; i++)
    {
        float cell_current = bms->raw.current * (1.0f + (i - 2) * 0.01f);
        bms->est.soc_cell[i] -= (cell_current / BATTERY_CAPACITY_AH) * dt * 100.0f;
        if(bms->est.soc_cell[i] > 100.0f) bms->est.soc_cell[i] = 100.0f;
        if(bms->est.soc_cell[i] < 0.0f) bms->est.soc_cell[i] = 0.0f;
        float ocv = OCV_LookupSOC_Inverse(bms->est.soc_cell[i]);
        bms->raw.cellvoltage[i] = RcModel_Update(&bms->rc[i],cell_current,ocv);
    }
}

float BMS_GetPackVoltage(BmsData* bms)
{
    float sum = 0;

    for(int i = 0; i < CELL_NUM; i++)
    {
        sum += bms->raw.cellvoltage[i];
    }

    return sum;
}

void BMS_GetCellMinMax(BmsData* bms,float* minV,float* maxV)
{
    *minV = bms->raw.cellvoltage[0];
    *maxV = bms->raw.cellvoltage[0];

    for(int i = 0; i < CELL_NUM; i++)
    {
        if(bms->raw.cellvoltage[i] < *minV)
            *minV = bms->raw.cellvoltage[i];

        if(bms->raw.cellvoltage[i] > *maxV)
            *maxV = bms->raw.cellvoltage[i];
    }
}
float BMS_GetDeltaVoltage(BmsData* bms)
{
    float minV,maxV;

    BMS_GetCellMinMax(bms,&minV,&maxV);

    return maxV - minV;
}
