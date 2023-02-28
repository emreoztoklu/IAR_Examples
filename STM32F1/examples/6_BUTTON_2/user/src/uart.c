#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>

#include "io.h"
#include "system.h"

#include "uart.h"

#define SZ_PRNBUF 256


typedef struct{
  uint8_t ioTX;             // TX pin io index
  uint8_t ioRX;             // RX pin io index
  
  uint32_t ckUART;          // clock maske de�eri
  USART_TypeDef *pUSART;    // CMSIS periph. pointer tu structer
}UARTcfg_t;


static UARTcfg_t _uCfg[] = {
  {IOP_UART1TX, IOP_UART1RX, RCC_APB2Periph_USART1, USART1},
  {IOP_UART2TX, IOP_UART2RX, RCC_APB1Periph_USART2, USART2},
  {IOP_UART3TX, IOP_UART3RX, RCC_APB1Periph_USART3, USART3},
};

/*
  UART �evresel birimi ba�latma
  param_1 : UART index (U1:0, U2:1, U3:2)
  param_2 : baudrate
  this init work 8-N-1
*/
void UART_Init(UARTperip_t idx, uint32_t baudrate){

  USART_InitTypeDef uInit;
  
  // 1) I/O u�lar� yap�land�r�l�r
  // Tx u�u alternate func  push_pull olmal�
  IO_Init(_uCfg[idx].ioTX, IO_MODE_ALTERNATE);
 
  // Rx u�u input_floating
  IO_Init(_uCfg[idx].ioRX, IO_MODE_INPUT);
  
  // 2) UART �evresel birim i�in clock s�lan�r
  if(idx == UART_1)
    //RCC_APB2Periph_USART1
    RCC_APB2PeriphClockCmd(_uCfg[idx].ckUART, ENABLE);
  else 
    //RCC_APB1Periph_USART2 & USART3;
    RCC_APB1PeriphClockCmd(_uCfg[idx].ckUART, ENABLE);
  
  // 3) Init yap�s� ba�lat�lacak
  uInit.USART_BaudRate = baudrate;
  uInit.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  uInit.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
  uInit.USART_Parity = USART_Parity_No;
  uInit.USART_StopBits = USART_StopBits_1;
  uInit.USART_WordLength = USART_WordLength_8b;
  
  USART_Init(_uCfg[idx].pUSART, &uInit);
  
  // 4) �evresel birim aktive edilecek (Perih->ON)
  USART_Cmd(_uCfg[idx].pUSART, ENABLE);
}

//UART �evresel birimine veri g�nderme
void UART_Send(UARTperip_t idx, unsigned char val){
  
  // 1) TDR y�kleme i�in uygun mu?
  while(!USART_GetFlagStatus(_uCfg[idx].pUSART, USART_FLAG_TXE));
  
  // 2) y�klemeyi yapmaya ba�la
  // Not: Verinin gitmesini beklemiyoruz, sadece y�kleme yap�p ��k�yoruz
  USART_SendData(_uCfg[idx].pUSART, val);
}

void UART_Send2(UARTperip_t idx, unsigned char val){
  // 1) y�klemeyi yapmaya ba�la
  // Not: Verinin gitmesini beklemiyoruz, sadece y�kleme yap�p ��k�yoruz
  USART_SendData(_uCfg[idx].pUSART, val);
  
    // 2) veri gidene kadar bekle
  while(!USART_GetFlagStatus(_uCfg[idx].pUSART, USART_FLAG_TC));
}

uint8_t UART_DataReady(UARTperip_t idx){
  return USART_GetFlagStatus(_uCfg[idx].pUSART, USART_FLAG_RXNE);
}

uint8_t UART_Recv(UARTperip_t idx){
  int8_t ret;
    
  USART_TypeDef *pUART = _uCfg[idx].pUSART;

  while(!UART_DataReady(idx));
  
  if(USART_GetFlagStatus(pUART, USART_FLAG_ORE | USART_FLAG_NE |
                                USART_FLAG_FE | USART_FLAG_PE)) {
  
    if(USART_GetFlagStatus(pUART, USART_FLAG_NE))
        ret = UART_ERR_NOISE;
    else if(USART_GetFlagStatus(pUART, USART_FLAG_ORE))
       ret = UART_ERR_OVERRUN;
    if(USART_GetFlagStatus(pUART, USART_FLAG_FE))
       ret = UART_ERR_FRAMING;
    if(USART_GetFlagStatus(pUART, USART_FLAG_PE))
       ret = UART_ERR_PARITY;
       
    USART_ReceiveData(pUART);
  }
  else 
    ret = USART_ReceiveData(pUART);

  return ret;
}

void UART_putch(UARTperip_t idx, unsigned char ch){
  if(ch == '\n')
    UART_Send2(idx, '\r');
  UART_Send2(idx, ch);
}

//str'deki  string ifadesini idx olarak belirtilen UART'a g�nderilir
uint8_t UART_puts(UARTperip_t idx, const char *str){
  uint8_t i = 0;
  
  while(str[i]){
    UART_putch(idx,str[i++]);
  }
  return i;
}

uint8_t UART_printf(UARTperip_t idx, const char *fmt, ...){

  va_list args;
  char str[SZ_PRNBUF];
  
  va_start(args, fmt);
  vsnprintf(str, SZ_PRNBUF, fmt, args);
  
  return UART_puts(idx, str);
}
 