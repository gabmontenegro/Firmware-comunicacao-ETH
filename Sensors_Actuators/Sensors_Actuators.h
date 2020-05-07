#ifndef SENSORS_ACTUATORS
#define SENSORS_ACTUATORS
#include "mbed.h"
#include "DS1820.h"
#include "GmarketComm.h"
#include "EthernetConnect.h"

#define POWER_LIMIT 2
#define TICKERTIMER 2000
#define DEFINEDRESETTIME 100
#define sirene          PB_1
#define exaustor        PC_5
#define porta1          PE_2       //interrupt
#define porta2          PE_3       //interrupt
#define porta3          PE_4       //interrupt
#define porta4          PE_5       //interrupt
#define sensorEnergia   PA_1       //fotoacoplador
#define sensorFumaca    PC_3       //interrupt
#define sensorImpacto   PA_11      //ativo a baixo, interrupt
#define panic           PC_2       //interrupt
#define trava_porta1    PE_9
#define trava_porta2    PE_8
#define trava_porta3    PE_7
#define trava_porta4    PB_2
#define temperatures    PE_14



//char temperature_smoke = 0x22;
//char impacto_energia = 0x22;
//static  uint16_t TEMPERATURE_LIMIT = 7680;

class Sensores_Atuadores
{
public:
  Sensores_Atuadores(PinName Psirene, PinName Pexaustor, PinName Pporta1, PinName Pporta2, PinName Pporta3, PinName Pporta4, PinName Ptporta1,
                     PinName Ptporta2, PinName Ptporta3, PinName Ptporta4, PinName Penergia, PinName PsensorFumaca,
                     PinName PsensorImpacto, PinName Ppanic, PinName Ptemperature);

  //void impact_alert();
  void open_close_doors();
  void statusDoor();
  //void smoke_alert();
  void smoke_alert(int f);
  void porta3_fechou();
  void porta4_fechou();
  void porta3_abriu();
  void porta4_abriu();
  void inicializa();
  void temperature_power();
  void all_functions();
  void sirene_alert(int n);

  Stcomm get_system_status(Stcomm RcvData);
  Stcomm set_door(Stcomm RcvData);
  Stcomm get_door(Stcomm RcvData);
  Stcomm get_temperature(Stcomm RcvData);
  Stcomm set_temperature(Stcomm RcvData);
  Stcomm set_actuators(Stcomm RcvData);
  Stcomm get_signal_level(Stcomm RcvData);
  Stcomm upddate_firmware(Stcomm RcvData);
  Stcomm set_comm_mode(Stcomm RcvData, bool isETHmode, bool isConnecttoServer, EthernetFunctions *ethe);
  Stcomm unknown_response(Stcomm RcvData);
  
  char porta_status[4];
  char porta_current_status[4];
  char temperature_smoke;
  char impacto_energia;
  uint16_t currentTemperature;

  uint16_t TEMPERATURE_LIMIT;

  uint8_t compare_status(bool trava_porta, uint8_t dados);

  DigitalOut _sirene;
  DigitalOut _exaustor;
  DS1820 _temperatureSensor;
  AnalogIn _sensorEnergia;
  DigitalIn _sensorFumaca;  //interrupt
  DigitalIn _sensorImpacto; //ativo a baixo, interrupt
  InterruptIn _panic;       //interrupt

private:
  void porta1_fechou();
  void porta2_fechou();
  void porta1_abriu();
  void porta2_abriu();
  DigitalIn _porta1; //interrupt
  DigitalIn _porta2; //interrupt
  DigitalIn _porta3; //interrupt
  DigitalIn _porta4; //interrupt
  DigitalOut _trava_porta1;
  DigitalOut _trava_porta2;
  DigitalOut _trava_porta3;
  DigitalOut _trava_porta4;
};
#endif
