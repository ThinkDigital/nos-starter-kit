/*
 * hardware.cpp
 *
 *  Created on: Nov 30, 2018
 *      Author: jsilva
 */

#include "hardware.h"


#define HARDWARE_TYPE_STM32L0_NOSKIT

extern UART_HandleTypeDef huart4; // debug

//ADC_HandleTypeDef hadc;  // uncomment
/*
I2C_HandleTypeDef hi2c1;
I2C_HandleTypeDef hi2c2;

SPI_HandleTypeDef hspi1;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart4;
*/
// ADC channel configuration structure declaration
//ADC_ChannelConfTypeDef        sConfig;  // uncomment
// Variable used to get converted value
__IO uint32_t uwADCxConvertedValue = 0;

#if defined HARDWARE_TYPE_STM32L0_TESTBOARD

	const uint8_t HARDWARE_GPIO_COUNT                  = 4;
  const uint8_t HARDWARE_GPIO_ADDRESS[4]             = { 0x01, 0x02, 0x03, 0x04 };
	GPIO_TypeDef* HARDWARE_RS485_PIN_TYPE             = GPIOC;
	const uint16_t HARDWARE_RS485_PIN_NUMBER          = GPIO_PIN_6;
	const bool    HARDWARE_RS485_PIN_LOGIC            = false;

#elif defined HARDWARE_TYPE_STM32L0_NOSKIT

  const uint8_t HARDWARE_GPIO_COUNT                  = 4;
  const uint8_t HARDWARE_GPIO_ADDRESS[4]             = { 0x01, 0x02, 0x03, 0x04 };
  GPIO_TypeDef* HARDWARE_RS485_PIN_TYPE              = GPIOA;
  const uint16_t HARDWARE_RS485_PIN_NUMBER           = GPIO_PIN_15;
  const bool    HARDWARE_RS485_PIN_LOGIC             = false;
  const uint16_t HARDWARE_PERIPHERALS_PIN_NUMBER     = GPIO_PIN_4;
#endif

Serial *serial_rs485 = NULL;
//Serial serial_rs485;

void hardware_init() {

  hardware_rs485_init();

  config_adc();

  update_gpios();

}

void config_adc(){
  /*
  hadc.Instance = ADC1;

  hadc.Init.OversamplingMode      = DISABLE;

  hadc.Init.ClockPrescaler        = ADC_CLOCK_SYNC_PCLK_DIV1;
  hadc.Init.LowPowerAutoPowerOff  = DISABLE;
  hadc.Init.LowPowerFrequencyMode = ENABLE;
  hadc.Init.LowPowerAutoWait      = DISABLE;

  hadc.Init.Resolution            = ADC_RESOLUTION_12B;
  hadc.Init.SamplingTime          = ADC_SAMPLETIME_7CYCLES_5;
  hadc.Init.ScanConvMode          = ADC_SCAN_DIRECTION_FORWARD;
  hadc.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
  hadc.Init.ContinuousConvMode    = ENABLE;
  hadc.Init.DiscontinuousConvMode = DISABLE;
  hadc.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc.Init.EOCSelection          = ADC_EOC_SINGLE_CONV;
  hadc.Init.DMAContinuousRequests = DISABLE;
  */
}

void update_gpios(){
  // read internal Table
  /*
  for (uint8_t i = 0; i < N_GPIOS; i++) {
		// change modes
	}
  */
}

void hardware_en_peripherals(){

  #ifdef HARDWARE_TYPE_STM32L0_NOSKIT
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET);
		HAL_Delay(4000);
  #endif

}

void hardware_dis_peripherals(){

  #ifdef HARDWARE_TYPE_STM32L0_NOSKIT
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_RESET);
  #endif
}

bool set_digital_output(uint8_t index, bool state) {
	if (index > HARDWARE_GPIO_COUNT) {
		return false;
	}
	//digitalWrite(HARDWARE_RELAY_ADDRESS[index], state ? HIGH : LOW);

  // check if port is in output mode
  switch(index){
    case 1:
      if(state)
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET);
      else
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_RESET);
      break;
    case 2:
      if(state)
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET);
      else
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_RESET);
      break;
    case 3:
      if(state)
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);
      else
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET);
      break;
    case 4:
      if(state)
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET);
      else
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);
      break;
    }
	return true;
}

bool read_digital_input(uint8_t index, uint8_t* value) {

  if (index > HARDWARE_GPIO_COUNT) {
		return false;
	}
  // check if port is in input mode
  switch(index){
    case 1:
      *value = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_0);
      break;
    case 2:
      *value = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_1);
      break;
    case 3:
      *value = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_2);
      break;
    case 4:
      *value = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_3);
			logging.println("hardware","val GPIO_PIN_3 ",(int)*value);
      break;
    }
	return true;
}

bool read_analog_input(uint8_t index, uint16_t* value){

	//MX_ADC_Init();

	ADC_ChannelConfTypeDef sConfig;

  // ### - 3 - Channel configuration ########################################
  // Select Channel 0 to be converted
  //sConfig.Channel = ADC_CHANNEL_0;

  /**Configure for the selected ADC regular channel to be converted.
  */
  #ifdef HARDWARE_TYPE_STM32L0_NOSKIT_FIX_IT
    sConfig.Channel = index;
    sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;
    if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
    {
      _Error_Handler(__FILE__, __LINE__);
    }
  #endif

  #ifdef HARDWARE_TYPE_STM32L0_TESTBOARD
    sConfig.Channel = ADC_CHANNEL_8;
    sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;
    if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
    {
      _Error_Handler(__FILE__, __LINE__);
    }
		/*
    sConfig.Channel = ADC_CHANNEL_9;
    sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;
    if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
    {
      _Error_Handler(__FILE__, __LINE__);
    }
		*/
  #endif

  //##- 4- Start the conversion process #######################################

  /*
  if (HAL_ADC_Start(&hadc) != HAL_OK)
  {
    // Start Conversation Error
    Error_Handler();
  }
*/

  //bool read = false;
    /* uncomment
  uint32_t timeout = millis()+3000;
  while(!read && timeout > millis()){
	  HAL_ADC_PollForConversion(&hadc, 10);

	  // Check if the continous conversion of regular channel is finished
    if ((HAL_ADC_GetState(&hadc) & HAL_ADC_STATE_REG_EOC) == HAL_ADC_STATE_REG_EOC)
    {
      //##-6- Get the converted value of regular channel  ########################
      uwADCxConvertedValue = HAL_ADC_GetValue(&hadc);
      read = true;
    }
  }
  *value = (uint16_t)uwADCxConvertedValue;
*/

	//MX_ADC_DeInit();

	return true;
}

int16_t validate_analog_input(uint8_t index, uint16_t value) {
  return 0;
 }


void set_rgb_colour(uint8_t colour){

	#ifdef HARDWARE_TYPE_STM32L0_TESTBOARD
		if(colour != NONE)
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6,GPIO_PIN_SET);
		else
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6,GPIO_PIN_SET);
  #elif defined HARDWARE_TYPE_STM32L0_NOSKIT
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_RESET);

		if(colour == GREEN)
		 HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_SET);
		else if(colour == RED)
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, GPIO_PIN_SET);
		else if(colour == BLUE)
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_SET);
	#endif

	return;
}




void hardware_rs485_init() {

 serial_rs485 = new Serial();

 #ifdef logging_HARDWARE
 	logging.log("init","rs485");
 #endif

 HAL_GPIO_WritePin(HARDWARE_RS485_PIN_TYPE,HARDWARE_RS485_PIN_NUMBER,HARDWARE_RS485_PIN_LOGIC ? GPIO_PIN_RESET : GPIO_PIN_SET);

}

void hardware_rs485_write(uint8_t data[], uint8_t len) {

	HAL_GPIO_WritePin(HARDWARE_RS485_PIN_TYPE,HARDWARE_RS485_PIN_NUMBER,HARDWARE_RS485_PIN_LOGIC ? GPIO_PIN_SET : GPIO_PIN_RESET);
	HAL_Delay(1);

/*
	uint16_t crc = ModbusRTU::crc(data, len - 2);

	data[len - 2] = (uint8_t) crc;
	data[len - 1] = (uint8_t) (crc >> 8);
*/

	#ifdef logging_HARDWARE
	logging.log_nnl("[rs485] writing: ","rs485");
	  uint8_t i = 0;
	  while(i < len){
	   sprintf(logging.log_msg,"%x ", data[i]);
	   logging.log_nnl(logging.log_msg,"rs485");
	   i++;
	  }
		logging.log_nnl("\n","rs485");logging.log_nnl(logging.log_msg,"rs485");
	#endif

	serial_rs485->clear();
	serial_rs485->write(data, len);
	//serial_rs485->flush();

	//HAL_Delay(1);
	HAL_GPIO_WritePin(HARDWARE_RS485_PIN_TYPE,HARDWARE_RS485_PIN_NUMBER,HARDWARE_RS485_PIN_LOGIC ? GPIO_PIN_RESET : GPIO_PIN_SET);
}

uint8_t hardware_rs485_read(uint8_t data[], uint8_t len) {

	uint8_t e = 0;

	#ifdef logging_HARDWARE
		logging.log_nnl("[rs485] reading: ","rs485");
	#endif

	uint8_t r = 0, i = 0;
	for (i = 0; i < len; i++) {
		if(!hardware_rs485_read_byte(&r))
			return i;

		data[i] = r;
		#ifdef logging_HARDWARE
			sprintf(logging.log_msg,"%x ", r);
	    logging.log_nnl(logging.log_msg,"rs485");
		#endif

		/*
		if (r == -1) {
			if (++e >= 5) return n;
		} else {
			e = 0;
			data[n++] = r;

			if (n >= len) return n;
		}
		*/
	}
	logging.log_nnl("\n","rs485");
  if(i == 0)
    logging.log("no data available","rs485");

  return i;
}

bool hardware_rs485_read_byte(uint8_t *byte) {
  uint8_t tries = HARDWARE_RS485_READ_TRIES;

	while (!serial_rs485->read_byte(byte)) {
		HAL_Delay(HARDWARE_RS485_READ_WAIT);
		if (--tries == 0) return false;
	}

	return true;
}

