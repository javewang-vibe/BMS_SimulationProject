#include <stdio.h>
#include "logger.h"

static FILE *fp;

void CSV_Init(void)
{
    fp = fopen("bms_log.csv","w");
    if(fp == NULL)
    {
        printf("Failed to open CSV file!\n");
        return;
    }
    fprintf(fp,"time,soc,voltage,current,temp,state,fault\n");
}

void CSV_Write(int time, float soc, float voltage, float current, float temp, const char* state, const char* fault)
{
    if(fp == NULL) return;
    fprintf(fp,"%d,%.6f,%.2f,%.2f,%.2f,%s,%s\n", time, soc, voltage, current, temp, state, fault);
    fflush(fp); 
}

void CSV_Close(void)
{
    if(fp != NULL)
    {
        fclose(fp);
        fp = NULL;
    }
}
