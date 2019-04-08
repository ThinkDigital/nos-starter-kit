/*
 * main_aux.c
 *
 *  Created on: Nov 13, 2018
 *      Author: jsilva
 */

#include "stm32l0xx_hal.h"
#include "main_aux.h"
/**
 * @brief  Retargets the C library printf function to the USART.
 * @param  None
 * @retval None
 */
extern UART_HandleTypeDef huart2;

PUTCHAR_PROTOTYPE {
	// Place your implementation of fputc here
	// e.g. write a character to the USART2 and Loop until the end of transmission
	HAL_UART_Transmit(&huart2, (uint8_t *) &ch, 1, 0xFFFF);
	//HAL_UART_Transmit(&huart2, (uint8_t *) &ch, 1, 10);

	return ch;
}
