#include "delay.h"
#include "sys.h"
#include "timer4.h"
#include "usart.h"
#include "lcd.h"
//��Ҫ������main.c��usart.c

int main(void)
{	
	u16 xx=0,a;
	u16 pwmcompare1=0;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// �����ж����ȼ�����2
	delay_init();	    	 //��ʱ������ʼ��	
	uart_init(9600);	 			//������9600	ѡ��USART2 ��A.2 A.3�� 
	//USART_Configuration(9600); ����USART3
	TIM1_PWM_Init(2000,720-1);//����((1+719)/72000000)*(1+2000)=0.02s
		//Ԥ��Ƶ7200���Զ���װ��200��20msʱ��ֵ
  	//TIM_SetCompare3(TIM1,1000);50%ռ�ձ�
	//ͨ��1~4ΪPA8~11
	while(1)
	{	
	}
}



