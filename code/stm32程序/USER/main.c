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

//�������ȼ�
#define START_TASK_PRIO		3
//�����ջ��С	
#define START_STK_SIZE 		128*2
//������ƿ�
OS_TCB StartTaskTCB;
//�����ջ	
CPU_STK START_TASK_STK[START_STK_SIZE];
//������
void start_task(void *p_arg);

/*littleVGL����*/
#define TASK1_TASK_PRIO		(4)
#define TASK1_STK_SIZE 		1024*5
OS_TCB Task1_TaskTCB;	
CPU_STK TASK1_TASK_STK[TASK1_STK_SIZE];

/*k210���ݽ�������*/
#define TASK2_TASK_PRIO		(5)
#define TASK2_STK_SIZE 		1024*5
OS_TCB Task2_TaskTCB;
CPU_STK TASK2_TASK_STK[TASK2_STK_SIZE];

/*��е�����ݷ�����״̬�������*/
#define TASK3_TASK_PRIO		(6)
#define TASK3_STK_SIZE 		128*2
OS_TCB Task3_TaskTCB;
CPU_STK TASK3_TASK_STK[TASK3_STK_SIZE];

/*��е�����ݷ�����״̬�������*/
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
	
	delay_init(168);  								//ʱ�ӳ�ʼ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //�жϷ�������
	Tim1_user(270-1,168-1);							//���������ʼ��
	uart_init(115200);								//���ڳ�ʼ��
	LCD_Init();										//LCD��ʼ��
	TIM3_Int_Init(999,83);							//��ʱ����ʼ��(1ms�ж�),���ڸ�lvgl�ṩ1ms����������
	Adc_Init();										//����������ʼ��
	tp_dev.init();									//��������ʼ��
	lv_init();										//lvglϵͳ��ʼ��
	lv_port_disp_init();							//lvgl��ʾ�ӿڳ�ʼ��,����lv_init()�ĺ���
	lv_port_indev_init();							//lvgl����ӿڳ�ʼ��,����lv_init()�ĺ���
	OSInit(&err);		    						//��ʼ��UCOSIII
	OS_CRITICAL_ENTER();							//�����ٽ���			 
	//������ʼ����
	OSTaskCreate((OS_TCB 	* )&StartTaskTCB,		//������ƿ�
				 (CPU_CHAR	* )"start task", 		//��������
                 (OS_TASK_PTR )start_task, 			//������
                 (void		* )0,					//���ݸ��������Ĳ���
                 (OS_PRIO	  )START_TASK_PRIO,     //�������ȼ�
                 (CPU_STK   * )&START_TASK_STK[0],	//�����ջ����ַ
                 (CPU_STK_SIZE)START_STK_SIZE/10,	//�����ջ�����λ
                 (CPU_STK_SIZE)START_STK_SIZE,		//�����ջ��С
                 (OS_MSG_QTY  )0,					//�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
                 (OS_TICK	  )0,					//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
                 (void   	* )0,					//�û�����Ĵ洢��
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //����ѡ��
                 (OS_ERR 	* )&err);				//��Ÿú�������ʱ�ķ���ֵ
	OS_CRITICAL_EXIT();	//�˳��ٽ���	 
	OSStart(&err);      //����UCOSIII
	
}

//��ʼ����������
void start_task(void *p_arg)
{
	int task_num=0;
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;
	CPU_Init();	
	OS_CRITICAL_ENTER();	//�����ٽ���
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
					 
	OS_CRITICAL_EXIT();	//�˳��ٽ���
	while(1)
	{
		LCD_ShowxNum(0,10,task_num,3,16,0x80);  //��ʾ����ִ�д���
		OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ1s
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
		OSTimeDlyHMSM(0,0,0,50,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ1s
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
				printf("����\n");
				lv_led_off(led1);
			}
			while(!read_logo);
			TIM_SetCompare4(TIM1,150);
			if(lv_obj_get_child(lv_disp_get_scr_act(NULL),NULL)!=gauge_btn1)
			{				
				lv_led_on(led1);
				printf("����\n");
			}
			NVID_Init();
			read_logo=0;
			sed_logo=0;
			sed_temp=0;
		}
		OSTimeDlyHMSM(0,0,0,50,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ1s
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
			adcx=Get_Adc_Average(ADC_Channel_5,30);//��ȡͨ��5��ת��ֵ��20��ȡƽ��
			adcx=((float)adcx*5)/4095;          //��ȡ�����Ĵ�С����ʵ�ʵ�ѹֵ������3.1111
			
			adc1=Get_Adc_Average(ADC_Channel_4,30);//��ȡͨ��5��ת��ֵ��20��ȡƽ��
			adc1=((float)adc1*5)/4095;          //��ȡ�����Ĵ�С����ʵ�ʵ�ѹֵ������3.1111
			
			adc2=Get_Adc_Average(ADC_Channel_3,30);//��ȡͨ��5��ת��ֵ��20��ȡƽ��
			adc2=((float)adc2*5)/4095;          //��ȡ�����Ĵ�С����ʵ�ʵ�ѹֵ������3.1111
			
			adcx=adcx*20;
			if(adcx>100)adcx=100;
			adc1=adc1*20;
			if(adc1>100)adc1=100;	
			adc2=adc2*20;
			
			if(adc2>100)adc2=100;	
			
			lv_gauge_set_value(gauge1, 0, (int)adcx); /* ����ֵ */
			lv_gauge_set_value(gauge2, 0,  (int)adc1); /* ����ֵ */
			lv_gauge_set_value(gauge3, 0, (int)adc2); /* ����ֵ */
		}
		
		OSTimeDlyHMSM(0,0,0,20,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ1s		
	}
}



