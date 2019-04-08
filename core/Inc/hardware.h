/*
 * hardware.h
 *
 *  Created on: Nov 30, 2018
 *      Author: jsilva
 */

#ifndef INC_HARDWARE_H_
#define INC_HARDWARE_H_


#include "stm32l0xx_hal.h"

#include "logging.h"
#include "settings.h"
#include "package.h"
#include "serial.h"

#ifdef ENABLE_RS485
	#include "modbus-rtu.h"
#endif

extern const uint8_t HARDWARE_RS485_PIN;
extern const bool    HARDWARE_RS485_PIN_LOGIC; // true = natural , false = inverted

// create stuct for gpios
// create spi and i2c functions

#define N_GPIOS                            	4

#define DIGITAL_INPUT                       1
#define COUNTER 														2
#define ANALOG                              3
#define DIGITAL_OUTPUT                      4

#define NONE	 															0
#define GREEN	 															1
#define RED	   															2
#define BLUE	 															3

// time to wait between RS485 read tries
#define    HARDWARE_RS485_READ_WAIT    10 // millis
// number of tries before giving up
#define    HARDWARE_RS485_READ_TRIES   3 // atempts

// public
void hardware_init(); // initialize all hardware
void update_gpios();

void hardware_en_peripherals();
void hardware_dis_peripherals();

bool set_digital_output(uint8_t index, bool state);		// set relay to a state (true = on, false = off), returns success
bool read_digital_input(uint8_t index, uint8_t* value);
bool read_analog_input(uint8_t index, uint16_t* value);		// not implemented

void set_rgb_colour(uint8_t colour);

void      hardware_rs485_init();
void      hardware_rs485_write(uint8_t data[], uint8_t len);
uint8_t   hardware_rs485_read(uint8_t data[], uint8_t len);

bool read_i2c();
bool write_i2c();


void MX_GPIO_Init(void);
void MX_GPIO_Low_Consumption(void);
void MX_GPIO_Enable_Clock(void);
void MX_USART1_UART_Init(void);
void MX_USART1_UART_DeInit(void);
void MX_USART2_UART_DeInit(void);
void MX_USART2_UART_Init(void);
void MX_USART4_UART_Init(void);
void MX_I2C1_Init(void);
void MX_SPI1_Init(void);
void MX_I2C2_Init(void);
void MX_ADC_Init(void);
void MX_ADC_DeInit(void);

// private
void config_adc();
bool hardware_rs485_read_byte(uint8_t* byte);


#endif /* INC_HARDWARE_H_ */
