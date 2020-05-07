#ifndef SERIAL_CONNECT_H
#define SERIAL_CONNECT_H

#include "mbed.h"


class SerialConnect
{
  public:
	  SerialConnect();
    void readFromSerial(char *message);

    void sendToSerial(char *packet);
    
  private:
    
};
#endif
