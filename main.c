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
    BMS_InitSOC(&bms);
    CSV_Init();
    static int t = 0;

    printf("==== BMS SYSTEM START ====\n");

    while(1)
    {
        t++;

        BMS_ReadSensor(&bms);

        BMS_UpdateSOC(&bms);

        BMS_CheckState(&bms);

        BMS_Control(&bms);

        BMS_Report(&bms);

        CSV_Write
        (
            t,
            bms.est.soc,
            BMS_GetPackVoltage(&bms),
            bms.raw.current,
            bms.raw.temperature,
            StateToStr(bms.safety.state),
            FaultToStr(bms.safety.fault_latch)
        );

        printf("---------------------\n");

        Sleep(1);
    }
    CSV_Close();
    return 0;
}
