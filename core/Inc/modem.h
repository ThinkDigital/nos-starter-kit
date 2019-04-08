/*
 * modem.h
 *
 *  Created on: Nov 12, 2018
 *      Author: jsilva
 */

#ifndef INC_MODEM_H_
#define INC_MODEM_H_


#include <timestamp.h>
#include "stm32l0xx_hal.h"
#include "malloc.h"
#include "string.h"
#include "strings.h"
#include "strstream" // to use string import this
#include "queue"

#include "serial.h"
#include "logging.h"
#include "network.h"

using namespace std;


#ifdef __cplusplus
 extern "C" {
#endif


class MODEM{
public:
  MODEM();

  void sendAtCommand(const char* command);
  void sendAtCommand(string command);
  bool checkAtCommand(const char* command, const char* response, uint16_t timeout);
  string getAtCommandResponse(const char* command, uint16_t timeout);
  string getAtCommandResponse(const char* command, const char* filter, uint16_t timeout);

private:

  void checkMessages();
  void parseMessage();
  void evaluateMessage(char* p_data);
  void sendData();

  int find(string text, string word);

  char response[256];
  uint8_t response_size;
};

//void logging_loop(uint32_t interval);

/*
void log(String text);
void log(String key, uint16_t value);
void log(String key, uint8_t data[], uint8_t len);
void log(String key, char* value);
void log_progress();
void log_progress_end();

String pad2(int value);
String date();
String dec2hex(uint8_t n);
*/

#ifdef __cplusplus
}
#endif


#endif /* INC_MODEM_H_ */
