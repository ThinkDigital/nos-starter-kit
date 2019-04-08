
#include "logging.h"
#include "serial.h"
#include "string.h"
#include "strstream" // to use string import this

Serial *serial_debug = NULL;
extern UART_HandleTypeDef huart2; // debug

LOGGING logging;

LOGGING::LOGGING() {

}

void LOGGING::init(uint32_t baudrate) {
	char* data = "OLA\n";
	HAL_UART_Transmit(&huart2,(uint8_t*)data,sizeof(data),10);

	serial_debug = new Serial();

	serial_debug->begin(2,baudrate);

	//settings.log.baudrate
	//settings.log.config

}


void LOGGING::log( char* data, const char* dev){

	string s ="[";
	s += string(dev);
	s += "]: ";
	s += string(data);
	s += '\n';

	// serial.write
	//serial_debug(& (char*)bfr, strlen(data)+strlen(str)+strlen(dev)+strlen("\n"), 100);
	serial_debug->write((char *)s.c_str(), s.length());

	return;
}

void LOGGING::log_nnl( char* data, const char* dev){

	string s = string(data);

	// serial.write
	//serial_debug(& (char*)bfr, strlen(data)+strlen(str)+strlen(dev)+strlen("\n"), 100);
	serial_debug->write((char *)s.c_str(), s.length());

	return;
}


void LOGGING::log( char* data, const char* dev, const char* flow){

	string s ="[";
	s += string(dev);
	s += "] ";
	s += string(flow);
	s += " ";
	s += string(data);
	s += '\n';

	// serial.write
	//serial_debug(& (char*)bfr, strlen(data)+strlen(str)+strlen(dev)+strlen("\n"), 100);
	serial_debug->write((char *)s.c_str(), s.length());

	return;
}

void LOGGING::log_hex( char* hex, const char* dev, const char* flow){

	string s ="[";
	s += string(dev);
	s += "] ";
	s += string(flow);
	s += " ";

	char byte[2];
	int len = sizeof(hex);
	for(int i=0; i< len; i++)
	{
			itoa((int)hex[i],byte,16);
	    s += "0x"+string(byte)+" ";
	}

	s += '\n';
	serial_debug->write((char *)s.c_str(), s.length());
	return;
}

/*
void LOGGING::log( string data, const char* dev){

	string s ="[";
	s += string(dev);
	s += "]: ";
	s += data;
	s += '\n';


	// serial.write
	//serial_debug(& (char*)bfr, strlen(data)+strlen(str)+strlen(dev)+strlen("\n"), 100);
	serial_debug->write((char *)s.c_str(), s.length());

	return;
}
*/

/*
void LOGGING::log( string data, string dev){

	string s ="[";
	s += dev;
	s += "]: ";
	s += data;
	s += '\n';


	// serial.write
	//serial_debug(& (char*)bfr, strlen(data)+strlen(str)+strlen(dev)+strlen("\n"), 100);
	serial_debug->write((char *)s.c_str(), s.length());

	return;
}
*/

bool LOGGING::print(char* data,int value){


	string s = string(data);
	s += std::to_string(value);

	serial_debug->write((char*)s.c_str(),s.size());

	return true;
}

bool LOGGING::print(char* data,double value){


	string s = string(data);
	s += std::to_string(value);

	serial_debug->write((char*)s.c_str(),s.size());

	return true;
}

bool LOGGING::print(char* data,long value){


	string s = string(data);
	s += std::to_string(value);

	serial_debug->write((char*)s.c_str(),s.size());

	return true;
}

bool LOGGING::print(char* data,float value){


	string s = string(data);
	s += std::to_string(value);

	serial_debug->write((char*)s.c_str(),s.size());

	return true;
}

bool LOGGING::println(char *log,char* data,int value){

	string s ="["
	+ string(log)
	+"]: "
	+ string(data)
	+ std::to_string(value)
	+ '\n';

	serial_debug->write((char*)s.c_str(),s.size());

	return true;
}

bool LOGGING::println(char *log,char* data,double value){

	string s ="[";
	s += string(log);
	s+= "]: ";
	s+= data;
	s += std::to_string(value);
	s += '\n';

	serial_debug->write((char*)s.c_str(),s.size());

	return true;
}

bool LOGGING::println(char *log,char* data,long value){

	string s ="[";
	s += string(log);
	s += "]: ";
	s += string(data);
	s += std::to_string(value);
	s += '\n';

	serial_debug->write((char*)s.c_str(),s.size());

	return true;
}

bool LOGGING::println(char *log,char* data,float value){

	string s ="["
	+ string(log)
	+ "]: "
	+ string(data)
	+ std::to_string(value)
	+ '\n';

	serial_debug->write((char*)s.c_str(),s.length());

	return true;
}

bool LOGGING::println(const char *log,const char* data,string value){


	string s ="["
	+ string(log)
	+ "]: "
	+ string(data)
	+ (string)value
	+ '\n';

	serial_debug->write((char*)s.c_str(),s.length());

	return true;
}
