#include <stdio.h>
#include "state_machine.h"
#include "battery.h"
#include "fault.h"

void BMS_CheckState(BmsData* bms)
{
    BMS_DiagnoseFault(bms);

    if(bms->fault != BMS_FAULT_NONE)
    {
        bms->state = BMS_FAULT;
        printf("[FAULT] %s\n", FaultToStr(bms->fault));
        return;
    }

    switch(bms->state)
    {
        case BMS_STANDBY:

            if(bms->current < -0.05f)
                bms->state = BMS_CHARGE;

            else if(bms->current > 0.05f)
                bms->state = BMS_DISCHARGE;

            break;

        case BMS_CHARGE:

            if(bms->current >= -0.05f)
                bms->state = BMS_STANDBY;

            break;

        case BMS_DISCHARGE:

            if(bms->current <= 0.05f)
                bms->state = BMS_STANDBY;

            break;

        case BMS_FAULT:

            bms->state = BMS_STANDBY;

            break;
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
