#include <stdio.h>
#include <stdint.h>

#include "system.h"
#include "io.h"
#include "lcdm.h"


// Data: Saf veri (Komut yada Karakter kodu olabilir)  RS = 0,1
// Komut Mode:    RS = 0
// Karakter Mode: RS = 1
#define RS_COMMAND_MODE (0)
#define RS_CHARACTER_MODE (!RS_COMMAND_MODE)


/* LCD modüle 4-bit senkron veri gönderir
 veri: karakter kodu veya komut kodu
 gönderilecek veri parameterenin düsük anlamli 4-bitinde yer alir.
*/
static void LCD_SendDataL(unsigned char ch){
  // 1) Data Setup (verinin hazirlanmasi)
  IO_Write(IOP_LCD_DB4, (ch & 1) != 0);
  IO_Write(IOP_LCD_DB5, (ch & 2) != 0); 
  IO_Write(IOP_LCD_DB6, (ch & 4) != 0); 
  IO_Write(IOP_LCD_DB7, (ch & 8) != 0); 

  //2) Clock generation (saat isareti "onaylama" gönderimi)
  IO_Write(IOP_LCD_E, PIN_SET);
  DelayUs(3);
  IO_Write(IOP_LCD_E, PIN_RESET);
  DelayUs(100); 
}

/*
LCD modüle 4-bit senkron komut gönderme*/
static void LCD_SendCmdL(unsigned char ch){
  IO_Write(IOP_LCD_RS, RS_COMMAND_MODE);   //RS = 0 (komut)
  LCD_SendDataL(ch);
}

/* 
LCD modüle 8-bit Veri gönderir
2 adimda: Önce yüksek 4-bit, sonra düsük 4-bit*/
void LCD_SendData(unsigned char ch){  
  LCD_SendDataL(ch>>4);
  LCD_SendDataL(ch);
}

/*
LCD modüle 8-bit Komut gönderir
2 adimda: Önce yüksek 4-bit, sonra düsük 4-bit*/
void LCD_SendCmd(unsigned char ch){  
  IO_Write(IOP_LCD_RS, RS_COMMAND_MODE);    // RS = 0 (Komut mode)
  LCD_SendData(ch);                         // 8-bit veri gönderimi
}

/*
LCD modüle 8-bit Karakter gönderir
2 adimda: Önce yüksek 4-bit, sonra düsük 4-bit*/
void LCD_PutChar(unsigned char ch){  
  IO_Write(IOP_LCD_RS, RS_CHARACTER_MODE);  // RS = 1 (karakter mode)
  LCD_SendData(ch);                         // 8-bit veri gönderimi
}


//Ekrani temizleme komutu
void LCD_Clear(void){
  LCD_SendCmd(0x01);
  DelayMs(5);           // LCD ekranin temizlenmesi için geçen sürenin dokumanda 2ms olarak belirtilmis
                        // garanti olsun diye 5ms olarak belirledik
}

void LCD_DisplayOn(unsigned char mode){
  if (mode == 0U){
    LCD_SendCmd(0x08 | LCD_MODE_ON | LCD_MODE_CURSOR |LCD_MODE_BLINK);
  }
  else{
    LCD_SendCmd(0x08 | mode);      //  Table 6 Instructions
  }
}

void LCD_SetCursor(unsigned char pos){
    LCD_SendCmd(0x80 | pos);
}

void LCD_init(void){  
  IO_Init(IOP_LCD_RS, IO_MODE_OUTPUT);
  IO_Write(IOP_LCD_E, PIN_RESET);
  IO_Init(IOP_LCD_E, IO_MODE_OUTPUT);
  
  IO_Init(IOP_LCD_DB4, IO_MODE_OUTPUT);
  IO_Init(IOP_LCD_DB5, IO_MODE_OUTPUT);
  IO_Init(IOP_LCD_DB6, IO_MODE_OUTPUT);
  IO_Init(IOP_LCD_DB7, IO_MODE_OUTPUT);
 /*********************************************************************/ 
  // 4 bit arayüz ile baslatma  --> Hitachi page 47  "4-Bit Interface"
  
  DelayMs(100);                 //Wait for more than 40 ms after VCC rises to 2.7 V
  LCD_SendCmdL(0x03);           //BF cannot be checked before this instruction. 
                                //Function set  D7 D6 D5 D4 => 0011 => 0x03 4-Bit Interface
  
  DelayMs(5);                   //Wait for more than 4.1 ms                    
  LCD_SendCmdL(0x03);           //BF cannot be checked before this instruction.
                                //Function set  D7 D6 D5 D4 => 0011 => 0x03 4-Bit Interface
  
  //DelayUs(100);                 //Wait for more than 100 µs
  
  LCD_SendCmd(0x32);            // Önce yüksek anlamli 0x03 gidecek //BF cannot be checked before this instruction. 
                                //Function set  D7 D6 D5 D4 => 0011 => 0x03 4-Bit Interface
  
                                //sonr a 0x02 arkasina gidecek
  
  LCD_SendCmd(0x28);            //N:Number of Lines,  F: Font size(big or small)
                                //Önce 2 degeri gönderilecek sonra N ve F degeleri ne olacak onlar gidecek  N:1 (ikisatir) F:0 (küçükfont)
  
  LCD_DisplayOn(0);             //LCD Display Off:0
  
  LCD_Clear();                  // LCD yi temizle
  
  LCD_SendCmd(0x06);            // I/D: increment/Decremant I:1 D:0 cursor'un otomatik artmasi yada azaltilmasi durumu
                                // S: lcd'yi shift etme modu   0 1 I/D S => 0110 => 0x06
 /*********************************************************************/
  //En son LCD off konumda oldugundan ekranin açilmasi gerekliki karakterleri görelim
  LCD_DisplayOn(LCD_MODE_ON);   //LCD Display ON
} 

//LCD modül printf destek fonksiyonu
void LCD_putch(unsigned char ch){
  switch(ch){
    case'\r':
      LCD_SetCursor(0);
      break; 
    case'\n':
      LCD_SetCursor(0x40);
      break;
    case'\f':
      LCD_Clear();
      break;
    default:
      LCD_PutChar(ch);
      break;
  }
}





