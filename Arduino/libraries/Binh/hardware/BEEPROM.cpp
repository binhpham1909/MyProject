#include "BEEPROM.h"
// init class
BEEPROM::BEEPROM(uint32_t sector)
: _sector(sector)
, _data(0)
, _size(0)
, _dirty(false)
{
}

void BEEPROM::begin(size_t size) {
  if (size <= 0)
    return;
  if (size > SPI_FLASH_SEC_SIZE)
    size = SPI_FLASH_SEC_SIZE;

  size = (size + 3) & (~3);

  if (_data) {
    delete[] _data;
  }

  _data = new uint8_t[size];
  _size = size;

  noInterrupts();
  spi_flash_read(_sector * SPI_FLASH_SEC_SIZE, reinterpret_cast<uint32_t*>(_data), _size);
  interrupts();
}

void BEEPROM::end() {
  if (!_size)
    return;

  commit();
  if(_data) {
    delete[] _data;
  }
  _data = 0;
  _size = 0;
}

bool BEEPROM::commit() {
  bool ret = false;
  if (!_size)
    return false;
  if(!_dirty)
    return true;
  if(!_data)
    return false;

  noInterrupts();
  if(spi_flash_erase_sector(_sector) == SPI_FLASH_RESULT_OK) {
    if(spi_flash_write(_sector * SPI_FLASH_SEC_SIZE, reinterpret_cast<uint32_t*>(_data), _size) == SPI_FLASH_RESULT_OK) {
      _dirty = false;
      ret = true;
    }
  }
  interrupts();

  return ret;
}

uint8_t BEEPROM::read(int address) {
  if (address < 0 || (size_t)address >= _size)
    return 0;
  if(!_data)
    return 0;

  return _data[address];
}

void BEEPROM::write(int address, uint8_t value) {
  if (address < 0 || (size_t)address >= _size)
    return;
  if(!_data)
    return;

  _data[address] = value;
  _dirty = true;
}

uint8_t * BEEPROM::getDataPtr() {
  _dirty = true;
  return &_data[0];
}
////////////////////////////////////////
///  Group function Write to EEPROM  ///
////////////////////////////////////////
template <class T> void BEEPROM::put(int StartAddress,T *storageVar){
    begin(MAX_EEPROM_SIZE);
	uint8_t * bytesToWriteEEPROM = (uint8_t *)storageVar;
	const int STORAGE_SIZE = sizeof(*storageVar);
    Serial.println(FPSTR(lb_EEPROM_WRITING));
	for (int i = 0; i < STORAGE_SIZE; i++) {
		write(StartAddress + i,bytesToWriteEEPROM[i]);
	}
    end();
};

//////////////////////////////////////
/// Group function Read to EEPROM  ///
//////////////////////////////////////
template <class T> void BEEPROM::get(int StartAddress,T *storageVar){
    begin(MAX_EEPROM_SIZE);
	uint8_t * bytesToReadEEPROM = (uint8_t *)storageVar;
	const int STORAGE_SIZE = sizeof(*storageVar);
    Serial.println(FPSTR(lb_EEPROM_READING));
	for (int i = 0; i < STORAGE_SIZE; i++) {
		bytesToReadEEPROM[i] = read(StartAddress + i);
	}
    end();
};

extern "C" uint32_t _SPIFFS_end;
BEEPROM EEPROM((((uint32_t)&_SPIFFS_end - 0x40200000) / SPI_FLASH_SEC_SIZE));
