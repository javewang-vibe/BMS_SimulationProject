#ifndef LOGGER_H
#define LOGGER_H

#ifdef __cplusplus
extern "C"
{
    #endif
    void CSV_Init(void);
    void CSV_Write(float soc, float voltage, float temp, const char* state);
    void CSV_Close(void);
    #ifdef __cplusplus
}
#endif

#endif