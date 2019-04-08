/*

 *
 *  Created on: Nov 13, 2018
 *      Author: jsilva
 */
/*
 * network.cpp
 *
 *  Created on: Nov 12, 2018
 *      Author: jsilva
 */

#include "network.h"

#define PSMMODE
//extern LOGGING logging;

MODEM *bcxx = NULL;

char imei[16] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

NETWORK::NETWORK(){
}

void NETWORK::init(){

  bcxx = new MODEM();

}

bool NETWORK::connect(){

  bcxx->checkAtCommand("ATE=0", "OK",1000); //power saving mode

	//if(dev.imei == "")
		//getImei();

	//if(dev.ccid = "")
		//getSimCardId();
	bcxx->checkAtCommand("AT+NCONFIG=\"CR_0354_0338_SCRAMBLING\",\"TRUE\"", "OK", 1000);
	bcxx->checkAtCommand("AT+NCONFIG=\"CR_0859_SI_AVOID\",\"TRUE\"", "OK", 1000);
	bcxx->checkAtCommand("AT+NCONFIG=\"AUTOCONNECT\",\"FALSE\"", "OK", 1000);

	bcxx->checkAtCommand("AT+NBAND=20", "OK", 1000);

  // if it was the first that these configs were set, the modem will need to be rebooted

	//power saving mode
	#ifdef PSMMODE
		setPSM(settings.modem.tau, settings.modem.awake);
	#else
		bcxx->checkAtCommand("AT+CPSMS=0", "OK",1000);
	#endif

	if (!bcxx->checkAtCommand("AT+CFUN=1", "OK", 3000)) return;

}

bool NETWORK::sendData(char *msg, uint16_t size){
  return modem->send_udp(0,settings.server.host,settings.server.port,msg,size,3);
}

void NETWORK::log(const char* text){
  #ifdef logging_NETWORK
    logging.log((char*)text,"network");
  #endif
}

void NETWORK::log(string text){
  #ifdef logging_NETWORK
    logging.log((char*)text.c_str(),"network");
  #endif
}
