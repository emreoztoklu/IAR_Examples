#ifndef _SYSTEM_H
#define _SYSTEM_H

#include "stm32f10x.h"


/*DEFİNES*/
#define CLOCKS_PER_SEC 1000   //1sn

#define CLOCK_FREQ   72000000
#define INSTR_FREQ   ((CLOCK_FREQ) *8 / 9)      // Bu satır önemli InstructionSet etme zamanı  64000000
#define US_CYCLES    ((INSTR_FREQ) / 1000000)   // 64000000/1000000 = 64
#define MS_CYCLES    ((INSTR_FREQ) / 1000)      // 64000000/1000    = 64000

/*function like makro*/
#define DelayUs(us)   __delay((us) * US_CYCLES)    //__delay  delay.s assembly dosyasından geliyor
#define DelayMs(ms)   __delay((ms) * MS_CYCLES)


/*TYPEDEFS*/
typedef uint32_t clock_t;          

/*
 not : 2^32bit/1000persec =>x ,  x/86400 = y gün  
 not2: (60*60*24 = 86400) 
*/

/*function declerations */
clock_t clock(void);
void Sys_ClockInit(void);
void Sys_ClockTick(void);

void Sys_IoInit(void);
void Sys_ConsoleInit(void);

void __delay(uint32_t ncy);



#endif 

/*
NOTE:  INSTRUCTION FREQUENCY  : 
1 assembly komutunun işletilme süresinin bilinmesi
bunda da 1 osc süresi gerçekten 1 komut işletilme süresine gelip gelmediğinin ölçülmesi gerekir

Soru: Birim komut çalışma süresi  == 1 Tcy   ? 
  
Tcy = 1 / Fosc 


*/