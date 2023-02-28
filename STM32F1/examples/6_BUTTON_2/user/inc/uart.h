#ifndef _UART_H
#define _UART_H

#ifdef __cplusplus
extern C {
#endif 

typedef enum {
  UART_1,
  UART_2,
  UART_3,
}UARTperip_t;

enum{
  UART_ERR_NOISE = -1,
  UART_ERR_OVERRUN = -2,
  UART_ERR_FRAMING = -3,
  UART_ERR_PARITY = -4,
};

void UART_Init(UARTperip_t idx, uint32_t baudrate);
void UART_Send(UARTperip_t idx, unsigned char val);
void UART_Send2(UARTperip_t idx, unsigned char val);
void UART_putch(UARTperip_t idx, unsigned char ch);

uint8_t UART_DataReady(UARTperip_t idx);
uint8_t UART_Recv(UARTperip_t idx);
uint8_t UART_puts(UARTperip_t idx, const char *str);
uint8_t UART_printf(UARTperip_t idx, const char *fmt, ...);

#ifdef __cplusplus
}
#endif 


#endif