/*
 * time.h
 *
 *  Created on: Nov 13, 2018
 *      Author: jsilva
 */

#ifndef INC_TIMESTAMP_H_
#define INC_TIMESTAMP_H_

#include <time.h>
#include "main.h"
#include "stm32l0xx_hal.h"

extern RTC_HandleTypeDef hrtc;

#ifdef __cplusplus
 extern "C" {
#endif

uint32_t getTimestamp(void);
uint32_t millis(void);
void setTime(tm curr_time);


#ifdef __cplusplus
}
#endif


#endif /* INC_TIMESTAMP_H_ */
