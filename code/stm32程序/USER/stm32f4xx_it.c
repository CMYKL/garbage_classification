
#include "stm32f4xx_it.h"
#include "stdio.h"
#include "os_app_hooks.h"
#include "usart.h"

char buff[20]; 
int temp=0;

/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
	  
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}


u8 id_log=0;
int x=0,y=0,id=0;
uint8_t usart_buff[20];
int sed_temp=0;
u8 sed_logo=0;
u8 read_logo=0;

void USART3_IRQHandler(void)  
{
	OSIntEnter();
	int i=0,temp_x,temp_y,temp_id;
	if(USART_GetITStatus(USART3,USART_IT_RXNE)!=RESET)
	{
		buff[temp]=USART_ReceiveData(USART3);
		usart_buff[sed_temp]=buff[temp];
		if(usart_buff[sed_temp]=='C')
		{
			NVID_DISABLE();
//			for(num=0;num<20;num++)
//			{
//				usart_buff[num]=0x00;
//				buff[num]=0x00;				
//			}
			sed_logo=1;
		}
		else
		{
			sed_temp++;
			sed_logo=0;
		}
		if(sed_temp>20)sed_temp=0;
		if(buff[temp]=='A')
		{
			if(temp==1)
			{
				temp_x=((int)buff[0]-(int)('0'));
				buff[0]=0;
			}
			else
			{
				temp_x=((int)buff[0]-(int)('0'));
				buff[0]=0;
				
				for(i=1;i<temp;i++)
				{
					temp_x*=10;
					temp_x+=((int)buff[i]-(int)('0'));
					buff[i]=0;
				}
			}
			
			x=temp_x;
			temp_x=0;
			temp=0;
		}
		else 		
			if(buff[temp]=='B')
			{
				if(temp==1)
				{
					temp_y=((int)buff[0]-(int)('0'));
					buff[0]=0;
				}
				else
				{
					temp_y=((int)buff[0]-(int)('0'));
					buff[0]=0;
					
					for(i=1;i<temp;i++)
					{
						temp_y*=10;
						temp_y+=((int)buff[i]-(int)('0'));
						buff[i]=0;
					}
				}
				
				y=temp_y;
				temp_y=0;
				temp=0;
			}
			else
				if(buff[temp]=='C')
				{
					if(temp==1)
					{
						temp_id=((int)buff[0]-(int)('0'));
						buff[0]=0;
					}
					else
					{
						temp_id=((int)buff[0]-(int)('0'));
						buff[0]=0;
						
						for(i=1;i<temp;i++)
						{
							temp_id*=10;
							temp_id+=((int)buff[i]-(int)('0'));
							buff[i]=0;
						}
					}
					
					id=temp_id;
					temp_id=0;
					temp=0;
					id_log=1;
				}
				else
				{
					id_log=0;
					temp++;
				}
		if(temp>=20)temp=0;
		USART_ClearITPendingBit(USART3,USART_IT_RXNE);
	}
	OSIntExit();	
	
}
void USART6_IRQHandler(void)  
{
	char user_temp;
	OSIntEnter();
	if(USART_GetITStatus(USART6,USART_IT_RXNE)!=RESET)
	{
		user_temp=USART_ReceiveData(USART6);
		if(user_temp=='O')
		{
			read_logo=1;
		}
	}
	USART_ClearITPendingBit(USART6,USART_IT_RXNE);
	OSIntExit();
}

void USART1_IRQHandler(void)  
{
	
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
