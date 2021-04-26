#include "delay.h"
#include "sys.h"
#include "timer4.h"
#include "usart.h"
#include "lcd.h"
//主要程序都在main.c与usart.c

int main(void)
{	
	u16 xx=0,a;
	u16 pwmcompare1=0;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// 设置中断优先级分组2
	delay_init();	    	 //延时函数初始化	
	uart_init(9600);	 			//波特率9600	选用USART2 即A.2 A.3口 
	//USART_Configuration(9600); 这是USART3
	TIM1_PWM_Init(2000,720-1);//周期((1+719)/72000000)*(1+2000)=0.02s
		//预分频7200，自动重装载200，20ms时基值
  	//TIM_SetCompare3(TIM1,1000);50%占空比
	//通道1~4为PA8~11
	while(1)
	{	
	}
}



