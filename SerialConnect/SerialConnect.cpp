#include "SerialConnect.h"

SerialConnect::SerialConnect(){}
	
Serial usb_2(USBTX, USBRX);//to show data in serial monitor PA_9 PA_10
Serial m6e_2(SERIAL_TX, SERIAL_RX);// to send and receive data from device PA_2 PA_3

 
void SerialConnect::readFromSerial(char *message)
{
	 if(m6e_2.readable()) {
        m6e_2.scanf("%s", message);
				//m6e_2.
		 usb_2.printf("lido:%s", message);
        }
	 //return *message;
}

void SerialConnect::sendToSerial(char *packet)
{
	usb_2.printf("Enviado ao M6E_NANO: \n%sEND\n", packet);
	m6e_2.printf("%s", packet);
	
	//usb_2.printf("\n%s\n", packet);
		
}



