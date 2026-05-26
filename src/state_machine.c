#include <stdio.h>
#include "state_machine.h"
#include "battery.h"
#include "fault.h"

void BMS_CheckState(BmsData* bms)
{
    BMS_DiagnoseFault(bms);

    if(bms->fault != BMS_FAULT_NONE)
    {
        if(bms->fault_latch == BMS_FAULT_NONE)
        {
            bms->fault_latch = bms->fault;
            bms->fault_timer = 0;
        } 
        bms->state = BMS_FAULT;
        printf("[FAULT] %s\n", FaultToStr(bms->fault));
        return;
    }
    if(bms->state == BMS_FAULT)
    {
        BMS_TryAutoReset(bms);
        if(bms->fault_reset_request ==1)
        {
            bms->fault_latch = BMS_FAULT_NONE;
            bms->fault_reset_request = 0;
            bms->fault_timer = 0;
            bms->state = BMS_STANDBY;
            printf("[RECOVERY] System reset, back to STANDBY\n");
        }
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
        default:
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
