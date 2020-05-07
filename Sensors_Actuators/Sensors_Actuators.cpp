#include "Sensors_Actuators.h"
int cont;
Sensores_Atuadores::Sensores_Atuadores(PinName Psirene, PinName Pexaustor, PinName Pporta1, PinName Pporta2, PinName Pporta3, PinName Pporta4, PinName Ptporta1,
                                       PinName Ptporta2, PinName Ptporta3, PinName Ptporta4, PinName Penergia, PinName PsensorFumaca,
                                       PinName PsensorImpacto, PinName Ppanic, PinName Ptemperature) : _sirene(Psirene), _exaustor(Pexaustor), _porta1(Pporta1), _porta2(Pporta2), _porta3(Pporta3), _porta4(Pporta4), _trava_porta1(Ptporta1),
                                                                                                       _trava_porta2(Ptporta2), _trava_porta3(Ptporta3), _trava_porta4(Ptporta4), _sensorEnergia(Penergia), _sensorFumaca(PsensorFumaca),
																																																			 _sensorImpacto(PsensorImpacto),_panic(Ppanic), _temperatureSensor(Ptemperature)
{
	
	temperature_smoke = 0x22;
  impacto_energia = 0x22;
  TEMPERATURE_LIMIT = 7680;
}

void Sensores_Atuadores::porta1_fechou()
{
    porta_status[0] = 0x02;
}

void Sensores_Atuadores::porta1_abriu()
{
    porta_status[0] = 0x01;
}

void Sensores_Atuadores::porta2_fechou()
{
    porta_status[1] = 0x02;
}

void Sensores_Atuadores::porta2_abriu()
{
    porta_status[1] = 0x01;
}
void Sensores_Atuadores::porta3_fechou()
{
    porta_status[2] = 0x02;
}

void Sensores_Atuadores::porta3_abriu()
{
    porta_status[2] = 0x01;
}
void Sensores_Atuadores::porta4_fechou()
{
    porta_status[3] = 0x02;
}

void Sensores_Atuadores::porta4_abriu()
{
    porta_status[3] = 0x01;
}
void Sensores_Atuadores::sirene_alert(int n)
{
   if(n==1){
		 _sirene=1;
	 }
	 else
	 {
	 _sirene=0;
	 }
}
/*
void Sensores_Atuadores::smoke_alert()
{
    temperature_smoke &= 0xF0;
    temperature_smoke |= 0x01;

    //abrir portas
    porta1_abriu();
    porta2_abriu();
    porta3_abriu();
    porta4_abriu();

    //acionar sistema de combate a incendio - sprinkler
    _exaustor = 1;

    //enviar mensagem via GPRS, mensagem enviada ao PC??
}*/


void Sensores_Atuadores::smoke_alert(int f)
{
    

	 if(f==1){
		temperature_smoke &= 0xF0;
    temperature_smoke |= 0x01;
		//abrir portas
    porta1_abriu();
    porta2_abriu();
    porta3_abriu();
    porta4_abriu();

    //acionar sistema de combate a incendio - sprinkler
    _exaustor = 1;
		_sirene=1;
	 }
	 else if(f==2)
	 {
		temperature_smoke &= 0xF0;
    temperature_smoke |= 0x02;
	 //abrir portas
    porta1_fechou();
    porta2_fechou();
    porta3_fechou();
    porta4_fechou();

    //acionar sistema de combate a incendio - sprinkler
    _exaustor = 0;
		_sirene=0;
	 }
    
	

    //enviar mensagem via GPRS, mensagem enviada ao PC??
}

/*
void Sensores_Atuadores::impact_alert()
{
    impacto_energia &= 0x0F;
    impacto_energia |= 0x10;

    //trava portas de entrada
    porta1_fechou();
    porta2_fechou();
    //abre porta de saida
    porta3_abriu();
    porta4_abriu();
    //envia mensagem gprs
}*/

void Sensores_Atuadores::open_close_doors()
{
    if (porta_status[0] == 0x01)
    {
        _trava_porta1 = 1;
    }
    else
    {
        _trava_porta1 = 0;
    }

    if (porta_status[1] == 0x01)
    {
        _trava_porta2 = 1;
    }
    else
    {
        _trava_porta2 = 0;
    }

    if (porta_status[2] == 0x01)
    {
        _trava_porta3 = 1;
    }
    else
    {
        _trava_porta3 = 0;
    }

    if (porta_status[3] == 0x01)
    {
        _trava_porta4 = 1;
    }
    else
    {
        _trava_porta4 = 0;
    }
}

void Sensores_Atuadores::statusDoor()
{
    if (_porta1.read() == 1)
    {
        porta_current_status[0] = 0x01;
    }
    else
    {
        porta_current_status[0] = 0x02;
    }
    if (_porta2.read() == 1)
    {
        porta_current_status[1] = 0x01;
    }
    else
    {
        porta_current_status[1] = 0x02;
    }
    if (_porta3.read() == 1)
    {
        porta_current_status[2] = 0x01;
    }
    else
    {
        porta_current_status[2] = 0x02;
    }
    if (_porta4.read() == 1)
    {
        porta_current_status[3] = 0x01;
    }
    else
    {
        porta_current_status[3] = 0x02;
    }
}

uint8_t Sensores_Atuadores::compare_status(bool trava_porta, uint8_t dados)
{
    if ((trava_porta && dados == 0x01) || (!trava_porta && dados == 0x02))
    {
        return 0x01;
    }
    else
    {
        return 0x02;
    }
}

Stcomm Sensores_Atuadores::get_system_status(Stcomm RcvData)
{
    Stcomm ResultData;

    ResultData.Header = RcvData.Header;
    ResultData.pkgtype = RcvData.pkgtype;
    ResultData.data[0] = temperature_smoke;
    ResultData.data[1] = impacto_energia;
    strcpy(ResultData.timestamp, RcvData.timestamp);

    return ResultData;
}

Stcomm Sensores_Atuadores::set_door(Stcomm RcvData)
{
    Stcomm ResultData;

    ResultData.Header = RcvData.Header;
    ResultData.pkgtype = RcvData.pkgtype;

    porta_status[RcvData.data[0] - 1] = RcvData.data[1];

    if (RcvData.data[0] == 0x01)
    {
        ResultData.data[0] = compare_status((bool)_trava_porta1, RcvData.data[1]);
    }
    else if (RcvData.data[0] == 0x02)
    {
        ResultData.data[0] = compare_status((bool)_trava_porta2, RcvData.data[1]);
    }
    else if (RcvData.data[0] == 0x03)
    {
        ResultData.data[0] = compare_status((bool)_trava_porta3, RcvData.data[1]);
    }
    else if (RcvData.data[0] == 0x04)
    {
        ResultData.data[0] = compare_status((bool)_trava_porta4, RcvData.data[1]);
    }

    //TODO TRABALHAR DADOS PARA FUNÇÃO QUE IRA LIBERAR AS PORTAS
    strcpy(ResultData.timestamp, RcvData.timestamp);
    //Confirma se a ação foi concluida mandando no data[0] OK:0X01 NOK:0X02

    return ResultData;
}

Stcomm Sensores_Atuadores::get_door(Stcomm RcvData)
{
    Stcomm ResultData;

    ResultData.Header = RcvData.Header;
    ResultData.pkgtype = RcvData.pkgtype;
    ResultData.data[0] = RcvData.data[0];                           //DOOR NUMBER CAN BE 0X01 0X02 0X03 0X04
    ResultData.data[1] = porta_current_status[RcvData.data[0] - 1]; //IRRELEVANTE
    //TODO função que vai pegar o status da porta
    strcpy(ResultData.timestamp, RcvData.timestamp);
    //ENVIO NO DATA[0]:DATA NUMBER E NO DATA[1]: DOOR STATUS
    return ResultData;
}

Stcomm Sensores_Atuadores::get_temperature(Stcomm RcvData)
{
    Stcomm ResultData;
    ResultData.Header = RcvData.Header;
    ResultData.pkgtype = RcvData.pkgtype;

    ResultData.data[1] = currentTemperature & 0xFF;
    ResultData.data[0] = (currentTemperature >> 8);
    //TODO função que vai pegar o valor da temperatura e converter
    strcpy(ResultData.timestamp, RcvData.timestamp);
    return ResultData;
}
Stcomm Sensores_Atuadores::set_temperature(Stcomm RcvData)
{
    Stcomm ResultData;
    ResultData.Header = RcvData.Header;
    ResultData.pkgtype = RcvData.pkgtype;
    ResultData.data[0] = 0x01;
    ResultData.data[1] = 0x01;
    strcpy(ResultData.timestamp, RcvData.timestamp);

    TEMPERATURE_LIMIT = (((RcvData.data[0] & 0xFF) << 8) | (RcvData.data[1] & 0xFF));
    return ResultData;
}

Stcomm Sensores_Atuadores::set_actuators(Stcomm RcvData)
{
    Stcomm ResultData;
    ResultData.Header = RcvData.Header;
    ResultData.pkgtype = RcvData.pkgtype;

    if (RcvData.data[0] == 0x01)
    {
        _exaustor = 1;
    }
    else if (RcvData.data[0] == 0x02)
    {
        _exaustor = 0;
    }

    if (RcvData.data[1] == 0x01)
    {
        _sirene = 1;
    }
    else if (RcvData.data[1] == 0x02)
    {
        _sirene = 0;
    }

    if ((exaustor == 1 && RcvData.data[0] == 0x01) || (exaustor == 0 && RcvData.data[0] == 0x02)) //Confirma se a ação foi concluida mandando no data[0] OK:0X01 NOK:0X02 PARA O FAN
    {
        ResultData.data[0] = 0x01;
    }
    else
    {
        ResultData.data[0] = 0x02;
    }
    if ((sirene == 1 && RcvData.data[1] == 0x01) || (sirene == 0 && RcvData.data[1] == 0x02)) //Confirma se a ação foi concludia mandando no data[1] OK:0X01 NOK:0X02 PARA O BUZZER
    {
        ResultData.data[1] = 0x01;
    }
    else
    {
        ResultData.data[1] = 0x02;
    }

    strcpy(ResultData.timestamp, RcvData.timestamp);
    return ResultData;
}

Stcomm Sensores_Atuadores::get_signal_level(Stcomm RcvData)
{
    Stcomm ResultData;
    ResultData.Header = RcvData.Header;
    ResultData.pkgtype = RcvData.pkgtype;
    ResultData.data[0] = 0x15; //IRRELEVANTE
    ResultData.data[1] = 0x55; //IRRELEVANTE
    //TODO FUNÇÃO QUE IRA LER O VALOR DO SINAL
    strcpy(ResultData.timestamp, RcvData.timestamp);
    //ENVIO NO DATA[0]:O BYTE CORRESPONDENTE EM RELAÇÃO AO SINAL
    return ResultData;
}
Stcomm Sensores_Atuadores::upddate_firmware(Stcomm RcvData)
{
    Stcomm ResultData;
    ResultData.Header = RcvData.Header;
    ResultData.pkgtype = RcvData.pkgtype;
    ResultData.data[0] = 0x01; //IRRELEVANTE//Enviar no data[0] pra confirmar comando recebido;
    ResultData.data[1] = 0x30; //IRRELEVANTE
    strcpy(ResultData.timestamp, RcvData.timestamp);
    return ResultData;
}
Stcomm Sensores_Atuadores::set_comm_mode(Stcomm RcvData, bool isETHmode, bool isConnecttoServer, EthernetFunctions *ethe)
{
    Stcomm ResultData;
    if (isETHmode)
    {

        ResultData.Header = RcvData.Header;
        ResultData.pkgtype = RcvData.pkgtype;
        ResultData.data[0] = 0x30; //IRRELEVANTE//Enviar no data[0] pra confirmar comando recebido;
        ResultData.data[1] = 0x30; //IRRELEVANTE
        strcpy(ResultData.timestamp, RcvData.timestamp);
    }
    else
    {
        ResultData.Header = RcvData.Header;
        ResultData.pkgtype = RcvData.pkgtype;
        ResultData.data[0] = 0x30; //IRRELEVANTE//Enviar no data[0] pra confirmar comando recebido;
        ResultData.data[1] = 0x30; //IRRELEVANTE

        if (isConnecttoServer == true)
        {
            strcpy(ResultData.timestamp, ethe->ipAddress);
        }
    }
    return ResultData;
}

Stcomm Sensores_Atuadores::unknown_response(Stcomm RcvData)
{
    Stcomm ResultData;
    ResultData.Header = RcvData.Header;
    ResultData.pkgtype = 0x00; //Para Unknown Response Envio de 0x00 no pkgtype
    ResultData.data[0] = RcvData.data[0];
    ResultData.data[1] = RcvData.data[1];
    strcpy(ResultData.timestamp, RcvData.timestamp);
    return ResultData;
}

void Sensores_Atuadores::inicializa()
{

    porta_status[0] = 0x02;
    porta_status[1] = 0x02;
    porta_status[2] = 0x02;
    porta_status[3] = 0x02;
}

void Sensores_Atuadores::temperature_power()
{
    _temperatureSensor.startConversion(); // start temperature conversion
    currentTemperature = _temperatureSensor.read();

    if (currentTemperature > TEMPERATURE_LIMIT + 768 || currentTemperature < TEMPERATURE_LIMIT - 768)
    {
        temperature_smoke &= 0x0F; //clear current temperature alert
        temperature_smoke |= 0x10; //set alert to on
    }
    else
    {
        temperature_smoke &= 0x0F; //clear current temperature alert
        temperature_smoke |= 0x20; //set alert to on
    }

    if (_sensorEnergia.read() < POWER_LIMIT)
    {
        impacto_energia &= 0xF0; //clear current power alert
        impacto_energia |= 0x01; //set alert to on
    }
    else
    {
        impacto_energia &= 0xF0; //clear current power alert
        impacto_energia |= 0x02; //set alert to on
    }
}

/*void Sensores_Atuadores::all_functions()
{
    statusDoor();
    open_close_doors();

    if (_sensorFumaca.read())
    {
        smoke_alert();
    }

    if (!_sensorImpacto.read())
    {
        impact_alert();
    }
}*/
void Sensores_Atuadores::all_functions()
{
    
    statusDoor();
    open_close_doors();

    if (_sensorFumaca.read()==1)
    {
        smoke_alert(1);
			  cont=0;
			  
    }
		else if(_sensorFumaca.read()==0 and cont==0){
			smoke_alert(2);
		  cont++;
		}

    /*if (!_sensorImpacto.read())
    {
        impact_alert();
    }*/
}
