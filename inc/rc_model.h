#ifndef RC_MODEL_H
#define RC_MODEL_H

#define R0 0.05f
#define R1 0.02f
#define C1 2000.0f

typedef struct RcModel
{
    float v_rc;
}RcModel;

void RcModel_Init(RcModel* rc);
float RcModel_Update(RcModel* rc,float current,float ocv);

#endif
