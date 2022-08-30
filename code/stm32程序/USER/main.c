#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "adc.h"

#include "includes.h"
#include "motor.h"
#include "usart.h"
#include "os_app_hooks.h"

#include "lcd.h"
#include "key.h"  
#include "timer.h"
#include "touch.h" 
#include "lvgl.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"
#include "gui.h"

//任务优先级
#define START_TASK_PRIO		3
//任务堆栈大小	
#define START_STK_SIZE 		128*2
//任务控制块
OS_TCB StartTaskTCB;
//任务堆栈	
CPU_STK START_TASK_STK[START_STK_SIZE];
//任务函数
void start_task(void *p_arg);

/*littleVGL任务*/
#define TASK1_TASK_PRIO		(4)
#define TASK1_STK_SIZE 		1024*5
OS_TCB Task1_TaskTCB;	
CPU_STK TASK1_TASK_STK[TASK1_STK_SIZE];

/*k210数据接收任务*/
#define TASK2_TASK_PRIO		(5)
#define TASK2_STK_SIZE 		1024*5
OS_TCB Task2_TaskTCB;
CPU_STK TASK2_TASK_STK[TASK2_STK_SIZE];

/*机械臂数据发送与状态监测任务*/
#define TASK3_TASK_PRIO		(6)
#define TASK3_STK_SIZE 		128*2
OS_TCB Task3_TaskTCB;
CPU_STK TASK3_TASK_STK[TASK3_STK_SIZE];

/*机械臂数据发送与状态监测任务*/
#define TASK4_TASK_PRIO		(7)
#define TASK4_STK_SIZE 		1024
OS_TCB Task4_TaskTCB;
CPU_STK TASK4_TASK_STK[TASK4_STK_SIZE];



void GUI_task(void *p_arg);
void K210_data(void *p_arg);
void arms_Monitoring(void *p_arg);
void Sensor(void *p_arg);

extern u8 sed_logo;
extern u8 id_log;
extern uint8_t usart_buff[];
extern int sed_temp;
extern u8 read_logo;

extern int x;
extern int y;
extern int id;

extern lv_obj_t *led1;
extern lv_obj_t *gauge1;
extern lv_obj_t *gauge2;
extern lv_obj_t *gauge3;
extern lv_obj_t *gauge_btn1;



int main(void)
{	
	OS_ERR err;
	CPU_SR_ALLOC();
	
	delay_init(168);  								//时钟初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //中断分组配置
	Tim1_user(270-1,168-1);							//步进电机初始化
	uart_init(115200);								//串口初始化
	LCD_Init();										//LCD初始化
	TIM3_Int_Init(999,83);							//定时器初始化(1ms中断),用于给lvgl提供1ms的心跳节拍
	Adc_Init();										//各传感器初始化
	tp_dev.init();									//触摸屏初始化
	lv_init();										//lvgl系统初始化
	lv_port_disp_init();							//lvgl显示接口初始化,放在lv_init()的后面
	lv_port_indev_init();							//lvgl输入接口初始化,放在lv_init()的后面
	OSInit(&err);		    						//初始化UCOSIII
	OS_CRITICAL_ENTER();							//进入临界区			 
	//创建开始任务
	OSTaskCreate((OS_TCB 	* )&StartTaskTCB,		//任务控制块
				 (CPU_CHAR	* )"start task", 		//任务名字
                 (OS_TASK_PTR )start_task, 			//任务函数
                 (void		* )0,					//传递给任务函数的参数
                 (OS_PRIO	  )START_TASK_PRIO,     //任务优先级
                 (CPU_STK   * )&START_TASK_STK[0],	//任务堆栈基地址
                 (CPU_STK_SIZE)START_STK_SIZE/10,	//任务堆栈深度限位
                 (CPU_STK_SIZE)START_STK_SIZE,		//任务堆栈大小
                 (OS_MSG_QTY  )0,					//任务内部消息队列能够接收的最大消息数目,为0时禁止接收消息
                 (OS_TICK	  )0,					//当使能时间片轮转时的时间片长度，为0时为默认长度，
                 (void   	* )0,					//用户补充的存储区
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //任务选项
                 (OS_ERR 	* )&err);				//存放该函数错误时的返回值
	OS_CRITICAL_EXIT();	//退出临界区	 
	OSStart(&err);      //开启UCOSIII
	
}

//开始任务任务函数
void start_task(void *p_arg)
{
	int task_num=0;
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;
	CPU_Init();	
	OS_CRITICAL_ENTER();	//进入临界区
	user_gui_init();
	OSTaskCreate((OS_TCB 	* )&Task1_TaskTCB,		
				 (CPU_CHAR	* )"Task1 task", 		
                 (OS_TASK_PTR )GUI_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )TASK1_TASK_PRIO,     
                 (CPU_STK   * )&TASK1_TASK_STK[0],	
                 (CPU_STK_SIZE)TASK1_STK_SIZE/10,	
                 (CPU_STK_SIZE)TASK1_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,					
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR),
                 (OS_ERR 	* )&err);

	OSTaskCreate((OS_TCB 	* )&Task2_TaskTCB,		
					 (CPU_CHAR	* )"Task2 task", 		
					 (OS_TASK_PTR )K210_data, 			
					 (void		* )0,					
					 (OS_PRIO	  )TASK2_TASK_PRIO,     
					 (CPU_STK   * )&TASK2_TASK_STK[0],	
					 (CPU_STK_SIZE)TASK2_STK_SIZE/10,	
					 (CPU_STK_SIZE)TASK2_STK_SIZE,		
					 (OS_MSG_QTY  )0,					
					 (OS_TICK	  )0,					
					 (void   	* )0,					
					 (OS_OPT      )(OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR),
					 (OS_ERR 	* )&err);	

	OSTaskCreate((OS_TCB 	* )&Task3_TaskTCB,		
					 (CPU_CHAR	* )"Task3 task", 		
					 (OS_TASK_PTR )arms_Monitoring, 			
					 (void		* )0,					
					 (OS_PRIO	  )TASK3_TASK_PRIO,     
					 (CPU_STK   * )&TASK3_TASK_STK[0],	
					 (CPU_STK_SIZE)TASK3_STK_SIZE/10,	
					 (CPU_STK_SIZE)TASK3_STK_SIZE,		
					 (OS_MSG_QTY  )0,					
					 (OS_TICK	  )0,					
					 (void   	* )0,					
					 (OS_OPT      )(OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR),
					 (OS_ERR 	* )&err);
					 
	OSTaskCreate((OS_TCB 	* )&Task4_TaskTCB,		
					 (CPU_CHAR	* )"Task4 task", 		
					 (OS_TASK_PTR )Sensor, 			
					 (void		* )0,					
					 (OS_PRIO	  )TASK4_TASK_PRIO,     
					 (CPU_STK   * )&TASK4_TASK_STK[0],	
					 (CPU_STK_SIZE)TASK4_STK_SIZE/10,	
					 (CPU_STK_SIZE)TASK4_STK_SIZE,		
					 (OS_MSG_QTY  )0,					
					 (OS_TICK	  )0,					
					 (void   	* )0,					
					 (OS_OPT      )(OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR),
					 (OS_ERR 	* )&err);
					 
	OS_CRITICAL_EXIT();	//退出临界区
	while(1)
	{
		LCD_ShowxNum(0,10,task_num,3,16,0x80);  //显示任务执行次数
		OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_HMSM_STRICT,&err); //延时1s
		task_num++;
	}
}
void GUI_task(void *p_arg)
{
	OS_ERR err;
	p_arg = p_arg;
	while(1)
	{
		tp_dev.scan(0);
		lv_task_handler();
		OSTimeDly(1, OS_OPT_TIME_DLY, &err);	
	}
}

void K210_data(void *p_arg)
{
	OS_ERR err;
	p_arg = p_arg;
	while(1)
	{	
		if(id_log)
		{
			NVID_DISABLE();
			if(lv_obj_get_child(lv_disp_get_scr_act(NULL),NULL)!=gauge_btn1)
			{
				LCD_ShowxNum(120,200,x,3,16,0x80);
				LCD_ShowxNum(120,220,y,3,16,0x80);
				LCD_ShowxNum(120,240,id,3,16,0x80);
			}
			id_log=0;
		}
		OSTimeDlyHMSM(0,0,0,50,OS_OPT_TIME_HMSM_STRICT,&err); //延时1s
	}
}

void arms_Monitoring(void *p_arg)
{
	OS_ERR err;
	p_arg = p_arg;
	int i=0;
	while(1)
	{
		if(sed_logo)
		{
			TIM_SetCompare4(TIM1,0);
			NVID_DISABLE();
			for(i=0;i<=sed_temp;i++)
			{
				usart6_send_char(usart_buff[i]);				
			}
			if(lv_obj_get_child(lv_disp_get_scr_act(NULL),NULL)!=gauge_btn1)
			{
				printf("不在\n");
				lv_led_off(led1);
			}
			while(!read_logo);
			TIM_SetCompare4(TIM1,150);
			if(lv_obj_get_child(lv_disp_get_scr_act(NULL),NULL)!=gauge_btn1)
			{				
				lv_led_on(led1);
				printf("不在\n");
			}
			NVID_Init();
			read_logo=0;
			sed_logo=0;
			sed_temp=0;
		}
		OSTimeDlyHMSM(0,0,0,50,OS_OPT_TIME_HMSM_STRICT,&err); //延时1s
	}
}

void Sensor(void *p_arg)
{
	OS_ERR err;
	p_arg = p_arg;
	float adcx,adc1,adc2;
	while(1)
	{
		if(lv_obj_get_child(lv_disp_get_scr_act(NULL),NULL)==gauge_btn1)
		{
			adcx=Get_Adc_Average(ADC_Channel_5,30);//获取通道5的转换值，20次取平均
			adcx=((float)adcx*5)/4095;          //获取计算后的带小数的实际电压值，比如3.1111
			
			adc1=Get_Adc_Average(ADC_Channel_4,30);//获取通道5的转换值，20次取平均
			adc1=((float)adc1*5)/4095;          //获取计算后的带小数的实际电压值，比如3.1111
			
			adc2=Get_Adc_Average(ADC_Channel_3,30);//获取通道5的转换值，20次取平均
			adc2=((float)adc2*5)/4095;          //获取计算后的带小数的实际电压值，比如3.1111
			
			adcx=adcx*20;
			if(adcx>100)adcx=100;
			adc1=adc1*20;
			if(adc1>100)adc1=100;	
			adc2=adc2*20;
			
			if(adc2>100)adc2=100;	
			
			lv_gauge_set_value(gauge1, 0, (int)adcx); /* 设置值 */
			lv_gauge_set_value(gauge2, 0,  (int)adc1); /* 设置值 */
			lv_gauge_set_value(gauge3, 0, (int)adc2); /* 设置值 */
		}
		
		OSTimeDlyHMSM(0,0,0,20,OS_OPT_TIME_HMSM_STRICT,&err); //延时1s		
	}
}



