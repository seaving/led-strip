#ifndef __HAL_EEPROM_H__
#define __HAL_EEPROM_H__

#include "hal_sys.h"
#include "drv_eeprom.h"

#define hal_eeprom_erase_page()                     drv_eeprom_erase_page()
#define hal_eeprom_write_byte(offset, data_set)     drv_eeprom_write_byte(offset, data_set)
#define hal_eeprom_read_byte(offset)                drv_eeprom_read_byte(offset)
#define hal_eeprom_write(offset, wbuf, len)         drv_eeprom_write(offset, wbuf, len)

#endif
