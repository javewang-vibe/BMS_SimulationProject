#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

#include "battery.h"
#include "state_machine.h"
#include "control.h"
#include "logger.h"
#include "soc.h"
#include "fault.h"

int main()
{
    BmsData bms;

    srand((unsigned)time(NULL));

    BMS_Init(&bms);
    CSV_Init();
    static int time = 0;

    printf("==== BMS V1.0 START ====\n");

    while(1)
    {
        time++;

        BMS_ReadSensor(&bms);

        BMS_UpdateSOC(&bms);

        BMS_CheckState(&bms);

        BMS_Control(&bms);

        BMS_Report(&bms);

        CSV_Write
        (
            time,
            bms.soc,
            BMS_GetPackVoltage(&bms),
            bms.current,
            bms.temperature,
            StateToStr(bms.state),
            FaultToStr(bms.fault_latch)
        );

        printf("---------------------\n");

        Sleep(1000);
    }
    CSV_Close();
    return 0;
}
