#ifndef _LCDM_H
#define _LCDM_H

//Hitachi  HD44780  command page 25 
#define LCD_MODE_BLINK      1       // D :display (D) on/off,
#define LCD_MODE_CURSOR     2       // C :cursor on/off (C),
#define LCD_MODE_ON         4       // B :blinking of cursor position character (B)


void LCD_init(void);


void LCD_Clear(void);
void LCD_DisplayOn(unsigned char mode);
void LCD_SetCursor(unsigned char pos);


void LCD_PutChar(unsigned char ch);


#endif


