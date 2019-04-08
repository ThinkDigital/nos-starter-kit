/*
 * time.cpp
 *
 *  Created on: Nov 13, 2018
 *      Author: jsilva
 */

#include <timestamp.h>

// timestamp in Seconds
 uint32_t getTimestamp(void){

   RTC_DateTypeDef dateStruct;
    RTC_TimeTypeDef timeStruct;
    struct tm timeinfo;

    hrtc.Instance = RTC;

    // Read actual date and time
    HAL_RTC_GetTime(&hrtc, &timeStruct, FORMAT_BIN); // Read time first!
    HAL_RTC_GetDate(&hrtc, &dateStruct, FORMAT_BIN);

    // Setup a tm structure based on the RTC
    timeinfo.tm_wday = dateStruct.WeekDay;
    timeinfo.tm_mon  = dateStruct.Month - 1;
    timeinfo.tm_mday = dateStruct.Date;
    timeinfo.tm_year = dateStruct.Year + 100;
    timeinfo.tm_hour = timeStruct.Hours;
    timeinfo.tm_min  = timeStruct.Minutes;
    timeinfo.tm_sec  = timeStruct.Seconds;

    // Convert to timestamp
    time_t t = mktime(&timeinfo);
   return (uint32_t)t;
 }


 uint32_t millis(void){
   return HAL_GetTick();
 }

 void setTime(tm curr_time){

   RTC_DateTypeDef sdatestructureget;
   RTC_TimeTypeDef stimestructureget;

   sdatestructureget.WeekDay = RTC_WEEKDAY_MONDAY;
   sdatestructureget.Year = curr_time.tm_year;  // In fact: 2000 + 18 - 1900
   sdatestructureget.Date = curr_time.tm_mday;
   sdatestructureget.Month = curr_time.tm_mon;

   stimestructureget.Hours = curr_time.tm_hour;
   stimestructureget.Minutes = curr_time.tm_min;
   stimestructureget.Seconds = curr_time.tm_sec;
   stimestructureget.SubSeconds     = 0x00;
   stimestructureget.TimeFormat     = RTC_HOURFORMAT12_AM;
   stimestructureget.DayLightSaving = RTC_DAYLIGHTSAVING_NONE ;
   stimestructureget.StoreOperation = RTC_STOREOPERATION_RESET;

   HAL_RTC_SetTime(&hrtc, &stimestructureget, FORMAT_BIN);
   HAL_RTC_SetDate(&hrtc, &sdatestructureget, FORMAT_BIN);

}
