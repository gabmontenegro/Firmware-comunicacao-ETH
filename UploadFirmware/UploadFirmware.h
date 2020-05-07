#ifndef _UPLOAD_FIRMWARE_
#define _UPLOAD_FIRMWARE_

#include "mbed.h"
#include "eeprom_flash.h"

class UploadFirmware{
    public:
    static bool isBootMode();
    static void GotoBootMode();
    static void ResetSystem();
    
    private:
    static void JumpToBootloader();
};
#endif
