#ifndef LOGGER_H
#define LOGGER_H

void CSV_Init(void);
void CSV_Write(int time, float soc_true, float soc_ekf, float voltage, float current, float temp, const char* state, const char* fault);
void CSV_Close(void);

#endif
