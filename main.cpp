#include "mbed.h"
#include "EthernetConnect.h"
#include "string.h"

/* MACROS */
#define BUFFSIZE 512
#define MAX_PACKET_SIZE 256

/* LED de status */
DigitalOut led=PA_8;

/* DEBUG pela serial */
//Serial usb(USBTX, USBRX); 									// to show data in serial monitor  PA_2 PA_3

/* Leitor RFID */
Serial m6e(SERIAL_TX, SERIAL_RX); 						// to send and receive data from device PA_9 PA_10
InterruptIn m6e_rx(SERIAL_RX);

/* Interfaces de Comunicação */
SPI spi(PA_7, PA_6, PA_5);										// mosi, miso, sclk
WIZnetInterface ethernet(&spi, PA_4, PC_4); 	// spi, cs, reset
EthernetFunctions Eth;

/* Setando parametros da rede */
const char *IP_Addr = "192.168.11.194";
const char *IP_Subnet = "255.255.255.0";
const char *IP_Gateway = "192.168.11.1";
unsigned char MAC_Addr[6] = {0x00, 0x08, 0xDC, 0x12, 0x34, 0x56};

/* BUFFERS relacionados a comunicação Ethernet */
uint8_t respRcv[BUFFSIZE] = {0};												// buffer de resposta, construído na interrupção
char cmdSent[MAX_PACKET_SIZE] = {0};										// buffer de comando enviado pelo m6e para processamento

/* Variaveis utilizadas na função Rx_interrupt(), globais para que o tamanho do buffer seja visível na função principal */
uint8_t a;				// caracter coletado no m6e 
int size = 0;			// armazenar o tamanho do buffer respRcv
int cont = 0;     // contador para percorrer buffer respRcv na interrupção
int flag = 0;     // sinalização para habilitar e/ou desabilitar a interrupção

/*********************************************************************/ 

/*******========== Protótipo das funções =============== ************/ 
bool checkOpcode(uint8_t ch);
void zeraVetor(char *v);
time_t read_rtc(void);
void Rx_interrupt();

/*********************************************************************/ 

/***********  Função Principal  **************************************/ 
int main()
{
	//setando baud rate do leitor
	m6e.baud(115200);
		
	//atrelando interrupção a borda de descida do leitor m6e
	m6e_rx.fall(&Rx_interrupt);
	
	//comando para visualizar o debug por usb quando necessário
	attach_rtc(&read_rtc, NULL, NULL, NULL);
	
	int n = 0;
	int i = 0;
	
	/* Led da placa acende e apaga indicando que o firmware esta pronto para conexao */
	led = 1;
	Eth.ConnectServer(MAC_Addr, &ethernet);
	led = 0;
	
	while(true)
	{
		
		Eth.ConnectClient();	
		
		while(Eth.Cliente.is_connected() == true )
		{
				
				n = Eth.Cliente.receive(cmdSent, sizeof(cmdSent));
				
				if(n > 0)
				{
					for(i = 0; i < n; i++)
					{
						m6e.putc(cmdSent[i]);
						
					}
				}
				if(flag == 1) //enviando o buffer de resposta que foi construido no Rx_interrupt()
				{
					
					//Foi mudada a função de Eth.Cliente.send_all para Eth.write(Melhorou o tempo de comunicação)
					Eth.write((char*)respRcv, size);
					zeraVetor((char*)respRcv);
					flag = 0;
					size = 0;
					cont = 0;
					//depois de enviar a resposta do comando por ethernet, habilita a interrupção novamente
					m6e_rx.enable_irq(); 
				
				}
				
			  //Checando para desconectar o leitor
				if(Eth.Cliente.is_fin_received())
						Eth.Cliente.close();
		}
	}
}

/*********************************************************************/ 

/***********  Definição das Funções  **********/ 
//Checando se o codigo de operação do pacote é válido
bool checkOpcode(uint8_t ch)
{
	
	if( ch >= 0x01 && ch <= 0x0F )
	{
		return true;
	}
	else if( ch >= 0x21 && ch <= 0x2A )
		return true;
	else if( ch >= 0x2D && ch <= 0x2F )
		return true;
	else if( ch >= 0x61 && ch <= 0x6D )
		return true;
	else if( ch >= 0x70 && ch <= 0x72 )
		return true;
	else if( ch >= 0x91 && ch <= 0x9E )
		return true;
	else if( ch == 0xC1 )
		return true;
	else if( ch == 0xC3)
		return true;
	else
		return false;
}
//Limpar buffer
void zeraVetor(char *v)
{
	int i;
	for(i = 0; i < strlen(v); i++)
			v[i] = 0;
}
time_t read_rtc(void){
    return 0;
}
//Função para manipulação da interrupção
void Rx_interrupt()
{
	//cont = 0, size = 0
	a = m6e.getc();
	//Primeiro filtro: Reconhecimento do inicio do pacote, primeira posição é 0xFF. 
	if( cont == 0 && a != 0xFF)
	{
		
	}
	else
	{
		respRcv[cont++] = a;
		//Segundo Filtro: checando os 2 próximos bytes( tamanho da mensagem, Op Code)
		if( cont > 0 && cont == respRcv[1] + 7)
		{
			if( (checkOpcode(respRcv[2]) == true) || (respRcv[2] == 0x22) || (respRcv[2] == 0x2F) || (respRcv[2] == 0x9D) )
			{
				size = respRcv[1] + 7;
				flag = 1;
				// se os 2 filtros são satisfeitos então o pacote é válido, desabilita interrupção para que
				// seja feito o envio do pacote pela função principal
				m6e_rx.disable_irq(); 
			}
			else
			{
				cont = 0;
			}
		}
		// verificando se ocorre overflow no buffer que processa a resposta, 
		// caso acontecer zera-se o buffer e os parâmetros para desprezar 
		// o pacote e esperar por um novo.
		if(cont > BUFFSIZE)
		{
				zeraVetor((char*)respRcv);
				flag = 0;
				size = 0;
				cont = 0;
		}
	}
}