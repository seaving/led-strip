#ifndef __DRV_UART_H__
#define __DRV_UART_H__

#include "drv_sys.h"

void drv_uart_init(uint32_t sys_clk, uint32_t baud);
void drv_uart_send_byte(uint8_t byte);

#endif
