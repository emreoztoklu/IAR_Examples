#include <stdio.h>
#include <stdint.h>

#include "system.h"
#include "lcdm.h"
#include "uart.h"


#define _STDIN  0
#define _STDOUT 1
#define _STDERR 2


#define size(x)  (sizeof(x) / sizeof(x[0]))

static volatile clock_t _tmTick;

static const int _GPIO_Clocks[] ={
  RCC_APB2Periph_GPIOA,
  RCC_APB2Periph_GPIOB,
  RCC_APB2Periph_GPIOC,
  RCC_APB2Periph_GPIOD,
  RCC_APB2Periph_GPIOE,
  RCC_APB2Periph_GPIOF,
  RCC_APB2Periph_GPIOG,
}; 

#define N_PORTS  (sizeof(_GPIO_Clocks)/sizeof(int))

void Sys_IoInit(void){
  int i;
  /*GPIOx Clockları Aktif edilmesi*/
  for(i = 0; i < N_PORTS; i++)
    RCC_APB2PeriphClockCmd(_GPIO_Clocks[i], ENABLE);
  
  /*Alternatif Function IO uçları aktif edildi*/
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
  
  /*JTAG özelliğinin kapatılması, SWD kullanılacak */
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
}

void Sys_ClockTick(void){    // Bu fonksiyon "stm32f10x_it.c" içerisinde çagirilacak
  //elapsed time counter     // Sistem "SysTick_Handler" her tick kesmesinde tetiklenir
  ++_tmTick;                 // oda tm_tick counterini bir artirir.
}

clock_t clock(void){
  return _tmTick;
}

void Sys_ClockInit(void){
  SysTick_Config(SystemCoreClock / CLOCKS_PER_SEC);
}


void _putch(unsigned char ch){
  LCD_putch(ch);
  
}

void Sys_ConsoleInit(void){
  LCD_init();       //LCD Modül Baslangiç
  
  
#ifndef __IAR_SYSTEMS_ICC__
setvbuf(stdout, NULL, _IONBF, 0);
#endif
}


#ifdef __IAR_SYSTEMS_ICC__
size_t __write(int handle, unsigned char *buffer, size_t size){
#else
size_t _write(int handle, unsigned char *buffer, size_t size){
#endif
  size_t nChars = 0;
  
  if(buffer == NULL)
    return 0;
  
  if(handle != _STDOUT && handle != _STDERR)
    return 0;
  
  while(size--){
    _putch(*buffer++);
    ++nChars;
  }
  return nChars;
}



