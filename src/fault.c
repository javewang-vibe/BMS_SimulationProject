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
