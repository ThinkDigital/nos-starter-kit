#ifndef __SERIAL_H
#define __SERIAL_H

#include "stm32l0xx_hal.h"
#include "queue"
using namespace std;

#include "logging.h"


//#ifndef RXBUFFERSIZE
#define RXBUFFERSIZE 256
//#endif

#ifdef __cplusplus
 extern "C" {
#endif

class Serial{
public:

  std::queue<char> tx_buffer;
  std::queue<char> rx_buffer;

  Serial();
  void begin(uint8_t n_uart,uint32_t baudrate);
  void end();
  bool read_byte(uint8_t* byte);
  bool clear();
  uint8_t read();
  uint16_t available();
  void write(char* data, uint8_t size);
  void write(uint8_t* data, uint8_t size);
  //uint8_t write(std::queue<char> *buffer_tx);
  void flush();

private:

	uint8_t aRxBuffer[RXBUFFERSIZE];
  uint8_t rx_pos;
  UART_HandleTypeDef* huart = NULL; // debug

  void log(const char* text);
};



#ifdef __cplusplus
}
#endif

#endif
