#ifndef BEEPROM_h
#define BEEPROM_h

#include "Arduino.h"
#include <stddef.h>
#include <stdint.h>
#include <string.h>

extern "C" {
#include "c_types.h"
#include "ets_sys.h"
#include "os_type.h"
#include "osapi.h"
#include "spi_flash.h"
}
#include "BDef.h"

// EEPROM
// EEPROM save string need string size + 2 byte, fisrt byte store len of string, second byte store max len of string, after are bytes of string
// EEPROM save boolean, byte need 1 byte
// EEPROM save int need 2 byte
// EEPROM save float, IPAddress need 4 byte
#define MAX_EEPROM_SIZE 512		// Lan khoi dong dau tien

const char lb_EEPROM_READING[] PROGMEM = "Reading EEPROM..";
const char lb_EEPROM_WRITING[] PROGMEM = "Writing EEPROM..";

class BEEPROM{
    public:
        BEEPROM(uint32_t sector);
        void begin(size_t size);
        uint8_t read(int address);
        void write(int address, uint8_t val);
        bool commit();
        void end();
        uint8_t * getDataPtr();
        template <class T> void put(int StartAddress,T *storageVar);
        template <class T> void get(int StartAddress,T *storageVar);
    protected:
        uint32_t _sector;
        uint8_t* _data;
        size_t _size;
        bool _dirty;
};
extern BEEPROM EEPROM;
#endif