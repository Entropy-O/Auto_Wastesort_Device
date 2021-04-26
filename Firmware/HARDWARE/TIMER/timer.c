#include "timer.h"
#include "led.h"
#include "usart.h"
#include "sys.h"
#include "delay.h"
#include "lcd.h"
#include "timer2.h"
#include "string.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK Mini STM32������
//PWM  ��������			   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2010/12/03
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ����ԭ�� 2009-2019
//All rights reserved
////////////////////////////////////////////////////////////////////////////////// 	  


//��ʱ��3ͨ��1���벶������
#define Trig PAout(5) 
TIM_ICInitTypeDef  TIM3_ICInitStructure;
void TIM3_Cap_Init(u16 arr,u16 psc)
{	 
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//ʹ��TIM3ʱ��
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //ʹ��GPIOAʱ��
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6;  //PA0 ���֮ǰ����  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0 ����  
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_6);	
	//PA6 ����
  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode= GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_5);
	//��ʼ����ʱ��3 TIM3 
	TIM_TimeBaseStructure.TIM_Period = arr; //�趨�������Զ���װֵ 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 	//Ԥ��Ƶ��   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
  
	//��ʼ��TIM3���벶�����
	TIM3_ICInitStructure.TIM_Channel = TIM_Channel_1; //CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
  	TIM3_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//�����ز���
  	TIM3_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI1��
  	TIM3_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //���������Ƶ,����Ƶ 
  	TIM3_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 ���������˲��� ���˲�
  	TIM_ICInit(TIM3, &TIM3_ICInitStructure);
	
	//�жϷ����ʼ��
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM2�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //��ռ���ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ��� 
	
	TIM_ITConfig(TIM3,TIM_IT_Update|TIM_IT_CC1,ENABLE);//��������ж� ,����CC1IE�����ж�	
	
  TIM_Cmd(TIM3,ENABLE ); 	//ʹ�ܶ�ʱ��3
 
}


u8  TIM3CH1_CAPTURE_STA=0;	//���벶��״̬		    				
u16	TIM3CH1_CAPTURE_VAL;	//���벶��ֵ
 
//��ʱ��3�жϷ������	 
void TIM3_IRQHandler(void)
{ 
//1�������жϲ���
 	if((TIM3CH1_CAPTURE_STA&0X80)==0)//��δ�ɹ�����	
	{	  
		if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)		 
		{	    
			if(TIM3CH1_CAPTURE_STA&0X40)//�Ѿ����񵽸ߵ�ƽ��
			{
				if((TIM3CH1_CAPTURE_STA&0X3F)==0X3F)//�ߵ�ƽ̫����111111
				{
					TIM3CH1_CAPTURE_STA|=0X80;//��ǳɹ�������һ��
					TIM3CH1_CAPTURE_VAL=0XFFFF;
				}else TIM3CH1_CAPTURE_STA++;
			}	 
		}//2�������жϲ���
	if (TIM_GetITStatus(TIM3, TIM_IT_CC1) != RESET)//����1���������¼�
		{	
			if(TIM3CH1_CAPTURE_STA&0X40)		//����һ���½��� 		
			{	  			
				TIM3CH1_CAPTURE_STA|=0X80;		//��ǲ������
				TIM3CH1_CAPTURE_VAL=TIM_GetCapture1(TIM3);//������Ĵ�����ֵ��¼��TIM3CH1_CAPTURE_VAL��
		   		TIM_OC1PolarityConfig(TIM3,TIM_ICPolarity_Rising); //CC1P=0 ����Ϊ�����ز���
			}else  								//��δ��ʼ,��һ�β���������
			{
				TIM3CH1_CAPTURE_STA=0;			//���
				TIM3CH1_CAPTURE_VAL=0;
	 			TIM_SetCounter(TIM3,0);//��ʼ������ֵ
				TIM3CH1_CAPTURE_STA|=0X40;		//��ǲ�����������
		   		TIM_OC1PolarityConfig(TIM3,TIM_ICPolarity_Falling);		//CC1P=1 ����Ϊ�½��ز���
			}		    
		}			     	    					   
 	}
 
    TIM_ClearITPendingBit(TIM3, TIM_IT_CC1|TIM_IT_Update); //����жϱ�־λ
 
}
u32 Getdis(void)
{
		u32 temp=0;
    static u32 length=0,length1=0;
		Trig=1;
		delay_us(10);              //����һ��10us�ĸߵ�ƽ
		Trig=0;
		delay_us(2);
    if(TIM3CH1_CAPTURE_STA&0X80)//�ɹ�������һ�θߵ�ƽ
		{
			temp=TIM3CH1_CAPTURE_STA&0X3F;
			temp*=65536;					//���ʱ���ܺ�
			temp+=TIM3CH1_CAPTURE_VAL;			//�õ��ܵĸߵ�ƽʱ��
			length1=temp*0.017; 			//���㳤��
			length=kalmanFilter_A(length1); 
			if(Command_Init1())
				printf("HIGH:%d cm\r\n",length); //��ӡ����
 			TIM3CH1_CAPTURE_STA=0;			//������һ�β���
 		}
			return length;
}
float kalmanFilter_A(float inData) 
{
  static float prevData=0; 
  static float p=10, q=0.0008, r=0.002, kGain=0;
    p = p+q; 
    kGain = p/(p+r);

    inData = prevData+(kGain*(inData-prevData)); //(inData-prevData)����ϵͳ״̬��kGain���������棬p���Э���q���ϸ�˹�ֲ��Ĺ���������r���ϸ�˹�ֲ��Ĳ������� -- ��˹������
    p = (1-kGain)*p;

    prevData = inData;

    return inData; 
}
//����p�ĳ�ֵ�������ȡ�����ǲ���Ϊ0��Ϊ0�Ļ��������˲�������Ϊ�Ѿ��������˲����ˣ� 
//q,r��ֵ��Ҫ�����Գ���
//r���������˲����������ʵ�����ߵ�����̶ȣ�rԽСԽ�ӽ���q�������˲��������ƽ���̶ȣ�qԽСԽƽ����
u8 Command_Init1()
{
	u8 i,p;
	u8 USART_RX_BUF1[]="ultrasonic1";
	for(i=0;i<=11;i++){
		if(USART_RX_BUF1[i]!=USART_RX_BUF[i]){
			p=1;
		break;
	}
		else
			p=0;
	}
return 0;	
}



