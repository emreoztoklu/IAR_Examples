#ifndef _IO_H
#define _IO_H

#ifdef __cplusplus
extern C {
#endif 

#define PIN_SET 1
#define PIN_RESET (!PIN_SET)

#define TURN_OFF 1
#define TURN_ON (!TURN_OFF)

#include "stm32f10x.h"  

// I/O MODLARI
typedef enum { 
  IO_MODE_INPUT      = GPIO_Mode_IN_FLOATING,
  IO_MODE_OUTPUT     = GPIO_Mode_Out_PP,
  IO_MODE_ALTERNATE  = GPIO_Mode_AF_PP,
  IO_MODE_ANALOG     = GPIO_Mode_AIN,
  IO_MODE_INPUT_PD   = GPIO_Mode_IPD, 
  IO_MODE_INPUT_PU   = GPIO_Mode_IPU,
  IO_MODE_OUTPUT_OD  = GPIO_Mode_Out_OD,
  IO_MODE_ALTERNATE_OD=GPIO_Mode_AF_OD,
} IO_MODE;

typedef enum {
  IO_PORT_A,
  IO_PORT_B,
  IO_PORT_C,
  IO_PORT_D,
  IO_PORT_E,
  IO_PORT_F,
  IO_PORT_G,
} IO_PORTS;


typedef struct {
  int port;
  int pin;
}IO_PIN;

/************************************/
typedef enum {
  IOP_LED,          //Bluepill Ledpin
  
  /*LCD Modül Pin Tanimlamari*/
  IOP_LCD_RS,
  IOP_LCD_E,
  IOP_LCD_DB4,
  IOP_LCD_DB5,
  IOP_LCD_DB6,
  IOP_LCD_DB7,
  
  /*UART*/
  IOP_UART1TX,
  IOP_UART1RX,
  IOP_UART2TX,
  IOP_UART2RX,
  IOP_UART3TX,
  IOP_UART3RX,
  
  // Buttons
  IOP_BTN_SET,
  IOP_BTN_UP,
  IOP_BTN_DN,
} IO_IDX;

/************************************/
#ifdef _IOS_
  
IO_PIN _ios[]={
  {IO_PORT_C, 13},   //Bluepill GPIOC l13.pin LED
  
  /*LCD Modül Pin Tanimlamari*/
  {IO_PORT_B, 9},   // IOP_LCD_RS 
  {IO_PORT_B, 8},   // IOP_LCD_E
  {IO_PORT_B, 7},   // IOP_LCD_DB4
  {IO_PORT_B, 6},   // IOP_LCD_DB5
  {IO_PORT_B, 5},   // IOP_LCD_DB6
  {IO_PORT_B, 4},   // IOP_LCD_DB7
  
  /*UART port & pin  uçlarý*/
  {IO_PORT_A, 9},
  {IO_PORT_A, 10},
  {IO_PORT_A, 2},
  {IO_PORT_A, 3},
  {IO_PORT_B, 10},
  {IO_PORT_B, 11},
  
    // Buttons
  { IO_PORT_A, 0 },
  { IO_PORT_A, 1 },
  { IO_PORT_A, 2 },
};

GPIO_TypeDef *_GPIO_Ports[]={
  GPIOA,
  GPIOB,
  GPIOC,
  GPIOD,
  GPIOE,
  GPIOF,
  GPIOG,
};
#else
extern IO_PIN   _ios[];
extern GPIO_TypeDef *GPIO_Ports[];
#endif 
/************************************/
void IO_Init(IO_IDX idx, IO_MODE mode);
void IO_Write(IO_IDX idx, uint8_t val);
uint8_t IO_Read(IO_IDX idx);

#ifdef __cplusplus
}
#endif 

#endif 


