
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32l0xx_hal.h"


#include <string.h>
#include <strings.h> // to use string import this
#include <timestamp.h>

#include "package.h"
#include "main_aux.h"
#include "network.h"
#include "hardware.h"
#include "logging.h"
#include "hardware.h"
//#include "sensors.h"

#include <strstream> // to use string import this
using namespace std;


/* Private variables ---------------------------------------------------------*/
RTC_HandleTypeDef hrtc;
RTC_HandleTypeDef RTCHandle;

extern LOGGING logging;
extern UART_HandleTypeDef huart2;


/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_RTC_Init(void);
static void MX_NVIC_Init(void);
void RTC_TimeStampConfig(void);
void SystemPower_Config(void);
void enterStopMode();
void RTC_CalendarShow(void);
#ifdef DEBUG_RAM
  void free_ram(void);
#endif

NETWORK network;

//SENSORS sensor;

/**
  * @brief  The application entry point.
  *
  * @retval None
  */
int main(void)
{
  if(__HAL_PWR_GET_FLAG(PWR_FLAG_SB) != RESET)
    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_SB); // Clear Standby flag


  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  SystemPower_Config();

  RTC_TimeStampConfig();

  /* Initialize all configured peripherals */

  MX_RTC_Init();

  MX_GPIO_Init();

  MX_USART1_UART_Init();  // modem

  MX_USART2_UART_Init();  // log

  MX_USART4_UART_Init();  // rs485

  MX_I2C2_Init();

  MX_SPI1_Init();

  MX_I2C1_Init();

  /* Initialize interrupts */
  MX_NVIC_Init();

  logging.init((uint32_t)115200);

  printf("\nBooting \n");

  // init hadware, call it only on power cycle
  hardware_init();

  // call this only when it's need to connect to server
  network.init();

  //sensor.init();

  logging.println((char*)"main",(char*)"timestamp: ",(long)getTimestamp());
  }

  while (1)
  {
    #ifdef DEBUG_RAM
      free_ram();
    #endif

    set_rgb_colour(BLUE);

    if(settings.modem.active){
      network.loop();  // check if messages were received
    }

    RTC_CalendarShow();

    logging.println((char*)"main",(char*)"timestamp: ",(long)getTimestamp());

  	if(sleep){
      set_rgb_colour(NONE);
      printf("\nentering sleep mode \n");
      enterStopMode();
    }else{
      HAL_Delay(1000); // delay is now implement by StopMode
      set_rgb_colour(NONE);
      HAL_Delay(1000); // delay is now implement by StopMode
    }

  }

}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

    /**Configure the main internal regulator output voltage
    */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /**Initializes the CPU, AHB and APB busses clocks
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_5;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_USART2
                              |RCC_PERIPHCLK_I2C1|RCC_PERIPHCLK_RTC;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/**
  * @brief NVIC Configuration.
  * @retval None
  */
static void MX_NVIC_Init(void)
{
  /* USART1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USART2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(USART2_IRQn);
  /* USART4_5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(USART4_5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(USART4_5_IRQn);
  /* RTC_IRQn interrupt configuration */
  /*
  HAL_NVIC_SetPriority(RTC_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(RTC_IRQn);
  */
}


void RTC_TimeStampConfig(void)
{
  RTC_DateTypeDef sdatestructure;
  RTC_TimeTypeDef stimestructure;

  /*##-1- Configure the Time Stamp peripheral ################################*/
  /*  RTC TimeStamp generation: TimeStamp Rising Edge on PC.13 Pin */
  //HAL_RTCEx_SetTimeStamp_IT(&RtcHandle, RTC_TIMESTAMPEDGE_RISING, RTC_TIMESTAMPPIN_DEFAULT);

  /*##-2- Configure the Date #################################################*/
  /* Set Date: Monday April 14th 2014 */
  sdatestructure.Year    = 0x19;
  sdatestructure.Month   = RTC_MONTH_MARCH;
  sdatestructure.Date    = 0x13;
  sdatestructure.WeekDay = RTC_WEEKDAY_MONDAY;

  if(HAL_RTC_SetDate(&RTCHandle,&sdatestructure,RTC_FORMAT_BCD) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /*##-3- Configure the Time #################################################*/
  /* Set Time: 08:10:00 */
  stimestructure.Hours          = 0x00;
  stimestructure.Minutes        = 0x00;
  stimestructure.Seconds        = 0x00;
  stimestructure.SubSeconds     = 0x00;
  stimestructure.TimeFormat     = RTC_HOURFORMAT12_AM;
  stimestructure.DayLightSaving = RTC_DAYLIGHTSAVING_NONE ;
  stimestructure.StoreOperation = RTC_STOREOPERATION_RESET;

  if(HAL_RTC_SetTime(&RTCHandle,&stimestructure,RTC_FORMAT_BCD) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
}


/* RTC init function */
void MX_RTC_Init(void)
{

    /**Initialize RTC Only
    */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutRemap = RTC_OUTPUT_REMAP_NONE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* USER CODE BEGIN 4 */


void enterStopMode(){


  //HAL_MspDeInit();
  //MX_GPIO_Low_Consumption();
  MX_USART2_UART_DeInit();

  HAL_PWREx_EnableUltraLowPower();


	/* The Following Wakeup sequence is highly recommended prior to each Standby mode entry
	  mainly  when using more than one wakeup source this is to not miss any wakeup event.
	  - Disable all used wakeup sources,
	  - Clear all related wakeup flags,
	  - Re-enable all used wakeup sources,
	  - Enter the Standby mode.
	*/

  /*Suspend Tick increment to prevent wakeup by Systick interrupt.
  Otherwise the Systick interrupt will wake up the device within 1ms (HAL time base)*/
  HAL_SuspendTick();

	/* Disable all used wakeup sources*/
	HAL_RTCEx_DeactivateWakeUpTimer(&RTCHandle);

	/* Re-enable all used wakeup sources*/
	/* ## Setting the Wake up time ############################################*/
	/*  RTC Wakeup Interrupt Generation:
	  Wakeup Time Base = (RTC_WAKEUPCLOCK_RTCCLK_DIV /(LSI))
	  Wakeup Time = Wakeup Time Base * WakeUpCounter
		= (RTC_WAKEUPCLOCK_RTCCLK_DIV /(LSI)) * WakeUpCounter
		==> WakeUpCounter = Wakeup Time / Wakeup Time Base

	  To configure the wake up timer to 4s the WakeUpCounter is set to 0x1FFF:
	  RTC_WAKEUPCLOCK_RTCCLK_DIV = RTCCLK_Div16 = 16
	  Wakeup Time Base = 16 /(~39.000KHz) = ~0,410 ms
	  Wakeup Time = ~4s = 0,410ms  * WakeUpCounter
		==> WakeUpCounter = ~4s/0,410ms = 9750 = 0x2616 */
	HAL_RTCEx_SetWakeUpTimer_IT(&RTCHandle, 0x2616, RTC_WAKEUPCLOCK_RTCCLK_DIV16);

	/* Clear all related wakeup flags */

  //__HAL_RCC_PWR_CLK_SLEEP_DISABLE();

  /* Enable the power down mode during Sleep mode */
  __HAL_FLASH_SLEEP_POWERDOWN_ENABLE();

	__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
	/* Enter the Standby mode */
	//HAL_PWR_EnterSTANDBYMode();
	HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
	//HAL_PWR_EnterSLEEPMode(PWR_LOWPOWERREGULATOR_ON, PWR_SLEEPENTRY_WFI);

  __HAL_RCC_PWR_CLK_SLEEP_ENABLE();
  /* Resume Tick interrupt if disabled prior to sleep mode entry*/
  HAL_PWREx_DisableLowPowerRunMode();

  HAL_ResumeTick();


  //MX_GPIO_Enable_Clock();
  MX_USART2_UART_Init();
  //HAL_MspInit();

  //MX_GPIO_Init();
}


/**
  * @brief  System Power Configuration
  *         The system Power is configured as follow :
  *            + RTC Clocked by LSI
  *            + VREFINT OFF, with fast wakeup enabled
  *            + No IWDG
  *            + Automatic Wakeup using RTC clocked by LSI (after ~4s)
  * @param  None
  * @retval None
  */
void SystemPower_Config(void)
{
  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();

  /* Enable access to RTC domain */
  //HAL_PWR_EnableBkUpAccess();

  HAL_PWREx_EnableUltraLowPower();

  /* Enable the fast wake up from Ultra low power mode */
  HAL_PWREx_EnableFastWakeUp();

  // Configure RTC
  RTCHandle.Instance = RTC;
  // Set the RTC time base to 1s
  /* Configure RTC prescaler and RTC data registers as follow:
    - Hour Format = Format 24
    - Asynch Prediv = Value according to source clock
    - Synch Prediv = Value according to source clock
    - OutPut = Output Disable
    - OutPutPolarity = High Polarity
    - OutPutType = Open Drain */
  RTCHandle.Init.HourFormat = RTC_HOURFORMAT_24;
  /*
  RTCHandle.Init.AsynchPrediv = RTC_ASYNCH_PREDIV;
  RTCHandle.Init.SynchPrediv = RTC_SYNCH_PREDIV;
  */
  RTCHandle.Init.OutPut = RTC_OUTPUT_DISABLE;
  RTCHandle.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  RTCHandle.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if(HAL_RTC_Init(&RTCHandle) != HAL_OK)
  {
    // Initialization Error
    Error_Handler();
  }

}


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  //printf("Interrupt IO: %d",GPIO_Pin);
  if (GPIO_Pin == GPIO_PIN_0){
    logging.log((char*)"INTERRUPT",(char*)"GPIO0");
	   sensor.increment_counter(GPIO1_INDEX);
  }else if (GPIO_Pin == GPIO_PIN_1){
    logging.log((char*)"INTERRUPT",(char*)"GPIO1");
    sensor.increment_counter(GPIO2_INDEX);
  }
}


// Is this needed?
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle) {
	/* Set transmission flag: transfer complete */

}


void RTC_CalendarShow(void)
{
  RTC_DateTypeDef sdatestructureget;
  RTC_TimeTypeDef stimestructureget;

  // Get the RTC current Time
  HAL_RTC_GetTime(&hrtc, &stimestructureget, RTC_FORMAT_BIN);
  // Get the RTC current Date
  HAL_RTC_GetDate(&hrtc, &sdatestructureget, RTC_FORMAT_BIN);

  char* show_time = (char*)malloc(256);
  if(show_time != NULL){
    memset(show_time,0,256);
    // Display date Format : mm-dd-yy
    sprintf((char*)show_time,"Timestamp: %.2d-%.2d-%.4d ", sdatestructureget.Month, sdatestructureget.Date, 2000+sdatestructureget.Year);
    //printf("Timestamp: %s  ",show_time);
    HAL_UART_Transmit(&huart2, (uint8_t*) show_time, 256, 100);

    memset(show_time,0,256);
    // Display time Format : hh:mm:ss
    sprintf((char*)show_time,"%.2d:%.2d:%.2d \n", stimestructureget.Hours, stimestructureget.Minutes, stimestructureget.Seconds);
    //printf("%s \n",show_time);
    HAL_UART_Transmit(&huart2, (uint8_t*) show_time, 256, 100);
    free(show_time);
  }

}

#ifdef DEBUG_RAM
void free_ram(void){
  logging.log("checking ram","main");
  uint8_t i = 0;
  size_t* ptr_data = nullptr;
  while(i<500){
    ptr_data = (size_t*)malloc(i*256);
    if(ptr_data != nullptr){
      /*
      sprintf(logging.log_msg,"i: %x, ptr: %x",i,ptr_data);
      logging.log(logging.log_msg,"main");
      */
      //arr[i] = ptr_data;
      free(ptr_data);
    }else
      break;
    i++;
  }
  sprintf(logging.log_msg,"free ram: %lu",i*256);
  logging.log(logging.log_msg,"main");
}
#endif

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  sprintf(logging.log_msg,"error on file: %s at line: %d", file, line);
  logging.log(logging.log_msg, "main");
  while(1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
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
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
