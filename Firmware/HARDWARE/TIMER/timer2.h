#ifndef __TIMER2_H
#define __TIMER2_H
#include "sys.h"

 
#define Trig1 PAout(4) 
void TIM5_Cap_Init(u16 arr,u16 psc);
float ukalmanFilter_A(float inData);
u32 Getdis2(void);
u8 Command_Init2(void);
#endif
