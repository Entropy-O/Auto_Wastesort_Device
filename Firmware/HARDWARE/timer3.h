#ifndef __TIMER3_H
#define __TIMER3_H
#include "sys.h"

 
#define Trig2 PAout(12) 
#define Trig3 PAout(13) 
void TIM2_Cap_Init(u16 arr,u16 psc);
float kalmanFilter_A3(float inData);
u32 Getdis3(void);
u8 Command_Init3(void);
void TIM4_Cap_Init(u16 arr,u16 psc);
float kalmanFilter_A4(float inData);
u32 Getdis4(void);
u8 Command_Init4(void);
void TIM8_Cap_Init(u16 arr,u16 psc);
#endif
