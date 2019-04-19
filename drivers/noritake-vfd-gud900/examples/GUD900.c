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

#include "GUD900.h"
#include "GUD_I2C.h"
#include "GUD_SPI.h"
#include "GUD_USART.h"
#include <stddef.h>

//Interface definitions
#define ASYNCH 0
#define SPI 1
#define I2C 2

//Display variables.
unsigned WIDTH;
unsigned HEIGHT;
unsigned LINES; // The number of character lines present on the display.
int interface;

/**
Initializes the Noritake GUD series module.

@param  width   The width of the display in dots.
@param  height  The height of the display in dots.
@return none
*/
void GUD_Init(unsigned width, unsigned height){
        WIDTH = width;
        HEIGHT = height;
        LINES = height / 8;
        
	GUD_print(0x1b);
	GUD_print(0x40);
}

/**
The universal print function for this library.
Prints bytes based on the interface value set by each interface's initialization function.

@param  data The data to be sent.
@return none
*/
void GUD_print(char data){
  switch(interface){
  case ASYNCH:
    writeUART(data);
    break;
  case SPI:
    writeSPI(data);
    break;
  case I2C:
    GUD_I2C_Write(data);
    break;
  default:
    break;
  }
}

/**
Performs a backspace on the GUD display.

@return none
*/
void GUD_back(){
  GUD_print(0x08);
}

/**
Moves the cursor forward one position on the GUD display.

@return none
*/
void GUD_forward(){
  GUD_print(0x09);
}

/**
Performs a line feed on the GUD display.

@return none
*/
void GUD_lineFeed(){
  GUD_print(0x0a);
}

/**
Brings the cursor to its home position (top left) on the GUD display.

@return none
*/
void GUD_home(){
  GUD_print(0x0b);
}

/**
Performs a carriage return on the GUD display.

@return none
*/
void GUD_carriageReturn(){
  GUD_print(0x0d);
}

/**
Performs a carriage return and line feed on the GUD display.

@return none
*/
void GUD_crlf(){
  //Carriage Return
  GUD_print(0x0d);
  //Line Feed
  GUD_print(0x0a);
}

/**
Sends an x and y coordinate to the GUD display.

@param  x       The desired x coordinate.
@param  y       The desired y coordinate.
@return none
*/
void GUD_XY(unsigned x, unsigned y){
	y /= 8;
        GUD_print(x);
        GUD_print(x >> 8);
        GUD_print(y);
        GUD_print(y >> 8);
}

/**
Sends an x and y coordinate to the GUD display.

@param  x       The desired x coordinate.
@param  y       The desired y coordinate.
@return none
*/
void GUD_XY1(unsigned x, unsigned y){
  GUD_print(x);
  GUD_print(x >> 8);
  GUD_print(y);
  GUD_print(y >> 8);
}

/**


@return none
*/
void GUD_usCommand(){
  GUD_print(0x1f);
  GUD_print(0x28);
}

/**
Moves the cursor to a specific location on the GUD module.

@param  x       The desired x coordinate.
@param  y       The desired y coordinate.
@return none
*/
void GUD_setCursor(unsigned x, unsigned y){
  GUD_print(0x1f);
  GUD_print('$');
  GUD_XY(x, y);
}

/**
Clears the screen of the GUD module.

@return none
*/
void GUD_clearScreen(){
  GUD_print(0x0c);
}

/**
Turns the cursor on.

@return none
*/
void GUD_cursorOn(){
  GUD_print(0x1f);
  GUD_print('C');
  GUD_print(0x01);
}

/**
Turns the cursor off.

@return none
*/
void GUD_cursorOff(){
  GUD_print(0x1f);
  GUD_print('C');
  GUD_print(0x00);
}

/**
Resets the GUD series module.

@return none
*/
void GUD_reset(){
	//TODO
}

void GUD_8by16DotMode(){
  GUD_usCommand();
  GUD_print(0x67);
  GUD_print(0x01);
  GUD_print(0x02);
}

/**
Enables the use of multiple-byte characters.

@param  enable  Enable or disable multiple byte characters.
@return none
*/
void GUD_useMultiByteChars(uint8_t enable){
  GUD_usCommand();
  GUD_print('g');
  GUD_print(0x02);
  GUD_print(enable);
}

/**
Sets the multiple byte character set to be used.

Code for each character set:
<ul>
<li> 0x00 - Japanese
<li> 0x01 - Korean
<li> 0x02 - Simplified Chinese
<li> 0x03 - Traditional Chinese
</ul>

@param  code    The character set code to be used.
@return none
*/
void GUD_setMultiByteCharSet(uint8_t code){
  GUD_usCommand();
  GUD_print('g');
  GUD_print(0x0f);
  GUD_print(code);
}

/**
Enables or disables the use of custom characters.

@param  enable  Enable of disable custom characters.
@return none
*/
void GUD_useCustomChars(uint8_t enable){
  GUD_print(0x1b);
  GUD_print('%');
  GUD_print(enable);
}

/**
Helper function for GUD900_defineCustomChar.

@param src The source data array to be used.
@param col The column of pixels to get.
@return uint8_t The desired column.
*/
uint8_t GUD_getColumn(uint8_t *src, int col){
    uint8_t out = 0;
    
    for(int i = 0; i < 8; i++){
		if(src[i] & (1 << (4 - col))){
			out += 1 << (7 - 1);
		}
    }
    return out;
}

/**
Character address starts at 0x20 and ends at 0x27.
<ul>
<li> 0 - GUD900 5x7 Format
<li> 1 - GUD900 7x8 Format
<li> 2 - CUU Format
</ul>

To print custom character, write the code of the character
to the display after using this function.

Data format for custom characters:

5x7 character
<ul>
<li> 0bX0000000
<li> 0bX0000000
<li> 0bX0000000
<li> 0bX0000000
<li> 0bX0000000
</ul> 

7x8 character
<ul>
<li> 0b00000000
<li> 0b00000000
<li> 0b00000000
<li> 0b00000000
<li> 0b00000000
<li> 0b00000000
<li> 0b00000000
</ul>

<ul>
<li> X = don't care
<li> LSB is the bottom of the character.
<li> MSB is the top of the character.
<li> First byte of data is left most column of the character.
<li> Last byte of data is the right most column of the character.
</ul> 

@param   code   The code for the character being defined.
@param   format The format of the character being defined.
@param   *data  The data for the character being defined.
@returns none
*/
void GUD_defineCustomChar(uint8_t code, uint8_t format, uint8_t *data){
	int j = 0;
        int count;
        
        GUD_print(0x1b);
	GUD_print('&');
	GUD_print(0x01);
	GUD_print(code);
	GUD_print(code);
        
	switch(format){
		case 0: //GUD9005x7Format
			GUD_print(0x05);
                        count = 5;
                        while(count != 0){
                          GUD_print(data[j]);
                          j++;
                          count--;
                        }
			break;
		case 1: //GUD9007x8Format
			GUD_print(0x07);
                        count = 7;
                        while(count != 0){
                          GUD_print(data[j]);
                          j++;
                          count--;
                        }
			break;
		case 2: //CUUFormat
			GUD_print(0x05);
			for(int i = 0; i < 5; i++){
				GUD_print(GUD_getColumn(data, i));
			}
			break;
	}
}

/**
Deletes a previously defined custom character.

@param  code    Address of the custom character to be deleted.
@return none
*/
void GUD_deleteCustomChar(uint8_t code){
  GUD_print(0x1b);
  GUD_print('?');
  GUD_print(0x01);
  GUD_print(code);
}

/**
Sets the desired ASCII variant to be used.
Also called "International font select" in the GUD software manual.

Font codes:
<ul>
<li> 0x00 - America
<li> 0x01 - France
<li> 0x02 - Germany
<li> 0x03 - England
<li> 0x04 - Denmark 1
<li> 0x05 - Sweden
<li> 0x06 - Italy
<li> 0x07 - Spain 1
<li> 0x08 - Japan
<li> 0x09 - Norway
<li> 0x0A - Denmark 2
<li> 0x0B - Spain 2
<li> 0x0C - Latin America
<li> 0x0D - Korea
</ul>

@param  code   The code of the ASCII variant to be used.
@return none
*/
void GUD_setAsciiVariant(uint8_t code){
	if(code < 0x0d){
          GUD_print(0x1b);
          GUD_print('R');
          GUD_print(code);
	}
}

/**
Sets the desired character set.

Character set codes:
<ul>
<li> 0x00 - PC437(USA-Euro std)
<li> 0x01 - Katakana - Japanese
<li> 0x02 - PC850 (Multilingual)
<li> 0x03 - PC860 (Portuguese)
<li> 0x04 - PC863 (Canadian-French)
<li> 0x05 - PC865 (Nordic)
<li> 0x10 - WPC1252 (Latin)
<li> 0x11 - PC866 (Cryllic #2)
<li> 0x12 - PC852 (Latin 2)
<li> 0x13 - PC858 (Eastern European)
</ul>

@param  code   The code for the desired character set.
@return none
*/
void GUD_setCharSet(uint8_t code){
	if(code < 0x05 || (0x10 <= code && code <= 0x13)){
          GUD_print(0x1b);
          GUD_print('t');
          GUD_print(code);
	}
}

/**
Sets the scroll mode on the GUD module.

Scroll modes:
<ul>
<li> 0x01 - Wrapping mode
<li> 0x02 - Vertical scroll mode
<li> 0x03 - Horizontal scroll mode
</ul>
@param  mode   The desired scroll mode.
@return none
*/
void GUD_setScrollMode(uint8_t mode){
  GUD_print(0x1f);
  GUD_print(mode);
}

/**
Sets the horizontal scroll speed on the GUD series module.

Speed parameter specifics:
<ul>
<li> 0x00 <= speed <= 0x1f
<li> speed = 0x00 : Instantaneous Speed
<li> speed = 0x01 : IntTime / 2 dots
<li> speed = 0x02 - 0x1f : (n-1) * IntTime / dot
</ul>

@param  speed  The desired horizontal scroll speed.
@return none
*/
void GUD_setHorizScrollSpeed(uint8_t speed){
  GUD_print(0x1f);
  GUD_print('s');
  GUD_print(speed);
}

/**
Turns the display inversion OFF.

@return none
*/
void GUD_invertOff(){
  GUD_print(0x1f);
  GUD_print('r');
  GUD_print(0x00);
}

/**
Turns the display inversion ON.

@return none
*/
void GUD_invertOn(){
  GUD_print(0x1f);
  GUD_print('r');
  GUD_print(0x01);
}

/**
Sets the composition mode of the GUD module.

Composition modes:
<ul>
<li> 0x00 - Normal display write (not mixture display)
<li> 0x01 - OR display write
<li> 0x02 - AND display write
<li> 0x03 - XOR display write
</ul>

@param  mode  The desired composition mode.
@return none
*/
void GUD_setCompositionMode(uint8_t mode){
  GUD_print(0x1f);
  GUD_print('w');
  GUD_print(mode);
}

/**
Sets the screen brightness of the GUD module.

Screen brightness levels:
<ul>
<li> 0x01 - 12.5%
<li> 0x02 - 25.0%
<li> 0x03 - 37.5%
<li> 0x04 - 50.0%
<li> 0x05 - 62.5%
<li> 0x06 - 75.0%
<li> 0x07 - 87.5%
<li> 0x08 - 100%
</ul>

@param  level The desired brightness level.
@return none
*/
void GUD_setScreenBrightness(unsigned level){
	if(level == 0){
		GUD_displayOff();
	}else if(level <= 100){
		GUD_displayOn();
                GUD_print(0x1f);
		GUD_print('X');
		GUD_print((level * 10 + 120) / 125);
	}
}

/**
Suspends the program and waits for a user defined amount of time.

@param  wait    The amount of time for the MCU to wait.
@return none
*/
void GUD_wait(uint8_t wait){
  GUD_usCommand();
  GUD_I2C_Write('a');
  GUD_I2C_Write(0x01);
  GUD_I2C_Write(wait);
}

/**
Scrolls the screen based on these parameters.

@param  x       The amount of pixels for the screen to scroll horizontally.
@param  y       The amount of pixels for the screen to scroll vertically.
@param times    The number of times the screen will scroll?
@param speed    The speed at which scrolling occurs.
@return none
*/
void GUD_scrollScreen(unsigned x, unsigned y, unsigned times, uint8_t speed){
        unsigned pos = (x * LINES) + (y / 8);
        GUD_usCommand();
        GUD_print('a');
        GUD_print(0x10);
        GUD_print(pos);
        GUD_print(pos >> 8);
        GUD_print(times);
        GUD_print(times >> 8);
        GUD_print(speed);
}

/**
Resets the blink settings on the module to all zeros.

@return none
*/
void GUD_blinkScreenOff(){
  	GUD_usCommand();
	GUD_print('a');
	GUD_print(0x11);
	GUD_print(0x00);
	GUD_print(0x00);
	GUD_print(0x00);
	GUD_print(0x00);
}
/**
Blinks the GUD series module screen based on user input parameters.

@param  enable   Enables or disables screen blinking.
                 0x00 - Normal display
                 0x01 - Blink display (alternatively Normal and Blank display)
                 0x02 - Blink display (alternatively Normal and Reverse display)
@param  reverse  Enables or disables the reverse blinking pattern.
@param  onTime   The time that the display stays ON during blinking
@param  offTtime The time that the displays stays OFF during blinking.
@return none
*/
void GUD_blinkScreenOn(uint8_t enable, uint8_t reverse, uint8_t onTime, uint8_t offTime, uint8_t cycles){
  GUD_usCommand();
  GUD_print('a');
  GUD_print(0x11);
  GUD_print(enable? (reverse? 2: 1): 0);
  GUD_print(onTime);
  GUD_print(offTime);
  GUD_print(cycles);
}

/**
Turns the display OFF.

@return none
*/
void GUD_displayOff(){
  GUD_usCommand();
  GUD_print('a');
  GUD_print(0x40);
  GUD_print(0x00);
}

/**
Turns the display ON.

@return none
*/
void GUD_displayOn(){
  GUD_usCommand();
  GUD_print('a');
  GUD_print(0x40);
  GUD_print(0x01);
}

/**
Sets the screen saver mode for the GUD series module.

Screen saver modes:
<ul>
<li> 0x00 - Display power OFF
<li> 0x01 - Display power ON
<li> 0x02 - All dots OFF
<li> 0x03 - All dots ON
<li> 0x04 - Repeat blink display with normal and reverse display
</ul>

@param  mode    The desired screensaver mode. (see ScreenSaver)
@return none
*/
void GUD_screenSaver(uint8_t mode){
  GUD_usCommand();
  GUD_print('a');
  GUD_print(0x40);
  GUD_print(mode);
}

/**
Sets the font style for the GUD series module.

@param  proportional Enable or disable a proportional font style.
@param  evenSpacing  Enable or disable even spacing between characters.
@return none
*/
void GUD_setFontStyle(uint8_t proportional, uint8_t evenSpacing){
  GUD_usCommand();
  GUD_print('g');
  GUD_print(0x03);
  GUD_print(proportional * 2 + evenSpacing);
}

/**
Sets the font size for the GUD series module.

@param  x
@param  y
@param  tall 
@return none
*/
void GUD_setFontSize(uint8_t x, uint8_t y, uint8_t tall){
	if(x <= 4 && y <= 2){
          GUD_usCommand();
          GUD_print('g');
          GUD_print(0x40);
          GUD_print(x);
          GUD_print(y);
          GUD_usCommand();
          GUD_print('g');
          GUD_print(0x01);
          GUD_print(tall + 1);
	}
}

/**
Selects the current window.

@param  window  The desired window to select. (0-4)
@return none
*/
void GUD_selectWindow(uint8_t window){
	if(window <= 4){
          GUD_print(0x10 + window);
	}
}
/**
Defines a specific window for the GUD series module.

@param  window  The number of the window to be created. (1-4)
@param  x       The x coordinate of the new window.
@param  y       The y coordinate of the new window.
@param  width   The width of the new window.
@param  height  The height of the new window.
@return none
*/
void GUD_defineWindow(uint8_t window, unsigned x, unsigned y, unsigned width, unsigned height){
  GUD_usCommand();
  GUD_print('w');
  GUD_print(0x02);
  GUD_print(window);
  GUD_print(0x01);
  GUD_XY(x, y);
  GUD_XY(width, height);
}

/**
Deletes a previously defined window.

@param  window  The number of the window to be deleted. (1-4)
@return none
*/
void GUD_deleteWindow(uint8_t window){
  GUD_usCommand();
  GUD_print('w');
  GUD_print(0x02);
  GUD_print(window);
  GUD_print(0x00);
  GUD_XY(0, 0);
  GUD_XY(0, 0);
}

/**
Joins all of the screens into one base screen.

@return none
*/
void GUD_joinScreens(){
  GUD_usCommand();
  GUD_print('w');
  GUD_print(0x10);
  GUD_print(0x01);
}

/**
Separates all of the screens into four separate screens.

@return none
*/
void GUD_separateScreens(){
  GUD_usCommand();
  GUD_print('w');
  GUD_print(0x10);
  GUD_print(0x00);
}

/**
Calculate the minimum value between two given values.

@param  x Value 1.
@param  y Value 2.
@return The smaller integer.
*/
int min(int x, int y){
  int result = (x < y)? x : y;
  return result;
}

/**
Draw a filled rectangle on the display.

@param  x0 The x coordinate of the top left hand corner of the rectangle.
@param  y0 The y coordinate of the top left hand corner of the rectangle
@param  x1 The x coordinate of the bottom right hand corner of the rectangle.
@param  y1 The y coordinate of the bottom right hand corner of the rectangle.
@param  on Enable or disable the rectangle drawing.
@return none
*/
void GUD_fillRect(unsigned x0, unsigned y0, unsigned x1, unsigned y1, uint8_t on){
  x0 = min(WIDTH, x0);
  x1 = min(WIDTH, x1);
  y0 = min(HEIGHT, y0);
  y1 = min(HEIGHT, y1);
  if(y1 <= y0 || x1 <= x0){
    return;
  }
  uint8_t bufw = 8;
  uint8_t bufh = (y1 - y0 + 7) / 8 * 8;
  uint8_t *buf = (uint8_t*)malloc(bufh / 8 * bufw);
  for(unsigned x = 0; x < x1 - x0; x += bufw){
    uint8_t part = (x + bufw < x1 - x0)? bufw: (x1 - x0) - x;
    if(on){
      for(uint8_t col = 0; col < part; col++){
        for(uint8_t py = y0 % 8; py < y0 % 8 + min(y1 - y0, 8); py++){
          buf[col * bufh / 8] |= 1 << (7 - py);
        }
        for(uint8_t row = (y0 + 7) / 8; row < y1 / 8; row++){
          buf[row - y0 / 8 + col * bufh / 8] = 0xff;
        }
        if(y0 / 8 != y1 / 8){
          for(uint8_t py = 0; py < y1 % 8; py++){
            buf[(y1 - y0)/8 + col * bufh / 8] |= 1 << (7 - py);
          }
        }
      }
      GUD_setCursor(x + x0, y0);
      GUD_drawImage(bufw, bufh, buf);
    }
  }
}

/**
Draw a bitmap image on the display.
This function is character line specific, not dot specific.

@param  width  The width of the bitmap image.
@param  height The height of the bitmap image.
@param  data   The byte data of the bitmap image.
@return none
*/
void GUD_drawImage(unsigned width, uint8_t height, uint8_t *data){
  if(height > HEIGHT){
    return;
  }
  GUD_usCommand();
  GUD_print('f');
  GUD_print(0x11);
  GUD_XY(width, height);
  GUD_print(0x01);
  for(unsigned i = 0; i < (height / 8) * width; i++){
    GUD_print(data[i]);
  }
}

/**
Draw a bitmap image on the display.
This function is dot specific, not character line specific.

@param  x The x coordinate for the top left hand corner of the image.
@param  y The y coordinate for the top left hand corner of the image.
@param  width The width of the image.
@param  height The height of the image.
@param  data The byte data of the image.
@return none
*/
void GUD_drawDotUnitImage(unsigned x, uint8_t y, unsigned width, uint8_t height, uint8_t *data){
  GUD_usCommand();
  GUD_print('d');
  GUD_print(0x21);
  GUD_XY1(x, y);
  GUD_XY1(width, height);
  GUD_print(0x01);
  for(unsigned i = 0; i < (height / 8) * width; i++){
    GUD_print(data[i]);
  }
}
/**
Print a string of characters at any give XY coordinate on screen.

@param  x      X coordinate of the top left hand corner of the first character.
@param  y      Y coordinate of the top left hand corner of the first character.
@param  buffer The string of characters to be printed.
@param  len    The length of the passed in string of characters.
@return none
*/
void GUD_printDotUnitChar(unsigned x, uint8_t y, uint8_t *buffer, uint8_t len){
  GUD_usCommand();
  GUD_print('d');
  GUD_print(0x30);
  GUD_XY(x, y * 8);
  GUD_print(0x00);
  GUD_print(len);
  while(len--){
    GUD_print(*buffer++);
  }
}

/**
This function stores an image into a specified address in FROM.

Definable area:
<ul>
<li> 0x00000000 <= bit image data memory address <= 0x0007ffff
<li> 0x00000000 <= bit image data length <= 0x00080000
</ul>

@param aL     Bit image data memory address, lower byte
@param aH     Bit image data memory address, upper byte
@param aE     Bit image data memory address, extension byte
@param length Bit image data length
@param lE     Bit image data length, extension byte
@param data   Bit image data
@return none
*/
void GUD_FROMImageDefinition(uint8_t aL, uint8_t aH, uint8_t aE, unsigned length, uint8_t lE, uint8_t *data){
  GUD_print(0x1f);
  GUD_print(0x28);
  GUD_print(0x65);
  GUD_print(0x10);
  GUD_print(aL);
  GUD_print(aH);
  GUD_print(aE);
  GUD_print(length);
  GUD_print(length >> 8);
  GUD_print(lE);
  while(length--){
    GUD_print(*data++);
  }
}

/**
Prints an image that was stored in FROM.

@param x       The x coordinate of the top left hand corner of the image.
@param y       The y coordinate of the top left hand corner of the image.
@param memory  The type of memory the image is stored in.
@param aL      Bit image data memory address, lower byte.
@param aH      Bit image data memory address, upper byte.
@param aE      Bit image data memory address, extension byte.
@param yL      Bit image defined, Y size, lower byte. 
@param yH      Bit image defined, Y size, upper byte.
@param xOffset Image data offset x.
@param yOffset Image data offset y.
@param xSize   Bit image display X size.
@param ySize   Bit image display Y size.
@return none
*/
void GUD_drawFROMImage(unsigned x, unsigned y, uint8_t memory, uint8_t aL, uint8_t aH, uint8_t aE, uint8_t yL, uint8_t yH, unsigned xOffset, unsigned yOffset, unsigned xSize, unsigned ySize){
  GUD_print(0x1f);
  GUD_print(0x28);
  GUD_print(0x64);
  GUD_print(0x20);
  GUD_XY1(x, y);
  GUD_print(memory);
  GUD_print(aL);
  GUD_print(aH);
  GUD_print(aE);
  GUD_print(yL);
  GUD_print(yH);
  GUD_XY1(xOffset, yOffset);
  GUD_XY1(xSize, ySize);
  GUD_print(0x01);
}

/**
Enter user setup mode.

@return none
*/
void GUD_enterUserSetupMode(){
  GUD_print(0x1f);
  GUD_print(0x28);
  GUD_print(0x65);
  GUD_print(0x01);
  GUD_print(0x49);
  GUD_print(0x4e);
}

/**
End user setup mode.

@return none
*/
void GUD_endUserSetupMode(){
  GUD_print(0x1f);
  GUD_print(0x28);
  GUD_print(0x65);
  GUD_print(0x02);
  GUD_print(0x4f);
  GUD_print(0x55);
  GUD_print(0x54);
}

/**
Function to show the ON/OFF status for all touch switches.

Module response will be:
1. Identifier - 0x10 - 1 byte
2. Switch number - InfoDatLen - 1 byte
3. Information ON/OFF - 0x00 through 0xff - InfoDatLen bytes

@return none
*/
void GUD_I2C_touchStatusReadAll(){
	GUD_print(0x1f);
	GUD_print(0x4b);
	GUD_print(0x10);
}

/**
Function to show the ON/OFF status for an individual switch.
<ul>
<li> 0x00 <= switchNum <= MAX_NUM_SWITCHES
<li> switchNum = 0x00 : Switch 1
<li> switchNum = 0x01 : Switch 2
<li> 			|
<li> 			|
<li> switchNum = n - 1 : Switch n
</ul>

Module response will be:
1. Identifier - 0x11 - 1 byte
2. Switch number - 0x00 through SwNumMax - 1 byte
3. Information ON/OFF - 0x00 through 0xff - 1 byte

<ul>
<li> 0x00 : Switch OFF
<li> 0x01 : Switch ON
</ul>

@param  switchNum  The number of the switch to be read.
@return none
*/
void GUD_I2C_touchStatusRead(uint8_t switchNum){
	GUD_print(0x1f);
	GUD_print(0x4b);
	GUD_print(0x11);
	GUD_print(switchNum);
}

/**
Function to set the Touch Switch status read mode

Settings:
<ul>
<li> mode = 0x00 : Manual transmit mode (Send only in response to read command)
<li> mode = 0x01 : Automatic transmit mode 1 (All Touch Switch status)
<li> mode = 0x02 : Automatic transmit mode 2 (Individual Touch Switch status)
</ul>

@param  mode  The touch mode to be used.
@return none
*/
void GUD_I2C_touchSet(uint8_t mode){
	GUD_print(0x1f);
	GUD_print(0x4b);
	GUD_print(0x18);
	GUD_print(mode);
}

/**
Function to send the response level for all switches.

Module response will be:
1. Identifier - 0x14 - 1 byte
2. Switch number - SwNumMax - 1 byte
3. Information ON/OFF - ResLevel - SwMax bytes

@return none
*/
void GUD_I2C_touchLevelRead(){
	GUD_print(0x1f);
	GUD_print(0x4b);
	GUD_print(0x14);
}

/**
Function to set the touch switch internal parameters.

Settings:
<ul>
<li> mode = 0x00 : Touch Sensitivity Level Setting
<li> 	0x00 <= value <= 0x07
<li> 	value = 0x00 : Threshold value is 12.5%
<li> 	value = 0x01 : Threshold value is 25.0%
<li> 	value = 0x02 : Threshold value is 37.5%
<li> 	value = 0x03 : Threshold value is 50.0%
<li> 	value = 0x04 : Threshold value is 62.5%
<li> 	value = 0x05 : Threshold value is 75.0%
<li> 	value = 0x06 : Threshold value is 87.5%
<li> 	value = 0x07 : Threshold value is 100%
<li> mode = 0x01 : Sampling Time Setting (ON decision)
<li> 	0x00 <= value <= 0xfe
<li> mode = 0x02 : Sampling Time Setting (OFF decision)
<li> 	0x00 <= value <= 0xfe
<li> mode = 0x03 : Calibration Period Setting
<li> 	0x00 <= value <= 0x64
</ul>

@param  mode  The desired touch setting to change.
@param  value The value used to change the touch setting.
@return none
*/
void GUD_I2C_touchChangeParam(uint8_t mode, uint8_t value){
	GUD_print(0x1f);
	GUD_print(0x4b);
	GUD_print(0x70);
	GUD_print(mode);
	GUD_print(value);
}

/**
Function to set the GPIO ports on the GUD unit to be inputs or outputs.

Settings:
<ul>
<li> portSetting = 0x00 (all input) to 0x0f (all output)
<li> The portSetting parameter is bit-wise to the ports present on the module.
</ul>

@param  portSetting The desired port setting, input or output.
@return none
*/
void GUD_IOPortSetting(uint8_t portSetting){
  GUD_print(0x1f);
  GUD_print(0x28);
  GUD_print(0x70);
  GUD_print(0x01);
  GUD_print(0x00);
  GUD_print(portSetting);
}

/**
Function to set the output value on the GPIO ports.

Settings:
<ul>
<li> 0x00 < output data value < 0x0f
</ul>

@param  portValue The desired value to put on the port.
@return none
*/
void GUD_IOPortOutput(uint8_t portValue){
  GUD_print(0x1f);
  GUD_print(0x28);
  GUD_print(0x70);
  GUD_print(0x10);
  GUD_print(0x00);
  GUD_print(portValue);
}

/**
Function to request the GUD module to read the values present on the GPIO ports.

@return none
*/
void GUD_IOPortInput(){
  GUD_print(0x1f);
  GUD_print(0x28);
  GUD_print(0x70);
  GUD_print(0x20);
  GUD_print(0x00);
}