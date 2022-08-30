#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "sys.h" 
  
#define USART_REC_LEN  			200  	//�����������ֽ��� 200
#define EN_USART1_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����
	  
	  
extern u8  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART_RX_STA;         		//����״̬���
void Data_Send_Status(float Pitch,float Roll,float Yaw);
void ANO_DT_Send_Senser(s16 a_x,s16 a_y,s16 a_z,s16 g_x,s16 g_y,s16 g_z,s16 m_x,s16 m_y,s16 m_z);
void uart_init(u32 bound);
void ANO_DT_Send_Status(float angle_rol, float angle_pit, float angle_yaw, s32 alt, u8 fly_model, u8 armed);

void Check(uint8_t SC_GET,uint8_t AC_GET);
void usart_send_char(uint8_t c);
void Postage(s16 pitch,s16 Roll,s16 yaw);
void Analysis(uint8_t buff[]);
void PID_Settings(uint8_t buff[]);
void set_data(int data);
void NVID_Init(void);
void usart6_send_char(uint8_t c);
void NVID_DISABLE(void);
#endif


