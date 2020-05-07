#ifndef STM32_ETHERNET_CONNECT
#define STM32_ETHERNET_CONNECT

#include "mbed.h"
#include "WIZnetInterface.h"
#include "GmarketComm.h"

//1 for dinamic. 0 to static
#define USE_DHCP 1

//Port to Bind Server
#define LOOPBACKPORT 5000

class EthernetFunctions
{
public:
  // EthernetFunctions(PinName mosi,PinName miso,PinName sclk,PinName cs,PinName Reset);
  EthernetFunctions();

  /**
        * Send a string according to the websocket format (see rfc 6455)
        *
        * @param str string to be sent
        *
        * @returns the number of bytes sent
        */
  int send(char * str);
 
  int readEthernet(char *message);
  //bool ReadEthernet(char * message);
  

  bool ConnectServer(unsigned char MAC_Addr[6], WIZnetInterface *_ethernet, const char *IP_Addr = "", const char *IP_Subnet = "", const char *IP_Gateway = "");

  void ConnectClient();

  TCPSocketServer Servidor;
  TCPSocketConnection Cliente;

  char *ipAddress;
  //private:
  // SPI _spi;
  // WIZnetInterface _ethernet;
	int sendOpcode(uint8_t opcode, char * msg);
	int sendMask(char * msg);
	int sendLength(uint32_t len, char * msg);
  int read(char * buf, int len, int min_len = -1);
	int write(char * buf, int len);
	int readChar(char * pC, bool block = true);
};
#endif

