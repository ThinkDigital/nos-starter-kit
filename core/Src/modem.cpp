/*
 * modem.cpp

 *
 *  Created on: Nov 12, 2018
 *      Author: jsilva
 */
#include <modem.h>

Serial *serial_modem = NULL;

extern LOGGING logging;

#ifdef BC68
	extern Modem_BCxx* modem;
#endif

// init modem class
MODEM::MODEM(){

	response_size = 0;
	serial_modem = new Serial();
  serial_modem->begin(1,9600);
}


void MODEM::sendAtCommand(string command){
	sendAtCommand((const char*)command.c_str());
}

void MODEM::sendAtCommand(const char* command){

	checkMessages();

	char* ptr = (char*)malloc(256);

	if(ptr == nullptr)
		return;
	memset(ptr,0,256);
	strcpy(ptr,command);
	strcat(ptr,"\n\r");

	uint8_t i = 0;
	while(ptr[i] != '\0' && i < 256){
		serial_modem->tx_buffer.push(ptr[i]);
		i++;
	}
	free(ptr);

	sendData();

	HAL_Delay(100);

}

bool MODEM::checkAtCommand(const char* command, const char* response_expected, uint16_t timeout){

	memset(response,0,256);

	sendAtCommand(command);
	HAL_Delay(10);
	uint32_t time = timeout + millis();
	while(time > millis()){

		checkMessages();
		if(response_size > 0){
			string s = string(response);
			if(find(s,string(response_expected)) > -1){
				logging.log((char*)"response expected","modem");
				memset(response,0,256);
				return true;
			}
		}
		HAL_Delay(10);
	}
	return false;
}

string MODEM::getAtCommandResponse(const char* command, uint16_t timeout){

	sendAtCommand(command);

	uint32_t time = timeout + millis();
	while(time > millis()){
		checkMessages();
		if(response_size > 0){
			string r = string(response);
			int index = find(r,"ERROR");
			if(index == -1){  // check if no error was returned
				index = find(r,"\r\n");
				if(index > -1)
					r = r.substr(0,index);
				return r;
			}else return "";

		}
		HAL_Delay(10);
	}

	return (string)"";
}

string MODEM::getAtCommandResponse(const char* command, const char* filter, uint16_t timeout){

	sendAtCommand(command);

	uint32_t time = timeout + millis();
	while(time > millis()){
		checkMessages();
		if(response_size > 0){
			string r = string(response);
			string f = string(filter);
			int index = find(r,f);
			if(index > -1){
				string d = r.substr(index+f.length());
				index = find(d,"\n");
				if(index > -1)
					d = r.substr(0,index);
				logging.println("modem","val: ",d);
				return d;
			}
		}
		HAL_Delay(10);
	}

	return (string)"";
}


// -------------------
// private
// -------------------

// check if there are data available on buffer, in case there is reads that data
void MODEM::checkMessages(){

	response_size = 0;
	if(serial_modem->available()>0){
		serial_modem->read();
		parseMessage();
		//evaluateMessage();
	}

}

// if there is data on buffer, it tries to decode that data in messages
// It only supports one message at time for now
void MODEM::parseMessage(){

	response_size = 0;
	memset(response,0,256);
	char c = 0;
	char prev_c = 0;
	while(serial_modem->rx_buffer.size() > 0 && serial_modem->rx_buffer.size() < 256){
		c = (char)serial_modem->rx_buffer.front();
		if(prev_c == '\r' && c == '\n'){
		//if(c != '\n' && c != '\r'){
			response_size --;

		}else{
			response[response_size] = c;
			prev_c = c;
			response_size++;
		}
		serial_modem->rx_buffer.pop();
	}

	if(response_size > 0)
		logging.log((char*)response,"modem","<<");

	//modem->parseResponse(response);
}



void MODEM::sendData(){
	if(serial_modem->tx_buffer.size() > 0){

  	char* modem_buffer = (char*)malloc((unsigned int)serial_modem->tx_buffer.size());
  	//char* log_buffer = (char*)malloc((unsigned int)serial_modem->tx_buffer.size());
  	uint8_t buffer_size = 0;

  	if(modem_buffer == nullptr)
			return;

		while(serial_modem->tx_buffer.size() > 0){
			modem_buffer[buffer_size] = (uint8_t)serial_modem->tx_buffer.front();
      //log_buffer[buffer_size] = modem_buffer[buffer_size];
			serial_modem->tx_buffer.pop();
			buffer_size++;
		}

  	if(buffer_size > 0){
      //logging.log(modem_buffer,"modem");
      serial_modem->write(modem_buffer,buffer_size);
      logging.log(modem_buffer,"modem", ">>");
    }

  	free(modem_buffer);

  }
}

int MODEM::find(string text, string word){
	//logging.log((char*)text.c_str(),"find");
	//logging.log((char*)word.c_str(),"find");
  std::size_t found = text.find(word);
  if (found!=std::string::npos)
    return (int)found;
  else return -1;

}
