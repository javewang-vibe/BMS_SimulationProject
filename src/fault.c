#include <stdio.h>
#include "fault.h"
#include "battery.h"

#define CELL_OVERVOLTAGE_LIMIT 4.20f
#define CELL_UNDERVOLTAGE_LIMIT 2.80f
#define OVERTEMPERATURE_LIMIT 60.0f
#define UNDERTEMPERATURE_LIMIT -5.0f
#define CHARGE_OVERCURRENT_LIMIT -0.80f
#define DISCHARGE_OVERCURRENT_LIMIT 0.80f
#define CELL_IMBALANCE_LIMIT 0.20f

void BMS_DiagnoseFault(BmsData* bms)
{
    float minV;
    float maxV;
    float deltaV;

    BMS_GetCellMinMax(bms, &minV, &maxV);
    deltaV = maxV - minV;

    bms->fault = BMS_FAULT_NONE;

    if(maxV >= CELL_OVERVOLTAGE_LIMIT)
    {
        bms->fault = BMS_FAULT_CELL_OVERVOLTAGE;
    }
    else if(minV <= CELL_UNDERVOLTAGE_LIMIT)
    {
        bms->fault = BMS_FAULT_CELL_UNDERVOLTAGE;
    }
    else if(bms->temperature >= OVERTEMPERATURE_LIMIT)
    {
        bms->fault = BMS_FAULT_OVERTEMPERATURE;
    }
    else if(bms->temperature <= UNDERTEMPERATURE_LIMIT)
    {
        bms->fault = BMS_FAULT_UNDERTEMPERATURE;
    }
    else if(bms->current <= CHARGE_OVERCURRENT_LIMIT)
    {
        bms->fault = BMS_FAULT_CHARGE_OVERCURRENT;
    }
    else if(bms->current >= DISCHARGE_OVERCURRENT_LIMIT)
    {
        bms->fault = BMS_FAULT_DISCHARGE_OVERCURRENT;
    }
    else if(deltaV >= CELL_IMBALANCE_LIMIT)
    {
        bms->fault = BMS_FAULT_CELL_IMBALANCE;
    }
}


void BMS_TryAutoReset(BmsData* bms)
{
    float minV,maxV;
    BMS_GetCellMinMax(bms,&minV,&maxV);
    bms->fault_timer++;
    switch(bms->fault_latch)
    {
        case BMS_FAULT_OVERTEMPERATURE:
            if(bms->temperature < 55.0f && bms->fault_timer >=3)
            {
                bms->fault_reset_request = 1;
                printf("[RESET COND] Temperature recovered: %.1fC\n",bms->temperature);
            }
            else
            {
                printf("[RESET WAIT] Temp=%.1fC, need<55.0C, timer=%ds\n",bms->temperature,bms->fault_timer);
            }
            break;
        case BMS_FAULT_UNDERTEMPERATURE:
            if(bms->temperature > 0.0f && bms->fault_timer >=3)
            {
                bms->fault_reset_request = 1;
                printf("[RESET COND] Temperature recovered: %.1fC\n",bms->temperature);
            }
            else
            {
                printf("[RESET WAIT] Temp=%.1fC, need>0.0C, timer=%ds\n",bms->temperature,bms->fault_timer);
            }
            break;
        case BMS_FAULT_CELL_OVERVOLTAGE:
            if(maxV < 4.15f && bms->fault_timer >=3)
            {
                bms->fault_reset_request = 1;
                printf("[RESET COND] Overvoltage recovered: maxV=%.3fV\n",maxV);
            }
            else
            {
                printf("[RESET WAIT] maxV=%.3fV, need<4.15V, timer=%ds\n",maxV,bms->fault_timer);
            }
            break;
        case BMS_FAULT_CELL_UNDERVOLTAGE:
            if(minV > 2.85f && bms->fault_timer >=3)
            {
                bms->fault_reset_request = 1;
                printf("[RESET COND] Undercoltage recovered: minV=%.3fV\n",minV);
            }
            else
            {
                printf("[RESET WAIT] minV=%.3fV, need>2.85V, timer=%ds\n",minV,bms->fault_timer);
            }
            break;
        case BMS_FAULT_CHARGE_OVERCURRENT:
        case BMS_FAULT_DISCHARGE_OVERCURRENT:
            if(bms->current > -0.70f && bms->current <0.70f && bms->fault_timer >= 5)
            {
                bms->fault_reset_request = 1;
                printf("[RESET COND] Current recovered: %.2fA\n",bms->current);
            }
            else
            {
                printf("[RESET WAIT] I=%.2fA, need in(-0.7,0.7), timer=%ds\n",bms->current,bms->fault_timer);
            }
            break;
        case BMS_FAULT_CELL_IMBALANCE:
            if((maxV-minV) < 0.15f && bms->fault_timer >= 10)
            {
                bms->fault_reset_request = 1;
                printf("[RESET COND] Imbalance recovered: delta=%.3fV\n",maxV-minV);
            }
            else
            {
                printf("[RESET WAIT] delta=%.3fV, need <0.15V, timer=%ds\n",maxV-minV,bms->fault_timer);
            }
            break;
        default:
            break;
    }
}



const char* FaultToStr(BmsFault fault)
{
    switch(fault)
    {
        case BMS_FAULT_NONE:
            return "NONE";

        case BMS_FAULT_CELL_OVERVOLTAGE:
            return "CELL_OVERVOLTAGE";

        case BMS_FAULT_CELL_UNDERVOLTAGE:
            return "CELL_UNDERVOLTAGE";

        case BMS_FAULT_OVERTEMPERATURE:
            return "OVERTEMPERATURE";

        case BMS_FAULT_UNDERTEMPERATURE:
            return "UNDERTEMPERATURE";

        case BMS_FAULT_CHARGE_OVERCURRENT:
            return "CHARGE_OVERCURRENT";

        case BMS_FAULT_DISCHARGE_OVERCURRENT:
            return "DISCHARGE_OVERCURRENT";

        case BMS_FAULT_CELL_IMBALANCE:
            return "CELL_IMBALANCE";
    }

    return "UNKNOWN";
}
