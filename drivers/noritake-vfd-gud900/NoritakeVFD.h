/*
 * NoritakeVFD.h
 *
 *  Created on: Oct 30, 2018
 *      Author: gdbeckstein
 */

#ifndef MBED_LVGL_DRIVERS_NORITAKE_VFD_GUD900_NORITAKEVFD_H_
#define MBED_LVGL_DRIVERS_NORITAKE_VFD_GUD900_NORITAKEVFD_H_

#include "DisplayDriver.h"

class NoritakeVFD : public DisplayDriver
{
	public:

		/**
		 * Instantiates a DisplayDriver with a given DisplayInterface
		 */
		NoritakeVFD(DisplayInterface& interface,
				uint32_t height = 32, uint32_t width = 128);

		virtual ~NoritakeVFD() { }

		/**
		Initializes the Noritake GUD series module.

		@param  width   The width of the display in dots.
		@param  height  The height of the display in dots.
		@return none
		*/
		virtual void init(void);

		/**
		Performs a backspace on the GUD display.

		@return none
		*/
		void back(void);

		/**
		Moves the cursor forward one position on the GUD display.

		@return none
		*/
		void forward(void);

		/**
		Performs a line feed on the GUD display.

		@return none
		*/
		void linefeed();

		/**
		Brings the cursor to its home position (top left) on the GUD display.

		@return none
		*/
		void home();

		/**
		Performs a carriage return on the GUD display.

		@return none
		*/
		void carriage_return();

		/**
		Performs a carriage return and line feed on the GUD display.

		@return none
		*/
		void crlf();

		/**
		Sends an x and y coordinate to the GUD display.

		@param  x       The desired x coordinate.
		@param  y       The desired y coordinate.
		@return none
		*/
		void send_xy(unsigned x, unsigned y);

		/**
		Sends an x and y coordinate to the GUD display.

		@param  x       The desired x coordinate.
		@param  y       The desired y coordinate.
		@return none
		*/
		void send_xy1(unsigned x, unsigned y);

		/**
		gdbeckst: not exactly sure what this does...
		Sends 0x1f 0x28 (which is a preamble to a lot of commands)

		@return none
		*/
		void us_command();

		/**
		Moves the cursor to a specific location on the GUD module.

		@param  x       The desired x coordinate.
		@param  y       The desired y coordinate.
		@return none
		*/
		void set_cursor(unsigned x, unsigned y);

		/**
		Clears the screen of the GUD module.

		@return none
		*/
		void clear_screen();

		/**
		Turns the cursor on.

		@return none
		*/
		void cursor_on();

		/**
		Turns the cursor off.

		@return none
		*/
		void cursor_off();

		//???
		void dot_mode_8x16();

		/**
		Enables the use of multiple-byte characters.

		@param  enable  Enable or disable multiple byte characters.
		@return none
		*/
		void use_multi_byte_chars(uint8_t enable);

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
		void set_multi_byte_char_set(uint8_t code);

		/**
		Enables or disables the use of custom characters.

		@param  enable  Enable of disable custom characters.
		@return none
		*/
		void use_custom_chars(uint8_t enable);

		/**
		Helper function for define_custom_char.

		@param src The source data array to be used.
		@param col The column of pixels to get.
		@return uint8_t The desired column.
		*/
		uint8_t get_column(uint8_t *src, int col);

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
		void define_custom_char(uint8_t code, uint8_t format, uint8_t *data);

		/**
		Deletes a previously defined custom character.

		@param  code    Address of the custom character to be deleted.
		@return none
		*/
		void delete_custom_char(uint8_t code);

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
		void set_ascii_variant(uint8_t code);

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
		void set_char_set(uint8_t code);

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
		void set_scroll_mode(uint8_t mode);

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
		void set_horizontal_scroll_speed(uint8_t speed);

		/**
		Turns the display inversion OFF.

		@return none
		*/
		void invert_off();

		/**
		Turns the display inversion ON.

		@return none
		*/
		void invert_on();

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
		void set_composition_mode(uint8_t mode);

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
		void set_screen_brightness(unsigned level);

		/**
		Suspends the program and waits for a user defined amount of time.

		@param  wait    The amount of time for the MCU to wait.
		@return none
		*/
		void wait(uint8_t wait);

		/**
		Scrolls the screen based on these parameters.

		@param  x       The amount of pixels for the screen to scroll horizontally.
		@param  y       The amount of pixels for the screen to scroll vertically.
		@param times    The number of times the screen will scroll?
		@param speed    The speed at which scrolling occurs.
		@return none
		*/
		void scroll_screen(unsigned x, unsigned y, unsigned times, uint8_t speed);

		/**
		Resets the blink settings on the module to all zeros.

		@return none
		*/
		void blink_screen_off();

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
		void blink_screen_on(uint8_t enable, uint8_t reverse, uint8_t onTime, uint8_t offTime, uint8_t cycles);

		/**
		Turns the display OFF.

		@return none
		*/
		void display_off();

		/**
		Turns the display ON.

		@return none
		*/
		void display_on();

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
		void screen_saver(uint8_t mode);

		/**
		Sets the font style for the GUD series module.

		@param  proportional Enable or disable a proportional font style.
		@param  evenSpacing  Enable or disable even spacing between characters.
		@return none
		*/
		void set_font_style(uint8_t proportional, uint8_t evenSpacing);

		/**
		Sets the font size for the GUD series module.

		@param  x
		@param  y
		@param  tall
		@return none
		*/
		void set_font_size(uint8_t x, uint8_t y, uint8_t tall);

		/**
		Selects the current window.

		@param  window  The desired window to select. (0-4)
		@return none
		*/
		void select_window(uint8_t window);

		/**
		Defines a specific window for the GUD series module.

		@param  window  The number of the window to be created. (1-4)
		@param  x       The x coordinate of the new window.
		@param  y       The y coordinate of the new window.
		@param  width   The width of the new window.
		@param  height  The height of the new window.
		@return none
		*/
		void define_window(uint8_t window, unsigned x, unsigned y, unsigned width, unsigned height);

		/**
		Deletes a previously defined window.

		@param  window  The number of the window to be deleted. (1-4)
		@return none
		*/
		void delete_window(uint8_t window);

		/**
		Joins all of the screens into one base screen.

		@return none
		*/
		void join_screens();

		/**
		Separates all of the screens into four separate screens.

		@return none
		*/
		void separate_screens();

		/**
		Draw a filled rectangle on the display.

		@param  x0 The x coordinate of the top left hand corner of the rectangle.
		@param  y0 The y coordinate of the top left hand corner of the rectangle
		@param  x1 The x coordinate of the bottom right hand corner of the rectangle.
		@param  y1 The y coordinate of the bottom right hand corner of the rectangle.
		@param  on Enable or disable the rectangle drawing.
		@return none
		*/
		void fill_rect(unsigned x0, unsigned y0, unsigned x1, unsigned y1, uint8_t on);

		/**
		Draw a bitmap image on the display.
		This function is character line specific, not dot specific.

		@param  width  The width of the bitmap image.
		@param  height The height of the bitmap image.
		@param  data   The byte data of the bitmap image.
		@return none
		*/
		void draw_image(unsigned width, uint8_t height, uint8_t *data);

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
		void draw_dot_unit_image(unsigned x, uint8_t y, unsigned width, uint8_t height, uint8_t *data);

		/**
		Print a string of characters at any give XY coordinate on screen.

		@param  x      X coordinate of the top left hand corner of the first character.
		@param  y      Y coordinate of the top left hand corner of the first character.
		@param  buffer The string of characters to be printed.
		@param  len    The length of the passed in string of characters.
		@return none
		*/
		void print_dot_unit_char(unsigned x, uint8_t y, uint8_t *buffer, uint8_t len);

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
		void FROM_image_definition(uint8_t aL, uint8_t aH, uint8_t aE, unsigned length, uint8_t lE, uint8_t *data);

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
		void draw_FROM_image(unsigned x, unsigned y, uint8_t memory, uint8_t aL, uint8_t aH, uint8_t aE, uint8_t yL, uint8_t yH, unsigned xOffset, unsigned yOffset, unsigned xSize, unsigned ySize);

		/**
		Enter user setup mode.

		@return none
		*/
		void enter_user_setup_mode();

		/**
		End user setup mode.

		@return none
		*/
		void end_user_setup_mode();

		/**
		Function to show the ON/OFF status for all touch switches.

		Module response will be:
		1. Identifier - 0x10 - 1 byte
		2. Switch number - InfoDatLen - 1 byte
		3. Information ON/OFF - 0x00 through 0xff - InfoDatLen bytes

		@return none
		*/
		void touch_status_read_all();

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
		void touch_status_read(uint8_t switchNum);

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
		void touch_set(uint8_t mode);

		/**
		Function to send the response level for all switches.

		Module response will be:
		1. Identifier - 0x14 - 1 byte
		2. Switch number - SwNumMax - 1 byte
		3. Information ON/OFF - ResLevel - SwMax bytes

		@return none
		*/
		void touch_level_read();

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
		void touch_change_param(uint8_t mode, uint8_t value);

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
		void IO_port_setting(uint8_t portSetting);

		/**
		Function to set the output value on the GPIO ports.

		Settings:
		<ul>
		<li> 0x00 < output data value < 0x0f
		</ul>

		@param  portValue The desired value to put on the port.
		@return none
		*/
		void IO_port_output(uint8_t portValue);

		/**
		Function to request the GUD module to read the values present on the GPIO ports.

		@return none
		*/
		void IO_port_input();

#if MBED_USE_LVGL

		/*
		 * @brief Flush the content of the internal buffer to the specific area on the display
		 * You can use DMA or any hardware acceleration to do this operation in the background but
		 * this->flush_ready() has to be called when finished
		 * This function is required only when LV_VDB_SIZE != 0 in lv_conf.h
		 */
		virtual void flush(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const lv_color_t* color_p);

		/*
		 * @brief Write a pixel array (called 'map') to the a specific area on the display
		 * This function is required only when LV_VDB_SIZE == 0 in lv_conf.h
		 */
		virtual void map(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const lv_color_t* color_p);

		/*
		 * @brief Write a pixel array (called 'map') to the a specific area on the display
		 * This function is required only when LV_VDB_SIZE == 0 in lv_conf.h
		 */
		virtual void fill(int32_t x1, int32_t y1, int32_t x2, int32_t y2, lv_color_t color);

#if USE_LV_GPU

		/*
		 * @brief If your MCU has hardware accelerator (GPU) then you can use it to blend to memories using opacity
		 * It can be used only in buffered mode (LV_VDB_SIZE != 0 in lv_conf.h)
		 */
		virtual void gpu_blend(lv_color_t* dest, const lv_color_t* src, uint32_t length, lv_opt_t opa) { }

		/*
		 * @brief If your MCU has hardware accelerator (GPU) then you can use it to fill a memory with a color
		 * It can be used only in buffered mode (LV_VDB_SIZE != 0 in lv_conf.h)
		 */
		virtual void gpu_fill(lv_color_t* dest, uint32_t length, lv_color_t color) { }

#endif


	protected:

		void set_pixel(int32_t x, int32_t y, lv_color_t color);

#endif

	protected:

		/** The height and width of the display (in pixels) and the number of lines */
		uint32_t _height, _width, _lines;

		/** Internal buffer for bitwise image transfer */
		uint8_t _buf[512];



};



#endif /* MBED_LVGL_DRIVERS_NORITAKE_VFD_GUD900_NORITAKEVFD_H_ */
