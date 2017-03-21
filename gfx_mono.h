/*
 * gfx_mono.h
 *
 * Created: 28-Dec-15 2:31:47 AM
 *  Author: emon1
 */ 


#ifndef GFX_MONO_H_
#define GFX_MONO_H_


#define CONFIG_SSD1306_FRAMEBUFFER


#define GFX_MONO_LCD_WIDTH              128
#define GFX_MONO_LCD_HEIGHT             32
#define GFX_MONO_LCD_PIXELS_PER_BYTE    8
#define GFX_MONO_LCD_PAGES              (GFX_MONO_LCD_HEIGHT / GFX_MONO_LCD_PIXELS_PER_BYTE)
#define GFX_MONO_LCD_FRAMEBUFFER_SIZE   ((GFX_MONO_LCD_WIDTH * GFX_MONO_LCD_HEIGHT) / GFX_MONO_LCD_PIXELS_PER_BYTE)


/** Pixel operations */
enum gfx_mono_color {
	/** Pixel is cleared */
	GFX_PIXEL_CLR = 0,
	/** Pixel is set on screen (OR) */
	GFX_PIXEL_SET = 1,
	/** Pixel is XORed */
	GFX_PIXEL_XOR = 2,
};

/** Bitmap types */
enum gfx_mono_bitmap_type {
	/** Bitmap stored in SRAM */
	GFX_MONO_BITMAP_RAM,
	/** Bitmap stored in progmem */
	GFX_MONO_BITMAP_PROGMEM
};



#define gfx_mono_draw_horizontal_line(x, y, length, color) \
gfx_mono_generic_draw_horizontal_line(x, y, length, color)

#define gfx_mono_draw_vertical_line(x, y, length, color) \
gfx_mono_generic_draw_vertical_line(x, y, length, color)

#define gfx_mono_draw_line(x1, y1, x2, y2, color) \
gfx_mono_generic_draw_line(x1, y1, x2, y2, color)

#define gfx_mono_draw_rect(x, y, width, height, color) \
gfx_mono_generic_draw_rect(x, y, width, height, color)

#define gfx_mono_draw_filled_rect(x, y, width, height, color) \
gfx_mono_generic_draw_filled_rect(x, y, width, height, \
color)

#define gfx_mono_draw_circle(x, y, radius, color, octant_mask) \
gfx_mono_generic_draw_circle(x, y, radius, color, \
octant_mask)

#define gfx_mono_draw_filled_circle(x, y, radius, color, quadrant_mask)	\
gfx_mono_generic_draw_filled_circle(x, y, radius, \
color, quadrant_mask)

#define gfx_mono_put_bitmap(bitmap, x, y) \
gfx_mono_generic_put_bitmap(bitmap, x, y)

#define gfx_mono_draw_pixel(x, y, color) \
gfx_mono_ssd1306_draw_pixel(x, y, color)

#define gfx_mono_get_pixel(x, y) \
gfx_mono_ssd1306_get_pixel(x, y)

#define gfx_mono_init()	\
gfx_mono_ssd1306_init()

#define gfx_mono_put_page(data, page, column, width) \
gfx_mono_ssd1306_put_page(data, page, column, width)

#define gfx_mono_get_page(data, page, column, width) \
gfx_mono_ssd1306_get_page(data, page, column, width)

#define gfx_mono_put_byte(page, column, data) \
gfx_mono_ssd1306_put_byte(page, column, data, false)

#define gfx_mono_get_byte(page, column)	\
gfx_mono_ssd1306_get_byte(page, column)

#define gfx_mono_mask_byte(page, column, pixel_mask, color) \
gfx_mono_ssd1306_mask_byte(page, column, pixel_mask, color)

#define gfx_mono_put_framebuffer() \
gfx_mono_ssd1306_put_framebuffer()


void gfx_mono_ssd1306_put_framebuffer(void);
void gfx_mono_ssd1306_put_page(uint8_t *data, uint8_t page,uint8_t page_offset, uint8_t width);
void gfx_mono_ssd1306_get_page(uint8_t *data, uint8_t page,uint8_t page_offset, uint8_t width);
void gfx_mono_ssd1306_init(void);
void gfx_mono_ssd1306_draw_pixel(uint8_t x, uint8_t y,uint8_t color);
uint8_t gfx_mono_ssd1306_get_pixel(uint8_t x, uint8_t y);
void gfx_mono_ssd1306_put_byte(uint8_t page, uint8_t column,uint8_t data, uint8_t force);
uint8_t gfx_mono_ssd1306_get_byte(uint8_t page, uint8_t column);
void gfx_mono_ssd1306_mask_byte(uint8_t page, uint8_t column,uint8_t pixel_mask, uint8_t color);


void gfx_mono_set_framebuffer(uint8_t *framebuffer);
void gfx_mono_framebuffer_put_page(uint8_t *data, uint8_t page,uint8_t page_offset, uint8_t width);
void gfx_mono_framebuffer_get_page(uint8_t *data, uint8_t page,uint8_t page_offset, uint8_t width);
void gfx_mono_framebuffer_draw_pixel(uint8_t x, uint8_t y,uint8_t color);
uint8_t gfx_mono_framebuffer_get_pixel(uint8_t x, uint8_t y);
void gfx_mono_framebuffer_put_byte(uint8_t page, uint8_t column,uint8_t data);
uint8_t gfx_mono_framebuffer_get_byte(uint8_t page, uint8_t column);
void gfx_mono_framebuffer_mask_byte(uint8_t page, uint8_t column,uint8_t pixel_mask, uint8_t color);




static uint8_t framebuffer[GFX_MONO_LCD_FRAMEBUFFER_SIZE];//emon
static uint8_t *fbpointer;




/**
 * \brief Initialize SSD1306 controller and LCD display.
 * It will also write the graphic controller RAM to all zeroes.
 *
 * \note This function will clear the contents of the display.
 */
void gfx_mono_ssd1306_init(void)
{
	uint8_t page;
	uint8_t column;

#ifdef CONFIG_SSD1306_FRAMEBUFFER
	gfx_mono_set_framebuffer(framebuffer);
#endif

	/* Initialize the low-level display controller. */
	ssd1306_init();

	/* Set display to output data from line 0 */
	ssd1306_set_display_start_line_address(0);

	/* Clear the contents of the display.
	 * If using a framebuffer (SPI interface) it will both clear the
	 * controller memory and the framebuffer.
	 */
	for (page = 0; page < GFX_MONO_LCD_PAGES; page++) {
		for (column = 0; column < GFX_MONO_LCD_WIDTH; column++) {
			gfx_mono_ssd1306_put_byte(page, column, 0x00, true);
		}
	}
}



/**
 * \brief Put framebuffer to LCD controller
 *
 * This function will output the complete framebuffer from RAM to the
 * LCD controller.
 *
 * \note This is done automatically if using the graphic primitives. Only
 * needed if you are manipulating the framebuffer directly in your code.
 */
void gfx_mono_ssd1306_put_framebuffer(void)
{
	uint8_t page;

	for (page = 0; page < GFX_MONO_LCD_PAGES; page++) {
		ssd1306_set_page_address(page);
		ssd1306_set_column_address(0);
		gfx_mono_ssd1306_put_page(framebuffer
				+ (page * GFX_MONO_LCD_WIDTH), page, 0,
				GFX_MONO_LCD_WIDTH);
	}
}

/**
 * \brief Draw pixel to screen
 *
 * \param x         X coordinate of the pixel
 * \param y         Y coordinate of the pixel
 * \param color     Pixel operation
 *
 * The following will set the pixel at x=10,y=10:
 * \code
	gfx_mono_ssd1306_draw_pixel(10, 10, GFX_PIXEL_SET);
\endcode
 * The following example will clear the pixel at x=10,y=10:
 * \code
	gfx_mono_ssd1306_draw_pixel(10, 10, GFX_PIXEL_CLR);
\endcode
 * And the following will toggle the pixel at x=10,y=10:
 * \code
	gfx_mono_ssd1306_draw_pixel(10, 10, GFX_PIXEL_XOR);
\endcode
 */
void gfx_mono_ssd1306_draw_pixel(uint8_t x, uint8_t y,uint8_t color)
{
	uint8_t page;
	uint8_t pixel_mask;
	uint8_t pixel_value;

	/* Discard pixels drawn outside the screen */
	if ((x > GFX_MONO_LCD_WIDTH - 1) || (y > GFX_MONO_LCD_HEIGHT - 1)) {
		return;
	}

	page = y / GFX_MONO_LCD_PIXELS_PER_BYTE;
	pixel_mask = (1 << (y - (page * 8)));

	/*
	 * Read the page containing the pixel in interest, then perform the
	 * requested action on this pixel before writing the page back to the
	 * display.
	 */
	pixel_value = gfx_mono_get_byte(page, x);

	switch (color) {
	case GFX_PIXEL_SET:
		pixel_value |= pixel_mask;
		break;

	case GFX_PIXEL_CLR:
		pixel_value &= ~pixel_mask;
		break;

	case GFX_PIXEL_XOR:
		pixel_value ^= pixel_mask;
		break;

	default:
		break;
	}

	gfx_mono_put_byte(page, x, pixel_value);
}

/**
 * \brief Get the pixel value at x,y
 *
 * \param x      X coordinate of pixel
 * \param y      Y coordinate of pixel
 * \return Non zero value if pixel is set.
 *
 * The following example will read the pixel value from x=10,y=10:
 * \code
	pixelval = gfx_mono_ssd1306_get_pixel(10,10);
\endcode
 */
uint8_t gfx_mono_ssd1306_get_pixel(uint8_t x, uint8_t y)
{
	uint8_t page;
	uint8_t pixel_mask;

	if ((x > GFX_MONO_LCD_WIDTH - 1) || (y > GFX_MONO_LCD_HEIGHT - 1)) {
		return 0;
	}

	page = y / GFX_MONO_LCD_PIXELS_PER_BYTE;
	pixel_mask = (1 << (y - (page * 8)));

	return gfx_mono_get_byte(page, x) & pixel_mask;
}

/**
 * \brief Put a page from RAM to display controller.
 *
 * If the controller is accessed by the SPI interface, we can not read
 * back data from the LCD controller RAM. Because of this all data that is
 * written to the LCD controller in this mode is also written to a framebuffer 
 * in MCU RAM.
 *
 * \param data Pointer to data to be written
 * \param page Page address
 * \param column Offset into page (x coordinate)
 * \param width Number of bytes to be written.
 *
 * The following example will write 32 bytes from data_buf to the page 0,
 * column 10. This will place data_buf in the rectangle x1=10,y1=0,x2=42,y2=8
 * (10 pixels from the upper left corner of the screen):
 * \code
	gfx_mono_ssd1306_put_page(data_buf, 0, 10, 32);
\endcode
 */
void gfx_mono_ssd1306_put_page(uint8_t *data, uint8_t page,uint8_t column, uint8_t width)
{
#ifdef CONFIG_SSD1306_FRAMEBUFFER
	gfx_mono_framebuffer_put_page(data, page, column, width);
#endif
	ssd1306_set_page_address(page);
	ssd1306_set_column_address(column);

	do {
		ssd1306_write_data(*data++);
	} while (--width);
}

/**
 * \brief Read a page from the LCD controller
 *
 * If the LCD controller is accessed by the SPI interface we cannot read
 * data directly from the controller. In that case we will read the data from
 * the local framebuffer instead.
 *
 * \param data   Pointer where to store the read data
 * \param page   Page address
 * \param column Offset into page (x coordinate)
 * \param width  Number of bytes to be read
 *
 * The following example will read back the first 128 bytes (first page) from
 * the display memory:
 * \code
	gfx_mono_ssd1306_get_page(read_buffer, 0, 0, 128);
\endcode
 */
void gfx_mono_ssd1306_get_page(uint8_t *data, uint8_t page,
		uint8_t column, uint8_t width)
{
#ifdef CONFIG_SSD1306_FRAMEBUFFER
	gfx_mono_framebuffer_get_page(data, page, column, width);
#else
	ssd1306_set_page_address(page);
	ssd1306_set_column_address(column);

	do {
		*data++ = ssd1306_read_data();
	} while (--width);
#endif
}

/**
 * \brief Put a byte to the display controller RAM
 *
 * If the LCD controller is accessed by the SPI interface we will also put the
 * data to the local framebuffer.
 *
 * \param page Page address
 * \param column Page offset (x coordinate)
 * \param data Data to be written
 *
 * This example will put the value 0xFF to the first byte in the display memory
 * setting a 8 pixel high column of pixels in the upper left corner of the
 * display.
 * \code
	gfx_mono_ssd1306_put_byte(0, 0, 0xFF, false);
\endcode
 */
void gfx_mono_ssd1306_put_byte(uint8_t page, uint8_t column,
		uint8_t data, uint8_t force)
{
//#ifdef CONFIG_SSD1306_FRAMEBUFFER
	if (!force && data == gfx_mono_framebuffer_get_byte(page, column)) {
		return;
	}
	gfx_mono_framebuffer_put_byte(page, column, data);
//#endif

	ssd1306_set_page_address(page);
	ssd1306_set_column_address(column);

	ssd1306_write_data(data);
}

/**
 * \brief Get a byte from the display controller RAM
 *
 * If the LCD controller is accessed by the SPI interface we cannot read the
 * data. In this case return the data from the local framebuffer instead.
 *
 * \param page Page address
 * \param column Page offset (x coordinate)
 * \return data from LCD controller or framebuffer.
 *
 * The following code will read the first byte from the display memory or the
 * local framebuffer if direct read is not possible. The data represents the
 * pixels from x = 0 and y = 0 to y = 7.
 * \code
	data = gfx_mono_ssd1306_get_byte(0, 0);
\endcode
 */
uint8_t gfx_mono_ssd1306_get_byte(uint8_t page, uint8_t column)
{
#ifdef CONFIG_SSD1306_FRAMEBUFFER
	return gfx_mono_framebuffer_get_byte(page, column);

#else
	ssd1306_set_page_address(page);
	ssd1306_set_column_address(column);

	return ssd1306_read_data();

#endif
}

/**
 * \brief Read/Modify/Write a byte on the display controller
 *
 * This function will read the byte from the display controller (or the
 * framebuffer if we cannot read directly from the controller) and
 * do a mask operation on the byte according to the pixel operation selected
 * by the color argument and the pixel mask provided.
 *
 * \param page Page address
 * \param column Page offset (x coordinate)
 * \param pixel_mask Mask for pixel operation
 * \param color Pixel operation
 *
 * A small example that will XOR the first byte of display memory with 0xAA
 * \code
	gfx_mono_ssd1306_mask_byte(0,0,0xAA,GFX_PIXEL_XOR);
\endcode
 */
void gfx_mono_ssd1306_mask_byte(uint8_t page, uint8_t column,
		uint8_t pixel_mask, uint8_t color)
{
	uint8_t temp = gfx_mono_get_byte(page, column);

	switch (color) {
	case GFX_PIXEL_SET:
		temp |= pixel_mask;
		break;

	case GFX_PIXEL_CLR:
		temp &= ~pixel_mask;
		break;

	case GFX_PIXEL_XOR:
		temp ^= pixel_mask;
		break;

	default:
		break;
	}

	gfx_mono_put_byte(page, column, temp);
}




/* Pointer to the framebuffer; updated by the gfx_mono_set_framebuffer function */
static uint8_t *fbpointer;

/* \brief Set the LCD framebuffer.
 *
 * \param framebuffer A pointer to an allocated area of RAM that can hold the
 * framebuffer.
 *
 * A small example:
 * \code
	uint8_t framebuffer[FRAMEBUFFER_SIZE];
	gfx_mono_set_framebuffer(framebuffer);
\endcode
 */
void gfx_mono_set_framebuffer(uint8_t *framebuffer)
{
	fbpointer = framebuffer;
}

/**
 * \brief Put a page from RAM to the framebuffer
 *
 * \param data Pointer to data to be written
 * \param page Page address
 * \param column Offset into page (x coordinate)
 * \param width Number of bytes to be written.
 *
 * The following example will write 32 bytes from data_buf to the page 0,
 * column 10 (byte 10 to 42 in the framebuffer).
 * \code
	gfx_mono_framebuffer_put_page(data_buf, 0, 10, 32);
\endcode
 */
void gfx_mono_framebuffer_put_page(uint8_t *data, uint8_t page,
		uint8_t column, uint8_t width)
{
	uint8_t *data_pt = data;
	uint8_t *framebuffer_pt = fbpointer +
			((page * GFX_MONO_LCD_WIDTH) + column);

	do {
		*framebuffer_pt++ = *data_pt++;
	} while (--width > 0);
}

/**
 * \brief Read a page from the framebuffer
 *
 * \param data   Pointer where to store the read data
 * \param page   Page address
 * \param column Offset into page (x coordinate)
 * \param width  Number of bytes to be read
 *
 * The following example will read back the first 128 bytes (first page) from
 * the framebuffer:
 * \code
	gfx_mono_framebuffer_get_page(read_buffer, 0, 0, 128);
\endcode
 */
void gfx_mono_framebuffer_get_page(uint8_t *data, uint8_t page, \
		uint8_t column, uint8_t width)
{
	uint8_t *framebuffer_pt = fbpointer +
			((page * GFX_MONO_LCD_WIDTH) + column);
	do {
		*data++ = *framebuffer_pt++;
	} while (--width > 0);
}

/**
 * \brief Draw pixel to framebuffer
 *
 * \param x         X coordinate of the pixel
 * \param y         Y coordinate of the pixel
 * \param color     Pixel operation
 *
 */
void gfx_mono_framebuffer_draw_pixel(uint8_t x, uint8_t y,
		uint8_t color)
{
	uint8_t page;
	uint8_t pixel_mask;
	uint8_t pixel_value;

	/* Discard pixels drawn outside the screen */
	if ((x > GFX_MONO_LCD_WIDTH - 1) || (y > GFX_MONO_LCD_HEIGHT - 1)) {
		return;
	}

	page = y / GFX_MONO_LCD_PIXELS_PER_BYTE;
	pixel_mask = (1 << (y - (page * 8)));

	/*
	 * Read the page containing the pixel in interest, then perform the
	 * requested action on this pixel before writing the page back to the
	 * display.
	 */
	pixel_value = gfx_mono_framebuffer_get_byte(page, x);

	switch (color) {
	case GFX_PIXEL_SET:
		pixel_value |= pixel_mask;
		break;

	case GFX_PIXEL_CLR:
		pixel_value &= ~pixel_mask;
		break;

	case GFX_PIXEL_XOR:
		pixel_value ^= pixel_mask;
		break;

	default:
		break;
	}

	gfx_mono_framebuffer_put_byte(page, x, pixel_value);
}

/**
 * \brief Get the pixel value at x,y in framebuffer
 *
 * \param x      X coordinate of pixel
 * \param y      Y coordinate of pixel
 * \return Non zero value if pixel is set.
 *
 */
uint8_t gfx_mono_framebuffer_get_pixel(uint8_t x, uint8_t y)
{
	uint8_t page;
	uint8_t pixel_mask;

	if ((x > GFX_MONO_LCD_WIDTH - 1) || (y > GFX_MONO_LCD_HEIGHT - 1)) {
		return 0;
	}

	page = y / GFX_MONO_LCD_PIXELS_PER_BYTE;
	pixel_mask = (1 << (y - (page * 8)));

	return gfx_mono_framebuffer_get_byte(page, x) & pixel_mask;
}

/**
 * \brief Put a byte to the framebuffer
 *
 * \param page   Page address
 * \param column Page offset (x coordinate)
 * \param data   Data to be written
 *
 * This example will put the value 0xFF to the first byte in the framebuffer
 * \code
	gfx_mono_framebuffer_put_byte(0, 0, 0xFF);
\endcode
 */
void gfx_mono_framebuffer_put_byte(uint8_t page, uint8_t column,
		uint8_t data)
{
	*(fbpointer + (page * GFX_MONO_LCD_WIDTH) + column) = data;
}

/**
 * \brief Get a byte from the framebuffer
 *
 * \param page   Page address
 * \param column Page offset (x coordinate)
 * \return       data from LCD controller or framebuffer.
 *
 * The following code will read the first byte of the framebuffer
 * \code
	data = gfx_mono_framebuffer_get_byte(0, 0);
\endcode
 */
uint8_t gfx_mono_framebuffer_get_byte(uint8_t page, uint8_t column)
{
	return *(fbpointer + (page * GFX_MONO_LCD_WIDTH) + column);
}

/**
 * \brief Read/Modify/Write a byte in the framebuffer
 *
 * This function will read the byte from the framebuffer and
 * do a mask operation on the byte according to the pixel operation selected
 * by the color argument and the pixel mask provided.
 *
 * \param page       Page address
 * \param column     Page offset (x coordinate)
 * \param pixel_mask Mask for pixel operation
 * \param color      Pixel operation
 *
 * A small example that will XOR the first byte of the framebuffer with 0xAA
 * \code
	gfx_mono_framebuffer_mask_byte(0,0,0xAA,GFX_PIXEL_XOR);
\endcode
 */
void gfx_mono_framebuffer_mask_byte(uint8_t page, uint8_t column,
		uint8_t pixel_mask, uint8_t color)
{
	uint8_t temp;

	temp = gfx_mono_get_byte(page, column);

	switch (color) {
	case GFX_PIXEL_SET:
		temp |= pixel_mask;
		break;

	case GFX_PIXEL_CLR:
		temp &= ~pixel_mask;
		break;

	case GFX_PIXEL_XOR:
		temp ^= pixel_mask;
		break;
	}

	gfx_mono_put_byte(page, column, temp);
}























// 
// 
// 
// 
// 
// uint8_t gfx_mono_framebuffer_get_byte(uint8_t page, uint8_t column)
// {
// 	return *(fbpointer + (page * GFX_MONO_LCD_WIDTH) + column);
// }
// 
// void gfx_mono_framebuffer_put_byte(uint8_t page, uint8_t column,uint8_t data)
// {
// 	*(fbpointer + (page * GFX_MONO_LCD_WIDTH) + column) = data;
// }
// 
// void gfx_mono_ssd1306_put_byte(uint8_t page, uint8_t column,uint8_t data, uint8_t force)
// {
// 
// 	// 	#ifdef CONFIG_SSD1306_FRAMEBUFFER
// 	if (!force && data == gfx_mono_framebuffer_get_byte(page, column)) {
// 		return;
// 	}
// 	gfx_mono_framebuffer_put_byte(page, column, data);
// 	// 	#endif
// 
// 	ssd1306_set_page_address(page);
// 	ssd1306_set_column_address(column);
// 
// 	ssd1306_write_data(data);
// }
// 
// 
// void gfx_mono_set_framebuffer(uint8_t *framebuffer)
// {
// 	fbpointer = framebuffer;
// }
// 


#endif /* GFX_MONO_H_ */