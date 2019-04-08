/*
 * package.h
 *
 *  Created on: Nov 28, 2018
 *      Author: jsilva
 */

#ifndef INC_PACKAGE_H_
#define INC_PACKAGE_H_

// DO NOT CHANGE THIS, THINK 10000 TIMES AND ASK DIOGO BEFORE DOING IT
#define PACKAGE_VERSION_MAJOR   1
#define PACKAGE_VERSION_MINOR  	0

// !!!
#define DEVELOPING // comment it when development phase has finished
// !!!

//#define COAP_DEBUG
//#define DEBUG_LOTUS
#define DEBUG_SENSORS
#define DEBUG_SENSORS_HIGH
#define DEBUG_I2C
#define DEBUG_RS485
#define DEBUG_MESSAGE
#define DEBUG_MODBUS
#define DEBUG_MODEM
#define DEBUG_MODEM_MESSAGE

//#define logging_MEMORY_READ
//#define logging_MEMORY_WRITE
#define logging_MEMORY_CONTEXT

//#define DEBUG_RAM

/////////////////////////////////////////////////////////////////////
//                                                                 //
//                          CHOOSE HARDWARE                        //
//                                                                 //
/////////////////////////////////////////////////////////////////////

//#define HARDWARE_TYPE_STM32L0_TESTBOARD
#define HARDWARE_TYPE_STM32L0_NOSKIT


// uncomment the MODEM which you are using
//#define BC95
#define BC68

/////////////////////////////////////////////////////////////////////
//                                                                 //
//                         CHOOSE DEPLOYMENT                       //
//                                                                 //
/////////////////////////////////////////////////////////////////////
// !! choose one from here
// #define DEPLOY_PRODUCTION_VODAFONE // Vodafone default configuration
#define DEPLOY_DEVELOPMENT
// #define DEPLOY_PRODUCTION_NOS_EDP
// #define DEPLOY_SMARTSCREEN

// !! and one from here
#define DEPLOY_PRODUCTION_NOS_NB

/////////////////////////////////////////////////////////////////////
//                                                                 //
//                 CHOOSE ADDITIONAL STUFF (OPTIONAL)              //
//                                                                 //
/////////////////////////////////////////////////////////////////////

	// This module is responsible for creating a GPRS connection (if also defined in settings)
	//#define ENABLE_GPRS
	// #define ENABLE_GPRS_ID // if enabled, uses IMEI instead of MAC ADDRESS when GPRS is enabled

	// This module is responsible for creating a NB connection (if also defined in settings)

/////////////////////////////////////////////////////////////////////
//                                                                 //
// DO NOT CHANGE BELOW THIS LINE UNLESS YOU KNOW WHAT YOU'RE DOING //
//                                                                 //
/////////////////////////////////////////////////////////////////////

//#define CTT_BALANCE
// balance racio 1,17
// weight(g) = output value * 1,17

#if defined HARDWARE_TYPE_STM32L0_TESTBOARD

	#define PACKAGE_MODEL 50

	//#define ENABLE_RS485
	//#define ENABLE_I2C

	//#define BME280
	//#define HX711
	//#define RGBLED

#elif defined HARDWARE_TYPE_STM32L0_NOSKIT

	#define PACKAGE_MODEL 50

	//#define ENABLE_RS485
	//#define ENABLE_I2C
	//#define RGBLED

#endif

/////////////////////////////////////////////////////////////////////
//                                                                 //
//                 NETWORK SETTINGS									               //
//                                                                 //
/////////////////////////////////////////////////////////////////////

#define TIME_ONLINE_DURING_FIRST_CONNECTION 	30 // seconds
//#define PSMMODE

#if defined DEPLOY_PRODUCTION_NOS_NB
	#define SETTINGS_NB_APN         	"tdtnb"
	#define SETTINGS_NB_USERNAME    	""
	#define SETTINGS_NB_PASSWORD    	""
	#define SETTINGS_NB_BAND		    	20
	#define SETTINGS_NB_COPS		    	26803
	#define SETTINGS_NB_SERVER_HOST   "192.168.0.41"
	#define SETTINGS_NB_SERVER_PORT   5684
#endif

#define LOW 0
#define HIGH 1

#endif /* INC_PACKAGE_H_ */
