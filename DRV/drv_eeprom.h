#ifndef __DRV_EEPROM_H__
#define __DRV_EEPROM_H__

#include "drv_sys.h"

void drv_eeprom_erase_page(void);
void drv_eeprom_write_byte(uint16_t address, uint8_t value);
void drv_eeprom_write(uint16_t address, uint8_t *buf, uint8_t len);
uint8_t drv_eeprom_read_byte(uint16_t address);

#endif
