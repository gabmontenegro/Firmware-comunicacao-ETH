#include "GmarketComm.h"

bool isEthernetMode()

{

    bool isEthMode;

    uint16_t hexWord = readEEPROMHalfWord(2);

    if (hexWord == 0x01)
    {
        isEthMode = true;
    }
    else
    {
        isEthMode = false;
    }

    return isEthMode;
}


