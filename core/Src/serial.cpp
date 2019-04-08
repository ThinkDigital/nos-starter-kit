

#include "serial.h"
#include "string.h"
#include "malloc.h"

extern UART_HandleTypeDef huart1; // debug
extern UART_HandleTypeDef huart2; // debug
extern UART_HandleTypeDef huart4; // debug

extern LOGGING logging;

Serial::Serial(){
  memset(aRxBuffer, 0, RXBUFFERSIZE);
  rx_pos = 0;
}

void Serial::begin(uint8_t n_uart,uint32_t baudrate){
  if(n_uart == 1){
    huart = &huart1;
	  huart->Init.BaudRate = baudrate;
    // call it only once
 	 if(HAL_UART_Receive_IT(huart, (uint8_t *)aRxBuffer, RXBUFFERSIZE) != HAL_OK)
 	 {
       printf("USART1: error while initing  HAL_UART_Receive_IT function\n");
 	    //Error_Handler();
 	 }

	  //MX_USART1_UART_Init();
  }else if(n_uart == 2){
    huart = &huart2;
    huart->Init.BaudRate = baudrate;
    //MX_USART2_UART_Init();
  }else if(n_uart == 4){
    huart = &huart4;
	  huart->Init.BaudRate = baudrate;
    // call it only once

 	 if(HAL_UART_Receive_IT(huart, (uint8_t *)aRxBuffer, RXBUFFERSIZE) != HAL_OK)
 	 {
       printf("USART4: error while initing  HAL_UART_Receive_IT function\n");
 	    //Error_Handler();
 	 }

	  //MX_USART1_UART_Init();
  }

  /*
  char data[50];
  sprintf(data,"uart: %d baudrate %lu \n",n_uart,baudrate);
  HAL_UART_Transmit(&huart2,(uint8_t*)data,50,100);
  */
}

void Serial::end(){

	return;
}

void Serial::flush(){

	return;
}


void Serial::write(char* data, uint8_t size){

	if(size > 0)
		HAL_UART_Transmit(huart,(uint8_t*)data,size,3000);

	return;
}

void Serial::write(uint8_t* data, uint8_t size){

	if(size > 0)
		HAL_UART_Transmit(huart,data,size,3000);

	return;
}
/*

uint8_t Serial::write(std::queue<char> *buffer_tx){
	uint8_t* p_buffer = (uint8_t*)malloc((unsigned int)buffer_tx->size());
	uint8_t buffer_size = 0;

	if(p_buffer != NULL){
		while(buffer_tx->size() > 0){
			p_buffer[buffer_size] = (uint8_t)buffer_tx->front();
			buffer_tx->pop();
			buffer_size++;
		}
	}

	if(buffer_size > 0)
		HAL_UART_Transmit(huart,p_buffer,buffer_size,10);
	free(p_buffer);
	return (buffer_size-buffer_tx->size()); // I am not sure if the data is transmitted
}
*/

uint16_t Serial::available(){
  //return aRxBuffer[0];
  return (uint16_t)(huart->RxXferSize-huart->RxXferCount);
}

bool Serial::read_byte(uint8_t *byte){

  if(read()){
    *byte = rx_buffer.front();
    rx_buffer.pop();
    return true;
  }
  return false;
  /*
  if(rx_buffer.size() == 0)
    read();
  int8_t byte = -1;

  return byte;
  */
}

bool Serial::clear(){
  if(available())
    read();

  return true;
}

// this function reads from buffer and stores in a queue returning the number of bytes in that queue
uint8_t Serial::read(){


  // check if data was received
  // an interrupt should be called when one byte is received, not when buffer is full

  //__HAL_LOCK(huart);
  /*
  logging.print((char*)"huart->RxState: ", huart->RxState);
  logging.print("\n huart->RxXferCount:", huart->RxXferCount);
  logging.print("\n huart->RxXferSize: ", huart->RxXferSize);
  */


  while(rx_buffer.size() < RXBUFFERSIZE && rx_pos < (huart->RxXferSize-huart->RxXferCount) ){
    rx_buffer.push(aRxBuffer[rx_pos]);
    rx_pos++;
  }



  HAL_UART_AbortReceive_IT(huart); // this solves overflow issues
  huart->RxXferCount = huart->RxXferSize;
  rx_pos = 0;



  if(HAL_UART_Receive_IT(huart, (uint8_t *)aRxBuffer, RXBUFFERSIZE) != HAL_OK){
     printf("error while initing  HAL_UART_Receive_IT function\n");
     //Error_Handler();
  }

  //rx_pos = huart->RxXferCount;
  /*
  if(rx_pos > 0){
    memset(aRxBuffer,0,RXBUFFERSIZE);
    HAL_UART_AbortReceive_IT(huart); // this solves overflow issues

    rx_pos = 0;
  }
  */

  /*
  logging.print((char*)"\n huart->RxState: ", huart->RxState);
  logging.print("\n huart->RxXferCount:", huart->RxXferCount);
  logging.print("\n huart->RxXferSize: ", huart->RxXferSize);
  */
  //__HAL_UNLOCK(huart);

  return rx_buffer.size();
}

void Serial::log(const char* text){
  logging.log((char*)text,"serial");
}
