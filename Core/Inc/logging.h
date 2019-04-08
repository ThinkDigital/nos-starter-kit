/*
 * eeprom.h
 *
 *  Created on: May 20, 2018
 *      Author: jsilva
 */

#ifndef __LOGGING_H
#define __LOGGING_H

//#include <user.hpp>
#include "stm32l0xx_hal.h"
#include "string.h"
#include "queue"
#include "strstream" // to use string import this
using namespace std;

#define DEBUG_SETTINGS 					  0x01
//#define logging_MEMORY          0x02
#define logging_NETWORK           0x03
#define logging_HARDWARE          0x04

#ifdef __cplusplus
 extern "C" {
#endif


//#define LOG_MEMORY  // comment to hide log memory

class LOGGING {

public:
  LOGGING();
	char log_msg[256];
	void init(uint32_t baudrate) ;
	void log( char* data, const char* dev);
	void log_nnl( char* data, const char* dev);  // no new line
	void log( char* data, const char* dev, const char* flow);
	void log_hex( char* data, const char* dev, const char* flow);
	//void log( string data, const char* dev);
	bool print(char* data,int value);
	bool print(char* data,double value);
	bool print(char* data,long value);
	bool print(char* data,float value);
	bool println(char *log,char* data,int value);
	bool println(char *log,char* data,double value);
	bool println(char *log,char* data,long value);
	bool println(char *log,char* data,float value);
  bool println(const char *log,const char* data,string value);
private:
};

extern LOGGING logging;

#ifdef __cplusplus
}
#endif

#endif /* __LOG_H */
