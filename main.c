/**
  ******************************************************************************
  * @file    SysTick/main.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    19-September-2011
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************  
  */ 

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup STM32F4_Discovery_Peripheral_Examples
  * @{
  */

/** @addtogroup SysTick_Example
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment = 4   
  #endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */
__ALIGN_BEGIN USB_OTG_CORE_HANDLE  USB_OTG_dev __ALIGN_END;

int32_t Buffer[3];
__IO uint32_t TimingDelay = 0;
LIS302DL_InitTypeDef  LIS302DL_InitStruct;
LIS302DL_InterruptConfigTypeDef LIS302DL_InterruptStruct;
/* Private function prototypes -----------------------------------------------*/
void cortexm4f_enable_fpu(void);


/* Private functions ---------------------------------------------------------*/

/**
  * @brief   Main program
  * @param  None
  * @retval None
  */
int main(void)
{
	RCC_ClocksTypeDef RCC_Clocks;
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f4xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
        system_stm32f4xx.c file
     */     
       
  /* Initialize Leds mounted on STM32F4-Discovery board */
  STM_EVAL_LEDInit(LED4);
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED5);
  STM_EVAL_LEDInit(LED6);

	RCC_GetClocksFreq(&RCC_Clocks);
  SysTick_Config(SystemCoreClock / 1000);
  
  cortexm4f_enable_fpu();
		
	
	/* USB configuration */
  USBD_Init(&USB_OTG_dev,
        USB_OTG_FS_CORE_ID,
        &USR_desc, 
        &USBD_CDC_cb, 
        &USR_cb);
        
   /* Set configuration of LIS302DL*/

  LIS302DL_InitStruct.Power_Mode = LIS302DL_LOWPOWERMODE_ACTIVE;
  LIS302DL_InitStruct.Output_DataRate = LIS302DL_DATARATE_100;
  LIS302DL_InitStruct.Axes_Enable = LIS302DL_X_ENABLE | LIS302DL_Y_ENABLE | LIS302DL_Z_ENABLE;
  LIS302DL_InitStruct.Full_Scale = LIS302DL_FULLSCALE_2_3;
  LIS302DL_InitStruct.Self_Test = LIS302DL_SELFTEST_NORMAL;
  LIS302DL_Init(&LIS302DL_InitStruct);

    /* Set configuration of Internal High Pass Filter of LIS302DL*/

  LIS302DL_InterruptStruct.Latch_Request = LIS302DL_INTERRUPTREQUEST_NOTLATCHED;
  LIS302DL_InterruptStruct.SingleClick_Axes = LIS302DL_CLICKINTERRUPT_XYZ_DISABLE;
  LIS302DL_InterruptStruct.DoubleClick_Axes = LIS302DL_DOUBLECLICKINTERRUPT_XYZ_DISABLE;
  LIS302DL_InterruptConfig(&LIS302DL_InterruptStruct);
  
  Delay(30);
    
  while (1)
  {    
    Delay(10);
    LIS302DL_ReadACC(Buffer);
    printf("%d %d %d\r\n", Buffer[0], Buffer[1], Buffer[2]);
  }
}



void cortexm4f_enable_fpu() {
    /* set CP10 and CP11 Full Access */
    SCB->CPACR |= ((3UL << 10*2)|(3UL << 11*2));
}

void Delay(__IO uint32_t nTime)
{ 
  TimingDelay = nTime;

  while(TimingDelay != 0);
}

void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  { 
    TimingDelay--;
  }
}

uint32_t LIS302DL_TIMEOUT_UserCallback(void)
{
  /* MEMS Accelerometer Timeout error occured */
  while (1)
  {   
  }
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */ 

/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
