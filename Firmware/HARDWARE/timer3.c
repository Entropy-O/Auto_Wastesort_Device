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
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK Mini STM32开发板
//PWM  驱动代码			   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2010/12/03
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 正点原子 2009-2019
//All rights reserved
////////////////////////////////////////////////////////////////////////////////// 	  


//定时器3通道1输入捕获配置
#define Trig2 PAout(12) 
#define Trig3 PAout(13) 
TIM_ICInitTypeDef  TIM2_ICInitStructure;
void TIM2_Cap_Init(u16 arr,u16 psc)
{	 
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	//使能TIM3时钟
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //使能GPIOA时钟
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1;  //PA0 清除之前设置  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0 输入  
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_1);	
	//PA0 下拉
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode= GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_12);
	//初始化定时器2 TIM2 
	TIM_TimeBaseStructure.TIM_Period = arr; //设定计数器自动重装值 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 	//预分频器   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
  
	//初始化TIM2输入捕获参数
	TIM2_ICInitStructure.TIM_Channel = TIM_Channel_2; //CC1S=01 	选择输入端 IC1映射到TI1上
  	TIM2_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//上升沿捕获
  	TIM2_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI1上
  	TIM2_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //配置输入分频,不分频 
  	TIM2_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 配置输入滤波器 不滤波
  	TIM_ICInit(TIM2, &TIM2_ICInitStructure);
	
	//中断分组初始化
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //先占优先级2级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //从优先级0级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器 
	
	TIM_ITConfig(TIM2,TIM_IT_Update|TIM_IT_CC2,ENABLE);//允许更新中断 ,允许CC1IE捕获中断	
	
  TIM_Cmd(TIM2,ENABLE ); 	//使能定时器3
 
}


u8  TIM2CH2_CAPTURE_STA=0;	//输入捕获状态		    				
u16	TIM2CH2_CAPTURE_VAL;	//输入捕获值
 
//定时器3中断服务程序	 
void TIM2_IRQHandler(void)
{ 
//1、更新中断部分
 	if((TIM2CH2_CAPTURE_STA&0X80)==0)//还未成功捕获	
	{	  
		if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)		 
		{	    
			if(TIM2CH2_CAPTURE_STA&0X40)//已经捕获到高电平了
			{
				if((TIM2CH2_CAPTURE_STA&0X3F)==0X3F)//高电平太长了111111
				{
					TIM2CH2_CAPTURE_STA|=0X80;//标记成功捕获了一次
					TIM2CH2_CAPTURE_VAL=0XFFFF;
				}else TIM2CH2_CAPTURE_STA++;
			}	 
		}//2、捕获中断部分
	if (TIM_GetITStatus(TIM2, TIM_IT_CC2) != RESET)//捕获1发生捕获事件
		{	
			if(TIM2CH2_CAPTURE_STA&0X40)		//捕获到一个下降沿 		
			{	  			
				TIM2CH2_CAPTURE_STA|=0X80;		//标记捕获完成
				TIM2CH2_CAPTURE_VAL=TIM_GetCapture2(TIM2);//将捕获寄存器的值记录在TIM3CH1_CAPTURE_VAL中
		   		TIM_OC2PolarityConfig(TIM2,TIM_ICPolarity_Rising); //CC1P=0 设置为上升沿捕获
			}else  								//还未开始,第一次捕获上升沿
			{
				TIM2CH2_CAPTURE_STA=0;			//清空
				TIM2CH2_CAPTURE_VAL=0;
	 			TIM_SetCounter(TIM2,0);//初始化捕获值
				TIM2CH2_CAPTURE_STA|=0X40;		//标记捕获到了上升沿
		   		TIM_OC2PolarityConfig(TIM2,TIM_ICPolarity_Falling);		//CC1P=1 设置为下降沿捕获
			}		    
		}			     	    					   
 	}
 
    TIM_ClearITPendingBit(TIM2, TIM_IT_CC2|TIM_IT_Update); //清除中断标志位
 
}
u32 Getdis3(void)
{
		u32 temp=0;
    static u32 length=0,length1=0;
		Trig2=1;
		delay_us(10);              //输入一个10us的高电平
		Trig2=0;
		delay_us(2);
    if(TIM2CH2_CAPTURE_STA&0X80)//成功捕获到了一次高电平
		{
			temp=TIM2CH2_CAPTURE_STA&0X3F;
			temp*=65536;					//溢出时间总和
			temp+=TIM2CH2_CAPTURE_VAL;			//得到总的高电平时间
			length1=temp*0.017; 			//计算长度
			length=kalmanFilter_A3(length1); 
			if(Command_Init3())
				printf("HIGH:%d cm\r\n",length); //打印长度
 			TIM2CH2_CAPTURE_STA=0;			//开启下一次捕获
 		}
			return length;
}
float kalmanFilter_A3(float inData) 
{
  static float prevData=0; 
  static float p=10, q=0.0008, r=0.002, kGain=0;
    p = p+q; 
    kGain = p/(p+r);

    inData = prevData+(kGain*(inData-prevData)); //(inData-prevData)修正系统状态，kGain卡尔曼增益，p误差协方差，q符合高斯分布的过程噪声、r符合高斯分布的测量噪声 -- 高斯白噪声
    p = (1-kGain)*p;

    prevData = inData;

    return inData; 
}
//其中p的初值可以随便取，但是不能为0（为0的话卡尔曼滤波器就认为已经是最优滤波器了） 
//q,r的值需要我们试出来
//r参数调整滤波后的曲线与实测曲线的相近程度，r越小越接近。q参数调滤波后的曲线平滑程度，q越小越平滑。
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

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);	//使能TIM3时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);  //使能GPIOA时钟

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_9;  //PA0 清除之前设置  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0 输入  
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOB,GPIO_Pin_9);	
	//PB6 下拉
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode= GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_13);
	//初始化定时器4 TIM4 
	TIM_TimeBaseStructure.TIM_Period = arr; //设定计数器自动重装值 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 	//预分频器   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
  
	//初始化TIM4输入捕获参数
	TIM4_ICInitStructure.TIM_Channel = TIM_Channel_4; //CC1S=01 	选择输入端 IC1映射到TI1上
  	TIM4_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//上升沿捕获
  	TIM4_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI1上
  	TIM4_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //配置输入分频,不分频 
  	TIM4_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 配置输入滤波器 不滤波
  	TIM_ICInit(TIM4, &TIM4_ICInitStructure);
	
	//中断分组初始化
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //先占优先级2级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //从优先级0级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器 
	
	TIM_ITConfig(TIM4,TIM_IT_Update|TIM_IT_CC4,ENABLE);//允许更新中断 ,允许CC1IE捕获中断	
	
	TIM_Cmd(TIM4,ENABLE ); 	//使能定时器3
 
}


u8  TIM4CH2_CAPTURE_STA=0;	//输入捕获状态		    				
u16	TIM4CH2_CAPTURE_VAL;	//输入捕获值
 
//定时器3中断服务程序	 
void TIM4_IRQHandler(void)
{ 
//1、更新中断部分
 	if((TIM4CH2_CAPTURE_STA&0X80)==0)//还未成功捕获	
	{	  
		if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)		 
		{	    
			if(TIM4CH2_CAPTURE_STA&0X40)//已经捕获到高电平了
			{
				if((TIM4CH2_CAPTURE_STA&0X3F)==0X3F)//高电平太长了111111
				{
					TIM4CH2_CAPTURE_STA|=0X80;//标记成功捕获了一次
					TIM4CH2_CAPTURE_VAL=0XFFFF;
				}
				else 
					TIM4CH2_CAPTURE_STA++;
			}	 
		}//2、捕获中断部分
	if (TIM_GetITStatus(TIM4, TIM_IT_CC4) != RESET)//捕获1发生捕获事件
		{	
			if(TIM4CH2_CAPTURE_STA&0X40)		//捕获到一个下降沿 		
			{	  					
				TIM4CH2_CAPTURE_STA|=0X80;		//标记捕获完成
				TIM4CH2_CAPTURE_VAL=TIM_GetCapture4(TIM4);//将捕获寄存器的值记录在TIM3CH1_CAPTURE_VAL中
		   		TIM_OC4PolarityConfig(TIM4,TIM_ICPolarity_Rising); //CC1P=0 设置为上升沿捕获
			}
			else  								//还未开始,第一次捕获上升沿
			{
				TIM4CH2_CAPTURE_STA=0;			//清空
				TIM4CH2_CAPTURE_VAL=0;
	 			TIM_SetCounter(TIM4,0);//初始化捕获值
				TIM4CH2_CAPTURE_STA|=0X40;		//标记捕获到了上升沿
		   		TIM_OC4PolarityConfig(TIM4,TIM_ICPolarity_Falling);		//CC1P=1 设置为下降沿捕获
			}		    
		}			     	    					   
 	}
 
    TIM_ClearITPendingBit(TIM4, TIM_IT_CC4|TIM_IT_Update); //清除中断标志位
 
}

u32 Getdis4(void)
{
	u32 temp=0;
	static u32 length=0,length1=0;
	Trig3=1;
	delay_us(10);              //输入一个10us的高电平
	Trig3=0;
	delay_us(2);
    if(TIM4CH2_CAPTURE_STA&0X80)//成功捕获到了一次高电平
	{
		temp=TIM4CH2_CAPTURE_STA&0X3F;
		temp*=65536;					//溢出时间总和
		temp+=TIM4CH2_CAPTURE_VAL;			//得到总的高电平时间
		length1=temp*0.017; 			//计算长度
		length=kalmanFilter_A4(length1); 
		if(Command_Init4())
			printf("HIGH:%d cm\r\n",length); //打印长度
		TIM4CH2_CAPTURE_STA=0;			//开启下一次捕获
	}
	return length;
}
float kalmanFilter_A4(float inData) 
{
  static float prevData=0; 
  static float p=10, q=0.0008, r=0.002, kGain=0;
    p = p+q; 
    kGain = p/(p+r);

    inData = prevData+(kGain*(inData-prevData)); //(inData-prevData)修正系统状态，kGain卡尔曼增益，p误差协方差，q符合高斯分布的过程噪声、r符合高斯分布的测量噪声 -- 高斯白噪声
    p = (1-kGain)*p;

    prevData = inData;

    return inData; 
}
//其中p的初值可以随便取，但是不能为0（为0的话卡尔曼滤波器就认为已经是最优滤波器了） 
//q,r的值需要我们试出来
//r参数调整滤波后的曲线与实测曲线的相近程度，r越小越接近。q参数调滤波后的曲线平滑程度，q越小越平滑。
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

//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC|RCC_APB2Periph_TIM8, ENABLE);  //使能GPIOA时钟

//	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6;  //PA0 清除之前设置  
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0 输入  
//	GPIO_Init(GPIOC, &GPIO_InitStructure);
//	GPIO_ResetBits(GPIOC,GPIO_Pin_6);	
//	//PB6 下拉
//	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_13;
//	GPIO_InitStructure.GPIO_Mode= GPIO_Mode_Out_PP;
//	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);
//	GPIO_SetBits(GPIOA,GPIO_Pin_13);
//	//初始化定时器4 TIM4 
//	TIM_TimeBaseStructure.TIM_Period = arr; //设定计数器自动重装值 
//	TIM_TimeBaseStructure.TIM_Prescaler =psc; 	//预分频器   
//	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
//	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
//  
//	//初始化TIM4输入捕获参数
//	TIM8_ICInitStructure.TIM_Channel = TIM_Channel_1; //CC1S=01 	选择输入端 IC1映射到TI1上
//  	TIM8_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//上升沿捕获
//  	TIM8_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI1上
//  	TIM8_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //配置输入分频,不分频 
//  	TIM8_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 配置输入滤波器 不滤波
//  	TIM_ICInit(TIM8, &TIM8_ICInitStructure);
//	
//	//中断分组初始化
//	NVIC_InitStructure.NVIC_IRQChannel = TIM8_CC_IRQn;  //TIM2中断
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //先占优先级2级
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //从优先级0级
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
//	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器 
//	
//	TIM_ITConfig(TIM8,TIM_IT_Update|TIM_IT_CC1,ENABLE);//允许更新中断 ,允许CC1IE捕获中断	
//	
//	TIM_Cmd(TIM8,ENABLE ); 	//使能定时器3
// 
//}

//void TIM8_CC_IRQHandler(void)
//{ 
////1、更新中断部分
// 	if((TIM4CH2_CAPTURE_STA&0X80)==0)//还未成功捕获	
//	{	  
//		if (TIM_GetITStatus(TIM8, TIM_IT_Update) != RESET)		 
//		{	    
//			if(TIM4CH2_CAPTURE_STA&0X40)//已经捕获到高电平了
//			{
//				if((TIM4CH2_CAPTURE_STA&0X3F)==0X3F)//高电平太长了111111
//				{
//					TIM4CH2_CAPTURE_STA|=0X80;//标记成功捕获了一次
//					TIM4CH2_CAPTURE_VAL=0XFFFF;
//				}
//				else 
//					TIM4CH2_CAPTURE_STA++;
//			}	 
//		}//2、捕获中断部分
//	if (TIM_GetITStatus(TIM8, TIM_IT_CC1) != RESET)//捕获1发生捕获事件
//		{	
//			if(TIM4CH2_CAPTURE_STA&0X40)		//捕获到一个下降沿 		
//			{	  					
//				TIM4CH2_CAPTURE_STA|=0X80;		//标记捕获完成
//				TIM4CH2_CAPTURE_VAL=TIM_GetCapture1(TIM8);//将捕获寄存器的值记录在TIM3CH1_CAPTURE_VAL中
//		   		TIM_OC1PolarityConfig(TIM8,TIM_ICPolarity_Rising); //CC1P=0 设置为上升沿捕获
//			}
//			else  								//还未开始,第一次捕获上升沿
//			{
//				TIM4CH2_CAPTURE_STA=0;			//清空
//				TIM4CH2_CAPTURE_VAL=0;
//	 			TIM_SetCounter(TIM8,0);//初始化捕获值
//				TIM4CH2_CAPTURE_STA|=0X40;		//标记捕获到了上升沿
//		   		TIM_OC1PolarityConfig(TIM8,TIM_ICPolarity_Falling);		//CC1P=1 设置为下降沿捕获
//			}		    
//		}			     	    					   
// 	}
// 
//    TIM_ClearITPendingBit(TIM8, TIM_IT_CC1|TIM_IT_Update); //清除中断标志位
// 
//}