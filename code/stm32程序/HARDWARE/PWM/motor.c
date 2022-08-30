#include "motor.h"

//TIM2 PWM
//0~180：5~25
void TIM2_PWM_Init(u16 arr,u16 psc)
{
	GPIO_InitTypeDef gpio;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInit;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //时钟使能
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOE, ENABLE);
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource1,GPIO_AF_TIM2);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_TIM2);

	gpio.GPIO_Pin=GPIO_Pin_1|GPIO_Pin_2;     //TIM2 pwm通道引脚
	gpio.GPIO_Mode=GPIO_Mode_AF;									//复用功能
	gpio.GPIO_Speed=GPIO_Speed_100MHz;
	gpio.GPIO_OType = GPIO_OType_PP;      							//推挽复用输出
	gpio.GPIO_PuPd = GPIO_PuPd_UP;        							//上拉	
	GPIO_Init(GPIOA,&gpio);
	

	
	//定时器TIM2初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位

 	
	TIM_OCInit.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInit.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInit.TIM_OCPolarity=TIM_OCPolarity_High;
	

	TIM_OC2Init(TIM2,&TIM_OCInit);    //通道2
	TIM_OC3Init(TIM2,&TIM_OCInit);    //通道2
	

	
	TIM_OC2PreloadConfig(TIM2,TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM2,TIM_OCPreload_Enable);
	

	
	TIM_ARRPreloadConfig(TIM2,ENABLE);//ARPE使能 

	
	TIM_Cmd(TIM2, ENABLE);  //使能TIM2

	
	TIM_SetCompare2(TIM2,0);
	TIM_SetCompare3(TIM2,0);

}


void Tim1_user(u32 arr,u32 psc)
{
	

	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);  	//TIM1时钟使能    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 	//使能PORTA时钟	
	
	GPIO_PinAFConfig(GPIOA ,GPIO_PinSource11,GPIO_AF_TIM1); //GPIOA8复用为定时器1
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;           //GPIOA8
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;        //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure);              //初始化PA8
	  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period=arr;   //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;  //20201020
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);	//初始化定时器1
	
	//初始化TIM1 Channel1 PWM模式	
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputState_Disable;//20201020		
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式1
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性低
	TIM_OCInitStructure.TIM_OCNPolarity= TIM_OCPolarity_High;//20201020
 
	TIM_OCInitStructure.TIM_Pulse=400;
	TIM_OC4Init(TIM1, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM1 4OC1
 
	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);  //使能TIM1在CCR1上的预装载寄存器
 
	TIM_ARRPreloadConfig(TIM1,ENABLE);//ARPE使能 
	
	TIM_Cmd(TIM1, ENABLE);  //使能TIM1
	
	TIM_CtrlPWMOutputs(TIM1, ENABLE);    //20201020
	TIM_SetCompare4(TIM1,0);

}


void Tim_user(u32 arr,u32 psc)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
	
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period=arr;   //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;  //20201020
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);	//初始化定时器1
			// 清除计数器中断标志位
    TIM_ClearFlag(TIM3, TIM_FLAG_Update);
	  
		// 开启计数器中断
    TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);


	  NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;//外部中断2
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//抢占优先级3
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;//子优先级2
	  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
	  NVIC_Init(&NVIC_InitStructure);//配置
}


