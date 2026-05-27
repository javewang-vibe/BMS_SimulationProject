#include <stdio.h>
#include <stdlib.h>
#include "battery.h"
#include "ocv.h"

void BMS_Init(BmsData* bms)
{
    for(int i = 0; i < CELL_NUM; i++)
    {
        bms->soc_cell[i] = 50.0f;
        bms->cellvoltage[i] = 3.78f;
        RcModel_Init(&bms->rc[i]);
    }

    bms->temperature = 25.0f;
    bms->current = 0.0f;
    bms->soc = 50.0f;
    bms->state = BMS_STANDBY;
    bms->fault = BMS_FAULT_NONE;
    bms->fault_latch = BMS_FAULT_NONE;
    bms->fault_reset_request = 0;
    bms->fault_timer = 0;
}

void BMS_ReadSensor(BmsData* bms)
{
    bms->current = 0.5f + ((rand() % 21) - 10) * 0.005f;
    bms->temperature += 0.02f + ((rand() % 11) - 5) * 0.005f;
    if(bms->temperature > 70.0f) bms->temperature = 70.0f;
    if(bms->temperature < -10.0f) bms->temperature = -10.0f;
    float dt = 1.0f / 3600.0f;
    for(int i = 0; i < CELL_NUM; i++)
    {
        float cell_current = bms->current * (1.0f + (i - 2) * 0.01f);
        bms->soc_cell[i] -= (cell_current / 100.0f) * dt * 100.0f;
        if(bms->soc_cell[i] > 100.0f) bms->soc_cell[i] = 100.0f;
        if(bms->soc_cell[i] < 0.0f) bms->soc_cell[i] = 0.0f;
        float ocv = OCV_LookupSOC_Inverse(bms->soc_cell[i]);
        bms->cellvoltage[i] = RcModel_Update(&bms->rc[i],cell_current,ocv);
    }
}

float BMS_GetPackVoltage(BmsData* bms)
{
    float sum = 0;

    for(int i = 0; i < CELL_NUM; i++)
    {
        sum += bms->cellvoltage[i];
    }

    return sum;
}

void BMS_GetCellMinMax(BmsData* bms,float* minV,float* maxV)
{
    *minV = bms->cellvoltage[0];
    *maxV = bms->cellvoltage[0];

    for(int i = 0; i < CELL_NUM; i++)
    {
        if(bms->cellvoltage[i] < *minV)
            *minV = bms->cellvoltage[i];

        if(bms->cellvoltage[i] > *maxV)
            *maxV = bms->cellvoltage[i];
    }
}

float BMS_GetDeltaVoltage(BmsData* bms)
{
    float minV,maxV;

    BMS_GetCellMinMax(bms,&minV,&maxV);

    return maxV - minV;
}
