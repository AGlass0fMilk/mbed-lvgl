/**
 * @file hx8357d_driver.c
 * @brief Brief Description
 * 
 * Detailed Description
 *
 * Link to [markdown like this](@ref PageTag)
 * Make sure you tag the markdown page like this:
 * # Page title {#PageTag}
 * 
 * <a href='MyPDF.pdf'> Link to PDF documents like this</a>
 * If you add document files, make sure to add them into a directory inside a "docs" folder
 * And then run hud-devices/tools/copy-dox-files.py 
 *
 * To use images, make sure they're in an "images" folder and follow the doxygen user manual to add images.
 * You must run copy-dox-files.py after adding images as well.
 *
 * @copyright Copyright &copy; 2018 Heads Up Display, Inc.
 *
 *  Created on: Jun 16, 2018
 *      Author: gdbeckstein
 */

#include "hx8357d_driver.h"
#include "hx8357d_registers.h"

#include "mbed_wait_api.h"

void hx8357d_init(i8080_8bit_t* obj)
{
	uint8_t buffer[8];

	hx8357d_soft_reset(obj);
	wait_ms(10);
	hx8357d_set_extc(obj);
	wait_ms(300);

	// Set RGB
	buffer[0] = 0;
	buffer[1] = 0;
	buffer[2] = 0x6;
	buffer[3] = 0x6;
	hx8357d_set_rgb(obj, buffer);

	// SetCOM (0x25)
	hx8357d_set_com(obj, 0x25);

	//SetOSC(0x68)
	hx8357d_set_osc(obj, 0x68);

	//SetPanel(0x05)
	hx8357d_set_panel(obj, 0x05);

	//SetPWR1( 0x00, 0x15, 0x1C, 0x1C, 0x83, 0xAA )
	buffer[0] = 0;
	buffer[1] = 0x15;
	buffer[2] = 0x1C;
	buffer[3] = 0x1C;
	buffer[4] = 0x83;
	buffer[5] = 0xAA;
	buffer[6] = 0x29;
	hx8357d_set_power(obj, buffer);

	//SetSTBA( 0x50, 0x50, 0x01, 0x3C, 0x1E, 0x08 )
	buffer[0] = 0x50;
	buffer[1] = 0x50;
	buffer[2] = 0x01;
	buffer[3] = 0x3C;
	buffer[4] = 0x1E;
	buffer[5] = 0x08;
	hx8357d_set_stba(obj, buffer);

	//SetCYC (0x02, 0x40, 0x00, 0x2A, 0x2A, 0x0D, 0x78)
	buffer[0] = 0x02;
	buffer[1] = 0x40;
	buffer[2] = 0x00;
	buffer[3] = 0x2A;
	buffer[4] = 0x2A;
	buffer[5] = 0x0D;
	buffer[6] = 0x78;
	hx8357d_set_cyc(obj, buffer);

	// COLMOD (0x55)
	hx8357d_set_color_mode(obj, 0x55);

	// MADCTL (0xC0)
	hx8357d_set_address_mode(obj, 0x0); //0xC0);

	//hx8357d_set_tear_on(obj);

	// TEON (0x0)
	// TEARLINE(0x00, 0x02)
	// SLPOUT
	hx8357d_exit_sleep_mode(obj);
	wait_ms(150);

	hx8357d_display_on(obj);
	wait_ms(50);
}

void hx8357d_nop(i8080_8bit_t* obj)
{
	i8080_8bit_write(obj, HX8357_NOP);
}

void hx8357d_soft_reset(i8080_8bit_t* obj)
{
	i8080_8bit_write(obj, HX8357_SWRESET);
}

uint32_t hx8357d_get_id(i8080_8bit_t* obj)
{
	uint32_t id = 0;
	i8080_8bit_write(obj, HX8357_RDDID);
	id |= (i8080_8bit_read(obj) << 16);
	id |= (i8080_8bit_read(obj) << 8);
	id |= (i8080_8bit_read(obj));
	return id;
}

uint8_t hx8357d_get_power_mode(i8080_8bit_t* obj)
{
	i8080_8bit_write(obj, HX8357B_RDPOWMODE);
	return i8080_8bit_read(obj);
}

uint8_t hx8357d_get_address_mode(i8080_8bit_t* obj)
{
	i8080_8bit_write(obj, HX8357B_RDMADCTL);
	return i8080_8bit_read(obj);
}

void hx8357d_invert_on(i8080_8bit_t* obj)
{
	i8080_8bit_write(obj, HX8357_INVON);
}

void hx8357d_invert_off(i8080_8bit_t* obj)
{
	i8080_8bit_write(obj, HX8357_INVOFF);
}

void hx8357d_set_gamma_curve(i8080_8bit_t* obj, uint8_t curve)
{
	uint8_t cmd[] = 	{ HX8357_GAMSET, curve };
	i8080_8bit_write_bytes(obj, cmd, 2);
}

void hx8357d_display_on(i8080_8bit_t* obj)
{
	i8080_8bit_write(obj, HX8357_DISPON);
}
void hx8357d_display_off(i8080_8bit_t* obj)
{
	i8080_8bit_write(obj, HX8357_DISPOFF);
}

void hx8357d_set_column_address(i8080_8bit_t* obj, uint16_t col_start, uint16_t col_end)
{
	uint8_t cmd[] = { HX8357_CASET,
			(uint8_t)((col_start & 0xFF00) >> 8),
			(uint8_t)((col_start & 0x00FF)),
			(uint8_t)((col_end & 0xFF00) >> 8),
			(uint8_t)((col_end & 0x00FF))
	};

	i8080_8bit_write_bytes(obj, cmd, 5);
}

void hx8357d_set_row_address(i8080_8bit_t* obj, uint16_t row_start, uint16_t row_end)
{
	uint8_t cmd[] = { HX8357_PASET,
				(uint8_t)((row_start & 0xFF00) >> 8),
				(uint8_t)((row_start & 0x00FF)),
				(uint8_t)((row_end & 0xFF00) >> 8),
				(uint8_t)((row_end & 0x00FF))
		};

	i8080_8bit_write_bytes(obj, cmd, 5);
}

void hx8357d_write_memory_start(i8080_8bit_t* obj)
{
	i8080_8bit_write(obj, HX8357_RAMWR);
}

void hx8357d_read_memory_start(i8080_8bit_t* obj)
{
	i8080_8bit_write(obj, HX8357_RAMRD);
}

void hx8357d_set_tear_on(i8080_8bit_t* obj)
{
	i8080_8bit_write(obj, HX8357_TEON);
}

void hx8357d_set_tear_off(i8080_8bit_t* obj)
{
	i8080_8bit_write(obj, HX8357_TEOFF);
}

void hx8357d_set_address_mode(i8080_8bit_t* obj, uint8_t mode)
{
	uint8_t cmd[] = { HX8357_MADCTL, mode };
	i8080_8bit_write_bytes(obj, cmd, 2);
}

void hx8357d_enter_idle_mode(i8080_8bit_t* obj)
{
	i8080_8bit_write(obj, HX8357_IDMON);
}

void hx8357d_exit_idle_mode(i8080_8bit_t* obj)
{
	i8080_8bit_write(obj, HX8357_IDMOFF);
}

void hx8357d_enter_sleep_mode(i8080_8bit_t* obj)
{
	i8080_8bit_write(obj, HX8357_SLPIN);
}

void hx8357d_exit_sleep_mode(i8080_8bit_t* obj)
{
	i8080_8bit_write(obj, HX8357_SLPOUT);
}
void hx8357d_set_extc(i8080_8bit_t* obj)
{
	uint8_t cmd[] = {HX8357D_SETC, 0xFF, 0x83, 0x57 };
	i8080_8bit_write_bytes(obj, cmd, 4);
}

void hx8357d_set_rgb(i8080_8bit_t* obj, uint8_t* mode)
{
	uint8_t cmd[5];
	cmd[0] = HX8357_SETRGB;
	memcpy(&cmd[1], mode, 4);
	i8080_8bit_write_bytes(obj, cmd, 5);
}

void hx8357d_set_osc(i8080_8bit_t* obj, uint8_t mode)
{
	uint8_t cmd[] = { HX8357_SETOSC, mode };
	i8080_8bit_write_bytes(obj, cmd, 2);
}

void hx8357d_set_panel(i8080_8bit_t* obj, uint8_t mode)
{
	uint8_t cmd[] = { HX8357_SETPANEL, mode };
	i8080_8bit_write_bytes(obj, cmd, 2);
}

void hx8357d_set_power(i8080_8bit_t* obj, uint8_t* mode)
{
	uint8_t cmd[8];
	cmd[0] = HX8357_SETPWR1;
	memcpy(&cmd[1], mode, 7);
	i8080_8bit_write_bytes(obj, cmd, 8);
}

void hx8357d_set_stba(i8080_8bit_t* obj, uint8_t* mode)
{
	uint8_t cmd[7];
	cmd[0] = HX8357D_SETSTBA;
	memcpy(&cmd[1], mode, 6);
	i8080_8bit_write_bytes(obj, cmd, 7);
}

void hx8357d_set_cyc(i8080_8bit_t* obj, uint8_t* mode)
{
	uint8_t cmd[8];
	cmd[0] = HX8357D_SETCYC;
	memcpy(&cmd[1], mode, 7);
	i8080_8bit_write_bytes(obj, cmd, 8);
}

void hx8357d_set_color_mode(i8080_8bit_t* obj, uint8_t mode)
{
	uint8_t cmd[] = { HX8357_COLMOD, mode };
	i8080_8bit_write_bytes(obj, cmd, 2);
}

void hx8357d_set_com(i8080_8bit_t* obj, uint8_t mode)
{
	uint8_t cmd[] = { HX8357D_SETCOM, mode };
	i8080_8bit_write_bytes(obj, cmd, 2);
}

