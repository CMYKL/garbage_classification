#include "Sensor.h"

void user_sensor_init()
{
	GPIO_InitTypeDef sensor;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	
	sensor.GPIO_Mode=GPIO_Mode_IN;
	sensor.GPIO_Pin=GPIO_Pin_0;
	sensor.GPIO_PuPd=GPIO_PuPd_NOPULL;	
	GPIO_Init(GPIOB,&sensor);	
	
}


void read_sensor_data(u8 data1,u8 data2,u8 data3)
{
	data1=GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0);
	data2=GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0);
	data3=GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0);	
}
