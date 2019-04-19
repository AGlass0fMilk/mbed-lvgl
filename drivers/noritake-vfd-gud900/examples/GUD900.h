/**
@author   Cody Johnson
@email    cody.ele@noritake.com (Feel free to ask questions)
@website  noritake-elec.com
@version  v1.0
@ide      IAR
@brief    GU-D command library for STM32F4xx and Noritake GU-D Series Modules

@vertatim
Provide the GU-D commands between STM32F4Discovery and Noritake GU-D modules.
@endverbatim
*/

#ifndef GUD900_H
#define GUD900_H

#include "stm32f4xx.h"
#include "stm32f4xx_i2c.h"

void GUD_Init(unsigned width, unsigned height);
void GUD_print(char data);
void GUD_back();
void GUD_forward();
void GUD_lineFeed();
void GUD_home();
void GUD_carriageReturn();
void GUD_crlf();
void GUD_XY(unsigned x, unsigned y);
void GUD_XY1(unsigned x, unsigned y);
void GUD_usCommand();
void GUD_setCursor(unsigned x, unsigned y);
void GUD_clearScreen();
void GUD_cursorOn();
void GUD_cursorOff();
void GUD_reset();
void GUD_8by16DotMode();
void GUD_useMultiByteChars(uint8_t enable);
void GUD_setMultiByteCharSet(uint8_t code);
void GUD_useCustomChars(uint8_t enable);
uint8_t GUD_getColumn(uint8_t *src, int col);
void GUD_defineCustomChar(uint8_t code, uint8_t format, uint8_t *data);
void GUD_deleteCustomChar(uint8_t code);
void GUD_setAsciiVariant(uint8_t code);
void GUD_setCharSet(uint8_t code);
void GUD_setScrollMode(uint8_t mode);
void GUD_setHorizScrollSpeed(uint8_t speed);
void GUD_invertOff();
void GUD_invertOn();
void GUD_setCompositionMode(uint8_t mode);
void GUD_setScreenBrightness(unsigned level);
void GUD_wait(uint8_t wait);
void GUD_scrollScreen(unsigned x, unsigned y, unsigned times, uint8_t speed);
void GUD_blinkScreenOff();
void GUD_blinkScreenOn(uint8_t enable, uint8_t reverse, uint8_t onTime, uint8_t offTime, uint8_t cycles);
void GUD_displayOff();
void GUD_displayOn();
void GUD_screenSaver(uint8_t mode);
void GUD_setFontStyle(uint8_t proportional, uint8_t evenSpacing);
void GUD_setFontSize(uint8_t x, uint8_t y, uint8_t tall);
void GUD_selectWindow(uint8_t window);
void GUD_defineWindow(uint8_t window, unsigned x, unsigned y, unsigned width, unsigned height);
void GUD_deleteWindow(uint8_t window);
void GUD_joinScreens();
void GUD_separateScreens();
void GUD_fillRect(unsigned x0, unsigned y0, unsigned x1, unsigned y1, uint8_t on);
void GUD_drawImage(unsigned width, uint8_t height, uint8_t *data);
void GUD_drawDotUnitImage(unsigned x, uint8_t y, unsigned width, uint8_t height, uint8_t *data);
void GUD_printDotUnitChar(unsigned x, uint8_t y, uint8_t *buffer, uint8_t len);
void GUD_FROMImageDefinition(uint8_t aL, uint8_t aH, uint8_t aE, unsigned length, uint8_t lE, uint8_t *data);
void GUD_drawFROMImage(unsigned x, unsigned y, uint8_t memory, uint8_t aL, uint8_t aH, uint8_t aE, uint8_t yL, uint8_t yH, unsigned xOffset, unsigned yOffset, unsigned xSize, unsigned ySize);
void GUD_enterUserSetupMode();
void GUD_endUserSetupMode();
void GUD_I2C_touchStatusReadAll();
void GUD_I2C_touchStatusRead(uint8_t switchNum);
void GUD_I2C_touchSet(uint8_t mode);
void GUD_I2C_touchLevelRead();
void GUD_I2C_touchChangeParam(uint8_t mode, uint8_t value);
void GUD_IOPortSetting(uint8_t portSetting);
void GUD_IOPortOutput(uint8_t portValue);
void GUD_IOPortInput();
#endif