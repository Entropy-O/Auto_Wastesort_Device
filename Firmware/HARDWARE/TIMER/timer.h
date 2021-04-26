#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"

 
#define Trig PAout(5) 
void TIM3_Cap_Init(u16 arr,u16 psc);
float kalmanFilter_A(float inData);
u32 Getdis(void);
u8 Command_Init1(void);
#endif
