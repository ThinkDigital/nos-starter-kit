/*
 * network.h
 *
 *  Created on: Nov 12, 2018
 *      Author: jsilva
 */

#ifndef INC_NETWORK_H_
#define INC_NETWORK_H_

#include <timestamp.h>
#include "stm32l0xx_hal.h"

#include "malloc.h"
#include "string.h"
#include "strstream" // to use string import this
#include "queue"


#include "main.h"
#include "package.h"
#include "hardware.h"
#include "Modem_BCxx.h"
#include "logging.h"
#include "settings.h"
#include "message.h"


#define PROTOCOL_UDP   1
#define PROTOCOL_TCP   2

/*
#define HANDSHAKE_PREFIX    ""
#define HANDSHAKE_SUFIX     "#"

using namespace std;

// Network Events

#define NETWORK_STATE_UNKNOW                    0
#define NETWORK_STATE_DISCONNECTED              1
#define NETWORK_STATE_CONNECTED                 2
#define NETWORK_STATE_HANDSHAKE                 3
#define NETWORK_STATE_WAITING_HANDSHAKE         4
#define NETWORK_STATE_AUTHENTICATED             5
#define NETWORK_STATE_AUTHENTICATION_FAILED     6
*/

/*
#define RECONNECT_START     5000 // 5 seconds
#define RECONNECT_STEP         2 // x2
#define RECONNECT_MAX     320000 // 5 minutes and 20 seconds (must be multiple of start)

#define NETWORK_AUTH_TIMEOUT  15000 // milliseconds
#define NETWORK_AUTH_TRIES    3 // milliseconds

#define MAX_PACKET_SIZE        1024
#define MAX_LOCAL_PACKET_SIZE   200

#define TIMESTAMP_SYNC_INTERVAL  900000 // 30 mins
*/


#ifdef __cplusplus
 extern "C" {
#endif


 struct TCP{
    char* ip;
     uint16_t port;
   };
   struct UDP{
     char* ip;
     uint16_t port;
   };
   struct Modem{
     uint16_t baudrate;
   };
   struct Network{
     bool connected;
     Modem modem;
     TCP tcp;
     UDP udp;
   };

 struct ST_MACHINE{
		uint8_t previous_state;
		uint8_t actual_state;
		uint8_t event;
		uint32_t timeout;
		uint8_t tries;
};

class NETWORK{
public:

  std::queue<int> tasks;

  NETWORK();
  void     init();
  void     loop();
  bool     connect();
  bool     sleep();
  bool     isSleeping();
  bool     isConnected();
  bool     setPSM();
  bool     disconnect();
  bool     sendData(char *msg, uint16_t size);
  bool     isDataAvailable(uint8_t* sock, uint16_t* size);

  //void     network_setup(bool force = false);
  bool     network_read_check();
  void     network_keepalive();         // not useful
  string   network_address(bool prefix = false);
  int16_t  network_rssi();
  bool     getTimeZone(tm* curr_time);

private:

  bool authenticated();
  uint8_t readData(char* data, uint16_t size, uint8_t sock);
  void log(const char* text);
  void log(string text);

  void     send_autorequests();
  void     network_clear_autorequest();  // not implemented
  void     network_init_port();          // not implemented
  uint32_t network_connected_since();    // not useful, stateless communication or short duration
  //int16_t  network_read(uint8_t*);
  //uint16_t network_write(uint8_t*, uint16_t);
  //uint16_t network_write(uint8_t);
  void     network_close(uint8_t);

};

// ios
// server report


extern NETWORK network;

#ifdef __cplusplus
}
#endif


#endif /* INC_NETWORK_H_ */
