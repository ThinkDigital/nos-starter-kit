/*
 * hardware.cpp
 *
 *  Created on: Nov 30, 2018
 *      Author: jsilva
 */

#include "hardware.h"

extern UART_HandleTypeDef huart4; // debug
extern Table table;

ADC_HandleTypeDef hadc;

I2C_HandleTypeDef hi2c1;
I2C_HandleTypeDef hi2c2;

SPI_HandleTypeDef hspi1;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart4;

// ADC channel configuration structure declaration
ADC_ChannelConfTypeDef        sConfig;
// Variable used to get converted value
__IO uint32_t uwADCxConvertedValue = 0;

const uint8_t HARDWARE_GPIO_COUNT                  = 4;
const uint8_t HARDWARE_GPIO_ADDRESS[4]             = { 0x01, 0x02, 0x03, 0x04 };
GPIO_TypeDef* HARDWARE_RS485_PIN_TYPE              = GPIOA;
const uint16_t HARDWARE_RS485_PIN_NUMBER           = GPIO_PIN_15;
const bool    HARDWARE_RS485_PIN_LOGIC             = false;
const uint16_t HARDWARE_PERIPHERALS_PIN_NUMBER     = GPIO_PIN_4;


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
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET);
	HAL_Delay(4000);
}

void hardware_dis_peripherals(){
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_RESET);
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

	MX_ADC_Init();

  // ### - 3 - Channel configuration ########################################
  // Select Channel 0 to be converted
  //sConfig.Channel = ADC_CHANNEL_0;

  /**Configure for the selected ADC regular channel to be converted.
  */
  #ifdef HARDWARE_TYPE_STM32L0_NOSKIT
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
  if (HAL_ADC_Start(&hadc) != HAL_OK)
  {
    // Start Conversation Error
    Error_Handler();
  }

  bool read = false;
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

	MX_ADC_DeInit();

	return true;
}

int16_t validate_analog_input(uint8_t index, uint16_t value) {
  return 0;
 }


void set_rgb_colour(uint8_t colour){

	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_RESET);

	if(colour == GREEN)
	 HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_SET);
	else if(colour == RED)
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, GPIO_PIN_SET);
	else if(colour == BLUE)
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_SET);

	return;
}


void hardware_rs485_init() {
	if (!settings.rs485.active) return;

 serial_rs485 = new Serial();

 #ifdef logging_HARDWARE
 	logging.log("init","rs485");
 #endif

 	#ifdef logging_HARDWARE
 		logging.println("rs485",(char*)"initting port at ", (long)settings.rs485.baudrate);
 	#endif

 HAL_GPIO_WritePin(HARDWARE_RS485_PIN_TYPE,HARDWARE_RS485_PIN_NUMBER,HARDWARE_RS485_PIN_LOGIC ? GPIO_PIN_RESET : GPIO_PIN_SET);
 serial_rs485->begin(4,settings.rs485.baudrate);
}

void hardware_rs485_write(uint8_t data[], uint8_t len) {
	if (!settings.rs485.active) return;


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
	if (!settings.rs485.active) return 0;

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


bool read_i2c(){
	//Sensor* s

	/*##-5- Master sends read request for slave ##############################*/
	uint8_t id = (uint8_t)(s->address>>24);
	hi2c1.Init.OwnAddress1 = id;	// slave id

	/* Update bTransferRequest to send buffer read request for Slave */
	//uint8_t bTransferRequest = MASTER_REQ_READ;
	uint8_t bTransferRequest[1];
	bTransferRequest[0] = (uint8_t)s->address;

	uint8_t res = 0;

	id = id<<1;
	if(id != 0 && s->address != 0){

		#ifdef DEBUG_I2C
			sprintf(logging.log_msg,"i2c read >> id: 0x%x bTransferRequest: 0x%x \n",id,bTransferRequest[0]);
			logging.log_nnl(logging.log_msg,"sensors");
		#endif

		do
		{
			res = HAL_I2C_Master_Transmit(&hi2c1, (uint8_t)id, (uint8_t*)&bTransferRequest, 1,100);
			if( res != HAL_OK)
			{
				#ifdef DEBUG_I2C
					sprintf(logging.log_msg,"!! error i2c >> transmitting data... \n");
					logging.log_nnl(logging.log_msg,"sensors");
				#endif
				//Error_Handler();
				return false;
			}

			while (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY)
			{
			}
		}while(HAL_I2C_GetError(&hi2c1) == HAL_I2C_ERROR_AF);
	}

	/*##-7- Master receives aRxBuffer from slave #############################*/
	do
	{
		GPIO_InitTypeDef GPIO_InitStruct;
		//Configure GPIO pin Output Level /
    GPIO_InitStruct.Pin = GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
		GPIO_InitStruct.Pin = GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6,GPIO_PIN_RESET);

		while(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7) == GPIO_PIN_SET);

		MX_I2C1_Init();
		uint8_t* data = (uint8_t*)malloc(256);
		memset(data,0,256);
		if(data == nullptr)
			return false;

		#ifdef DEBUG_I2C
			sprintf(logging.log_msg,"i2c read >> requesting data... \n");
			logging.log_nnl(logging.log_msg,"sensors");
		#endif

		//if(HAL_I2C_Master_Receive(&hi2c1,id,data,s->len,100)!=HAL_OK){
		if(HAL_I2C_Master_Receive(&hi2c1, (uint8_t)id, (uint8_t*)data, s->len,2000)!= HAL_OK){
			/* Error_Handler() function is called when error occurs. */
			//Error_Handler();
			sprintf(logging.log_msg,"!! error i2c >> res: 0x%x \n",res);
			logging.log_nnl(logging.log_msg,"sensors");

			#ifdef DEBUG_I2C
				sprintf(logging.log_msg,"i2c read >> data received: ");
				logging.log_nnl(logging.log_msg,"sensors");
				uint8_t i = 0;
				while(i<s->len){
					if(i == s->len-1){
						sprintf(logging.log_msg," 0x%x \n",data[i]);
						logging.log_nnl(logging.log_msg,"sensors");
					}else{
						sprintf(logging.log_msg," 0x%x ",data[i]);
						logging.log_nnl(logging.log_msg,"sensors");
					}
					i++;
				}
			#endif
			return false;
		}
		uint8_t i = 0;
		#ifdef DEBUG_I2C
			i = 0;
			sprintf(logging.log_msg,"i2c read >> data received: ");
			logging.log_nnl(logging.log_msg,"sensors");

			while(i<s->len){
				if(i == s->len-1){
					sprintf(logging.log_msg," 0x%x \n",data[i]);
					logging.log_nnl(logging.log_msg,"sensors");
				}else{
					sprintf(logging.log_msg," 0x%x ",data[i]);
					logging.log_nnl(logging.log_msg,"sensors");
				}
				i++;
			}
		#endif

		memset(s->value,0,4);
		i = s->len;
		while(i<4){
			s->value[i] = data[i-1];
			i++;
		}
		free(data);

		while (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY)
		{
		}

	}while(HAL_I2C_GetError(&hi2c1) == HAL_I2C_ERROR_AF);

	return true;
}


bool write_i2c(){
	//Sensor* s

	/*##-5- Master sends read request for slave ##############################*/
	uint8_t id = (uint8_t)(s->address>>24);
	hi2c1.Init.OwnAddress1 = id;	// slave id

	/* Update bTransferRequest to send buffer read request for Slave */
	//uint8_t bTransferRequest = MASTER_REQ_READ;
	uint8_t len = s->len;
	uint8_t bTransferRequest[len+1];
	bTransferRequest[0] = (uint8_t)s->address;

	uint8_t res = 0;

	#ifdef DEBUG_I2C
		sprintf(logging.log_msg,"i2c write >> id: 0x%x, data: ",id);
		logging.log_nnl(logging.log_msg,"sensors");

		sprintf(logging.log_msg,"0x%x ",bTransferRequest[0]);
		logging.log_nnl(logging.log_msg,"sensors");
	#endif

	uint8_t i = 0;
	while(i < s->len){
		bTransferRequest[i+1] = (uint8_t)s->value[i];
		#ifdef DEBUG_I2C
			if(i == s->len-1){
				sprintf(logging.log_msg,"0x%x \n",bTransferRequest[i+1]);
				logging.log_nnl(logging.log_msg,"sensors");
			}else{
				sprintf(logging.log_msg,"0x%x ",bTransferRequest[i+1]);
				logging.log_nnl(logging.log_msg,"sensors");
			}
		#endif
		i++;
	}

	id = id<<1;
	do
	{
		res = HAL_I2C_Master_Transmit(&hi2c1, (uint8_t)id, (uint8_t*)&bTransferRequest, 1+s->len,100);
		if( res != HAL_OK)
		{
			sprintf(logging.log_msg,"!! error i2c >> res: 0x%x \n",res);
			logging.log_nnl(logging.log_msg,"sensors");
			/* Error_Handler() function is called when error occurs. */
			//Error_Handler();
			return false;
		}

		/*  Before starting a new communication transfer, you need to check the current
				state of the peripheral; if it�s busy you need to wait for the end of current
				transfer before starting a new one.
				For simplicity reasons, this example is just waiting till the end of the
				transfer, but application may perform other tasks while transfer operation
				is ongoing. */

		while (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY)
		{
		}


		/* When Acknowledge failure occurs (Slave don't acknowledge it's address)
			 Master restarts communication */
	}while(HAL_I2C_GetError(&hi2c1) == HAL_I2C_ERROR_AF);

	return true;
  /* Flush Rx buffers */
  //Flush_Buffer((uint8_t*)aRxBuffer,RXBUFFERSIZE);
}



/* SPI1 init function */
void MX_SPI1_Init(void)
{

  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_HARD_OUTPUT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* USART1 init function */
void MX_USART1_UART_Init(void)
{

  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;


  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* USART2 deinit function */
void MX_USART1_UART_DeInit(void)
{
	if (HAL_UART_DeInit(&huart1) != HAL_OK)
		_Error_Handler(__FILE__, __LINE__);
}

void MX_USART2_UART_DeInit(void)
{
	if (HAL_UART_DeInit(&huart2) != HAL_OK)
		_Error_Handler(__FILE__, __LINE__);
}

/* USART2 init function */
void MX_USART2_UART_Init(void)
{

  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}


/* USART4 init function */
void MX_USART4_UART_Init(void)
{

  huart4.Instance = USART4;
  huart4.Init.BaudRate = 9600;
  huart4.Init.WordLength = UART_WORDLENGTH_8B;
  huart4.Init.StopBits = UART_STOPBITS_1;
  huart4.Init.Parity = UART_PARITY_NONE;
  huart4.Init.Mode = UART_MODE_TX_RX;
  huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart4.Init.OverSampling = UART_OVERSAMPLING_16;
  huart4.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart4.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_RS485Ex_Init(&huart4, UART_DE_POLARITY_HIGH, 0, 0) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

void MX_GPIO_Low_Consumption(void){
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Enable GPIOs clock */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();

  /* Configure all GPIO port pins in Analog Input mode (floating input trigger OFF) */
  /* Note: Debug using ST-Link is not possible during the execution of this   */
  /*       example because communication between ST-link and the device       */
  /*       under test is done through UART. All GPIO pins are disabled (set   */
  /*       to analog input mode) including  UART I/O pins.           */
  GPIO_InitStructure.Pin = GPIO_PIN_All;
  GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStructure.Pull = GPIO_NOPULL;

  HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
  HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
  //HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);
  HAL_GPIO_Init(GPIOD, &GPIO_InitStructure);
  HAL_GPIO_Init(GPIOH, &GPIO_InitStructure);
  HAL_GPIO_Init(GPIOE, &GPIO_InitStructure);

  /* Disable GPIOs clock */
  __HAL_RCC_GPIOA_CLK_DISABLE();
  __HAL_RCC_GPIOB_CLK_DISABLE();
  __HAL_RCC_GPIOC_CLK_DISABLE();
  __HAL_RCC_GPIOD_CLK_DISABLE();
  __HAL_RCC_GPIOH_CLK_DISABLE();
  __HAL_RCC_GPIOE_CLK_DISABLE();


}

void MX_GPIO_Enable_Clock(void){
	/* Enable GPIOs clock */
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOH_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();
}

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;


  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

	// --- GPIOs ---
  //Configure GPIO pins : PC0 PC1 /
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	/* Enable and set EXTI lines 4 to 15 Interrupt to the lowest priority */
  HAL_NVIC_SetPriority(EXTI0_1_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ(EXTI0_1_IRQn);

	//Configure GPIO pins : PC2 /
	GPIO_InitStruct.Pin = GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	//Configure GPIO pins : PC3 /
	GPIO_InitStruct.Pin = GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	// --- --- ---

  //Configure GPIO pin Output Level /
  GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12, GPIO_PIN_RESET);


  GPIO_InitStruct.Pin = GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pins : PC3 PC9 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}


/* ADC init function */
void MX_ADC_Init(void)
{

  ADC_ChannelConfTypeDef sConfig;

    /**Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
    */
  hadc.Instance = ADC1;
  hadc.Init.OversamplingMode = DISABLE;
  hadc.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
  hadc.Init.Resolution = ADC_RESOLUTION_12B;
  hadc.Init.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  hadc.Init.ScanConvMode = ADC_SCAN_DIRECTION_FORWARD;
  hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc.Init.ContinuousConvMode = DISABLE;
  hadc.Init.DiscontinuousConvMode = DISABLE;
  hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc.Init.DMAContinuousRequests = DISABLE;
  hadc.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc.Init.LowPowerAutoWait = DISABLE;
  hadc.Init.LowPowerFrequencyMode = DISABLE;
  hadc.Init.LowPowerAutoPowerOff = DISABLE;

  if (HAL_ADC_Init(&hadc) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }


  // ### - 2 - Start calibration ############################################
  if (HAL_ADCEx_Calibration_Start(&hadc, ADC_SINGLE_ENDED) != HAL_OK)
  {
    Error_Handler();
  }

}

/* ADC deinit function */
void MX_ADC_DeInit(void)
{
	if (HAL_ADC_DeInit(&hadc) != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}
}

/* I2C1 init function */
void MX_I2C1_Init(void)
{

  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x00000708;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  //hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_10BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure Analogue filter
    */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure Digital filter
    */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* I2C2 init function */
void MX_I2C2_Init(void)
{
  hi2c2.Instance = I2C2;
  hi2c2.Init.Timing = 0x00000708;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure Analogue filter
    */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c2, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure Digital filter
    */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c2, 0) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}
