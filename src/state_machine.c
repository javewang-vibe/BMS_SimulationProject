#include <stdio.h>
#include "state_machine.h"
#include "battery.h"

void BMS_CheckState(BmsData* bms)
{
    float minV,maxV;

    BMS_GetCellMinMax(bms,&minV,&maxV);

    float delta = maxV - minV;

    switch(bms->state)
    {
        case BMS_STANDBY:

            if(maxV > 4.15)
                bms->state = BMS_CHARGE;

            else if(minV < 3.2)
                bms->state = BMS_DISCHARGE;

            break;

        case BMS_CHARGE:

            if(maxV > 4.25 || bms->temperature > 60)
                bms->state = BMS_FAULT;

            break;

        case BMS_DISCHARGE:

            if(minV < 3.0 || bms->temperature > 60)
                bms->state = BMS_FAULT;

            break;

        case BMS_FAULT:

            if(bms->temperature < 50)
                bms->state = BMS_STANDBY;

            break;
    }

    if(delta > 0.2)
    {
        printf("[WARN] Cell imbalance detected: %.3fV\n",delta);
    }
}

const char* StateToStr(BmsState state)
{
    switch(state)
    {
        case BMS_STANDBY:
            return "STANDBY";

        case BMS_CHARGE:
            return "CHARGE";

        case BMS_DISCHARGE:
            return "DISCHARGE";

        case BMS_FAULT:
            return "FAULT";
    }

    return "UNKNOWN";
}