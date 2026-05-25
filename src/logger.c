#include <stdio.h>
#include "logger.h"

FILE *fp;

void CSV_Init()
{
    fp = fopen("bms_log.csv","w");
    if(fp == NULL)
    {
        printf("Failed to open CSV file!\n");
        return;
    }
    fprintf(fp,"time,soc,voltage,temp,state\n");
}

void CSV_Write(int time,float soc, float voltage, float temp, const char* state)
{
    if(fp == NULL) return;
    fprintf(fp,"%d,%.6f,%.2f,%.2f,%s\n",time,soc,voltage,temp,state);
    fflush(fp); 
}

void CSV_Close()
{
    if(fp != NULL)
    {
        fclose(fp);
        fp = NULL;
    }
}