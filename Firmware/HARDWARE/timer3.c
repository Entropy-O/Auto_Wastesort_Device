#include "timer3.h"
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
#define Trig2 PAout(12) 
#define Trig3 PAout(13) 
TIM_ICInitTypeDef  TIM2_ICInitStructure;
void TIM2_Cap_Init(u16 arr,u16 psc)
{	 
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	//ʹ��TIM3ʱ��
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //ʹ��GPIOAʱ��
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1;  //PA0 ���֮ǰ����  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0 ����  
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_1);	
	//PA0 ����
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode= GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_12);
	//��ʼ����ʱ��2 TIM2 
	TIM_TimeBaseStructure.TIM_Period = arr; //�趨�������Զ���װֵ 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 	//Ԥ��Ƶ��   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
  
	//��ʼ��TIM2���벶�����
	TIM2_ICInitStructure.TIM_Channel = TIM_Channel_2; //CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
  	TIM2_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//�����ز���
  	TIM2_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI1��
  	TIM2_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //���������Ƶ,����Ƶ 
  	TIM2_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 ���������˲��� ���˲�
  	TIM_ICInit(TIM2, &TIM2_ICInitStructure);
	
	//�жϷ����ʼ��
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM2�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //��ռ���ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ��� 
	
	TIM_ITConfig(TIM2,TIM_IT_Update|TIM_IT_CC2,ENABLE);//��������ж� ,����CC1IE�����ж�	
	
  TIM_Cmd(TIM2,ENABLE ); 	//ʹ�ܶ�ʱ��3
 
}


u8  TIM2CH2_CAPTURE_STA=0;	//���벶��״̬		    				
u16	TIM2CH2_CAPTURE_VAL;	//���벶��ֵ
 
//��ʱ��3�жϷ������	 
void TIM2_IRQHandler(void)
{ 
//1�������жϲ���
 	if((TIM2CH2_CAPTURE_STA&0X80)==0)//��δ�ɹ�����	
	{	  
		if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)		 
		{	    
			if(TIM2CH2_CAPTURE_STA&0X40)//�Ѿ����񵽸ߵ�ƽ��
			{
				if((TIM2CH2_CAPTURE_STA&0X3F)==0X3F)//�ߵ�ƽ̫����111111
				{
					TIM2CH2_CAPTURE_STA|=0X80;//��ǳɹ�������һ��
					TIM2CH2_CAPTURE_VAL=0XFFFF;
				}else TIM2CH2_CAPTURE_STA++;
			}	 
		}//2�������жϲ���
	if (TIM_GetITStatus(TIM2, TIM_IT_CC2) != RESET)//����1���������¼�
		{	
			if(TIM2CH2_CAPTURE_STA&0X40)		//����һ���½��� 		
			{	  			
				TIM2CH2_CAPTURE_STA|=0X80;		//��ǲ������
				TIM2CH2_CAPTURE_VAL=TIM_GetCapture2(TIM2);//������Ĵ�����ֵ��¼��TIM3CH1_CAPTURE_VAL��
		   		TIM_OC2PolarityConfig(TIM2,TIM_ICPolarity_Rising); //CC1P=0 ����Ϊ�����ز���
			}else  								//��δ��ʼ,��һ�β���������
			{
				TIM2CH2_CAPTURE_STA=0;			//���
				TIM2CH2_CAPTURE_VAL=0;
	 			TIM_SetCounter(TIM2,0);//��ʼ������ֵ
				TIM2CH2_CAPTURE_STA|=0X40;		//��ǲ�����������
		   		TIM_OC2PolarityConfig(TIM2,TIM_ICPolarity_Falling);		//CC1P=1 ����Ϊ�½��ز���
			}		    
		}			     	    					   
 	}
 
    TIM_ClearITPendingBit(TIM2, TIM_IT_CC2|TIM_IT_Update); //����жϱ�־λ
 
}
u32 Getdis3(void)
{
		u32 temp=0;
    static u32 length=0,length1=0;
		Trig2=1;
		delay_us(10);              //����һ��10us�ĸߵ�ƽ
		Trig2=0;
		delay_us(2);
    if(TIM2CH2_CAPTURE_STA&0X80)//�ɹ�������һ�θߵ�ƽ
		{
			temp=TIM2CH2_CAPTURE_STA&0X3F;
			temp*=65536;					//���ʱ���ܺ�
			temp+=TIM2CH2_CAPTURE_VAL;			//�õ��ܵĸߵ�ƽʱ��
			length1=temp*0.017; 			//���㳤��
			length=kalmanFilter_A3(length1); 
			if(Command_Init3())
				printf("HIGH:%d cm\r\n",length); //��ӡ����
 			TIM2CH2_CAPTURE_STA=0;			//������һ�β���
 		}
			return length;
}
float kalmanFilter_A3(float inData) 
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
u8 Command_Init3()
{
	u8 i,p;
	u8 USART_RX_BUF3[]="ultrasonic3";
	for(i=0;i<=11;i++){
		if(USART_RX_BUF3[i]!=USART_RX_BUF[i]){
			p=1;
		break;
	}
		else
			p=0;
	}
return 0;	
}

TIM_ICInitTypeDef  TIM4_ICInitStructure;
void TIM4_Cap_Init(u16 arr,u16 psc)
{	 
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);	//ʹ��TIM3ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);  //ʹ��GPIOAʱ��

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_9;  //PA0 ���֮ǰ����  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0 ����  
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOB,GPIO_Pin_9);	
	//PB6 ����
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode= GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_13);
	//��ʼ����ʱ��4 TIM4 
	TIM_TimeBaseStructure.TIM_Period = arr; //�趨�������Զ���װֵ 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 	//Ԥ��Ƶ��   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
  
	//��ʼ��TIM4���벶�����
	TIM4_ICInitStructure.TIM_Channel = TIM_Channel_4; //CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
  	TIM4_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//�����ز���
  	TIM4_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI1��
  	TIM4_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //���������Ƶ,����Ƶ 
  	TIM4_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 ���������˲��� ���˲�
  	TIM_ICInit(TIM4, &TIM4_ICInitStructure);
	
	//�жϷ����ʼ��
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM2�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //��ռ���ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ��� 
	
	TIM_ITConfig(TIM4,TIM_IT_Update|TIM_IT_CC4,ENABLE);//��������ж� ,����CC1IE�����ж�	
	
	TIM_Cmd(TIM4,ENABLE ); 	//ʹ�ܶ�ʱ��3
 
}


u8  TIM4CH2_CAPTURE_STA=0;	//���벶��״̬		    				
u16	TIM4CH2_CAPTURE_VAL;	//���벶��ֵ
 
//��ʱ��3�жϷ������	 
void TIM4_IRQHandler(void)
{ 
//1�������жϲ���
 	if((TIM4CH2_CAPTURE_STA&0X80)==0)//��δ�ɹ�����	
	{	  
		if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)		 
		{	    
			if(TIM4CH2_CAPTURE_STA&0X40)//�Ѿ����񵽸ߵ�ƽ��
			{
				if((TIM4CH2_CAPTURE_STA&0X3F)==0X3F)//�ߵ�ƽ̫����111111
				{
					TIM4CH2_CAPTURE_STA|=0X80;//��ǳɹ�������һ��
					TIM4CH2_CAPTURE_VAL=0XFFFF;
				}
				else 
					TIM4CH2_CAPTURE_STA++;
			}	 
		}//2�������жϲ���
	if (TIM_GetITStatus(TIM4, TIM_IT_CC4) != RESET)//����1���������¼�
		{	
			if(TIM4CH2_CAPTURE_STA&0X40)		//����һ���½��� 		
			{	  					
				TIM4CH2_CAPTURE_STA|=0X80;		//��ǲ������
				TIM4CH2_CAPTURE_VAL=TIM_GetCapture4(TIM4);//������Ĵ�����ֵ��¼��TIM3CH1_CAPTURE_VAL��
		   		TIM_OC4PolarityConfig(TIM4,TIM_ICPolarity_Rising); //CC1P=0 ����Ϊ�����ز���
			}
			else  								//��δ��ʼ,��һ�β���������
			{
				TIM4CH2_CAPTURE_STA=0;			//���
				TIM4CH2_CAPTURE_VAL=0;
	 			TIM_SetCounter(TIM4,0);//��ʼ������ֵ
				TIM4CH2_CAPTURE_STA|=0X40;		//��ǲ�����������
		   		TIM_OC4PolarityConfig(TIM4,TIM_ICPolarity_Falling);		//CC1P=1 ����Ϊ�½��ز���
			}		    
		}			     	    					   
 	}
 
    TIM_ClearITPendingBit(TIM4, TIM_IT_CC4|TIM_IT_Update); //����жϱ�־λ
 
}

u32 Getdis4(void)
{
	u32 temp=0;
	static u32 length=0,length1=0;
	Trig3=1;
	delay_us(10);              //����һ��10us�ĸߵ�ƽ
	Trig3=0;
	delay_us(2);
    if(TIM4CH2_CAPTURE_STA&0X80)//�ɹ�������һ�θߵ�ƽ
	{
		temp=TIM4CH2_CAPTURE_STA&0X3F;
		temp*=65536;					//���ʱ���ܺ�
		temp+=TIM4CH2_CAPTURE_VAL;			//�õ��ܵĸߵ�ƽʱ��
		length1=temp*0.017; 			//���㳤��
		length=kalmanFilter_A4(length1); 
		if(Command_Init4())
			printf("HIGH:%d cm\r\n",length); //��ӡ����
		TIM4CH2_CAPTURE_STA=0;			//������һ�β���
	}
	return length;
}
float kalmanFilter_A4(float inData) 
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
u8 Command_Init4()
{
	u8 i,p;
	u8 USART_RX_BUF4[]="ultrasonic4";
	for(i=0;i<=11;i++)
	{
		if(USART_RX_BUF4[i]!=USART_RX_BUF[i])
		{
			p=1;
			break;
		}
		else
			p=0;
	}
	return 0;	
}

//TIM_ICInitTypeDef  TIM8_ICInitStructure;
//void TIM8_Cap_Init(u16 arr,u16 psc)
//{	 
//	GPIO_InitTypeDef GPIO_InitStructure;
//	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;

//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC|RCC_APB2Periph_TIM8, ENABLE);  //ʹ��GPIOAʱ��

//	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6;  //PA0 ���֮ǰ����  
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0 ����  
//	GPIO_Init(GPIOC, &GPIO_InitStructure);
//	GPIO_ResetBits(GPIOC,GPIO_Pin_6);	
//	//PB6 ����
//	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_13;
//	GPIO_InitStructure.GPIO_Mode= GPIO_Mode_Out_PP;
//	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);
//	GPIO_SetBits(GPIOA,GPIO_Pin_13);
//	//��ʼ����ʱ��4 TIM4 
//	TIM_TimeBaseStructure.TIM_Period = arr; //�趨�������Զ���װֵ 
//	TIM_TimeBaseStructure.TIM_Prescaler =psc; 	//Ԥ��Ƶ��   
//	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
//	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
//  
//	//��ʼ��TIM4���벶�����
//	TIM8_ICInitStructure.TIM_Channel = TIM_Channel_1; //CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
//  	TIM8_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//�����ز���
//  	TIM8_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI1��
//  	TIM8_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //���������Ƶ,����Ƶ 
//  	TIM8_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 ���������˲��� ���˲�
//  	TIM_ICInit(TIM8, &TIM8_ICInitStructure);
//	
//	//�жϷ����ʼ��
//	NVIC_InitStructure.NVIC_IRQChannel = TIM8_CC_IRQn;  //TIM2�ж�
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //��ռ���ȼ�2��
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�0��
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
//	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ��� 
//	
//	TIM_ITConfig(TIM8,TIM_IT_Update|TIM_IT_CC1,ENABLE);//��������ж� ,����CC1IE�����ж�	
//	
//	TIM_Cmd(TIM8,ENABLE ); 	//ʹ�ܶ�ʱ��3
// 
//}

//void TIM8_CC_IRQHandler(void)
//{ 
////1�������жϲ���
// 	if((TIM4CH2_CAPTURE_STA&0X80)==0)//��δ�ɹ�����	
//	{	  
//		if (TIM_GetITStatus(TIM8, TIM_IT_Update) != RESET)		 
//		{	    
//			if(TIM4CH2_CAPTURE_STA&0X40)//�Ѿ����񵽸ߵ�ƽ��
//			{
//				if((TIM4CH2_CAPTURE_STA&0X3F)==0X3F)//�ߵ�ƽ̫����111111
//				{
//					TIM4CH2_CAPTURE_STA|=0X80;//��ǳɹ�������һ��
//					TIM4CH2_CAPTURE_VAL=0XFFFF;
//				}
//				else 
//					TIM4CH2_CAPTURE_STA++;
//			}	 
//		}//2�������жϲ���
//	if (TIM_GetITStatus(TIM8, TIM_IT_CC1) != RESET)//����1���������¼�
//		{	
//			if(TIM4CH2_CAPTURE_STA&0X40)		//����һ���½��� 		
//			{	  					
//				TIM4CH2_CAPTURE_STA|=0X80;		//��ǲ������
//				TIM4CH2_CAPTURE_VAL=TIM_GetCapture1(TIM8);//������Ĵ�����ֵ��¼��TIM3CH1_CAPTURE_VAL��
//		   		TIM_OC1PolarityConfig(TIM8,TIM_ICPolarity_Rising); //CC1P=0 ����Ϊ�����ز���
//			}
//			else  								//��δ��ʼ,��һ�β���������
//			{
//				TIM4CH2_CAPTURE_STA=0;			//���
//				TIM4CH2_CAPTURE_VAL=0;
//	 			TIM_SetCounter(TIM8,0);//��ʼ������ֵ
//				TIM4CH2_CAPTURE_STA|=0X40;		//��ǲ�����������
//		   		TIM_OC1PolarityConfig(TIM8,TIM_ICPolarity_Falling);		//CC1P=1 ����Ϊ�½��ز���
//			}		    
//		}			     	    					   
// 	}
// 
//    TIM_ClearITPendingBit(TIM8, TIM_IT_CC1|TIM_IT_Update); //����жϱ�־λ
// 
//}