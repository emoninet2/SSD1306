/*
 * ssd1306.h
 *
 * Created: 27-Dec-15 2:08:21 AM
 *  Author: emon1
 */ 













#ifndef SSD1306_H_
#define SSD1306_H_


#include "ssd130g_arch_driver.h"
#include "font.h"


#define SSD1306_CMD_SET_LOW_COL(column)             (0x00 | (column))
#define SSD1306_CMD_SET_HIGH_COL(column)            (0x10 | (column))
#define SSD1306_CMD_SET_MEMORY_ADDRESSING_MODE      0x20
#define SSD1306_CMD_SET_COLUMN_ADDRESS              0x21
#define SSD1306_CMD_SET_PAGE_ADDRESS                0x22
#define SSD1306_CMD_SET_START_LINE(line)            (0x40 | (line))
#define SSD1306_CMD_SET_CONTRAST_CONTROL_FOR_BANK0  0x81
#define SSD1306_CMD_SET_CHARGE_PUMP_SETTING         0x8D
#define SSD1306_CMD_SET_SEGMENT_RE_MAP_COL0_SEG0    0xA0
#define SSD1306_CMD_SET_SEGMENT_RE_MAP_COL127_SEG0  0xA1
#define SSD1306_CMD_ENTIRE_DISPLAY_AND_GDDRAM_ON    0xA4
#define SSD1306_CMD_ENTIRE_DISPLAY_ON               0xA5
#define SSD1306_CMD_SET_NORMAL_DISPLAY              0xA6
#define SSD1306_CMD_SET_INVERSE_DISPLAY             0xA7
#define SSD1306_CMD_SET_MULTIPLEX_RATIO             0xA8
#define SSD1306_CMD_SET_DISPLAY_ON                  0xAF
#define SSD1306_CMD_SET_DISPLAY_OFF                 0xAE
#define SSD1306_CMD_SET_PAGE_START_ADDRESS(page)    (0xB0 | (page & 0x07))
#define SSD1306_CMD_SET_COM_OUTPUT_SCAN_UP          0xC0
#define SSD1306_CMD_SET_COM_OUTPUT_SCAN_DOWN        0xC8
#define SSD1306_CMD_SET_DISPLAY_OFFSET              0xD3
#define SSD1306_CMD_SET_DISPLAY_CLOCK_DIVIDE_RATIO  0xD5
#define SSD1306_CMD_SET_PRE_CHARGE_PERIOD           0xD9
#define SSD1306_CMD_SET_COM_PINS                    0xDA
#define SSD1306_CMD_SET_VCOMH_DESELECT_LEVEL        0xDB
#define SSD1306_CMD_NOP                             0xE3
//@}
//! \name Graphic Acceleration Command defines
//@{
#define SSD1306_CMD_SCROLL_H_RIGHT                  0x26
#define SSD1306_CMD_SCROLL_H_LEFT                   0x27
#define SSD1306_CMD_CONTINUOUS_SCROLL_V_AND_H_RIGHT 0x29
#define SSD1306_CMD_CONTINUOUS_SCROLL_V_AND_H_LEFT  0x2A
#define SSD1306_CMD_DEACTIVATE_SCROLL               0x2E
#define SSD1306_CMD_ACTIVATE_SCROLL                 0x2F
#define SSD1306_CMD_SET_VERTICAL_SCROLL_AREA        0xA3



#define ssd1306_reset_clear()    port_out_value_clear_mask(&PORTA,(1<<3));//select command
#define ssd1306_reset_set()      port_out_value_set_mask(&PORTA,(1<<3));//select command

// Data/CMD select, PC21Could not add reference to assembly IronPython.wpf
#define ssd1306_sel_data()       port_out_value_set_mask(&PORTD,(1<<0));//select command
#define ssd1306_sel_cmd()        port_out_value_clear_mask(&PORTD,(1<<0));//select command








typedef struct usart_spi_options_emon {
	//! Set baud rate of the USART in SPI mode.
	uint32_t baudrate;
	//! SPI transmission mode.
	uint8_t spimode;
	uint8_t data_order;
} usart_spi_options_emon_t;


typedef uint8_t spi_flags_t;
typedef uint32_t board_spi_select_id_t;

#ifndef USART_UCPHA_bm
#  define USART_UCPHA_bm 0x02
#endif
#ifndef USART_DORD_bm
#  define USART_DORD_bm 0x04
#endif


//! \name OLED controller write and read functions
//@{
/**
 * \brief Writes a command to the display controller
 *
 * This functions pull pin D/C# low before writing to the controller. Different
 * data write function is called based on the selected interface.
 *
 * \param command the command to write
 */
static void ssd1306_write_command(uint8_t command)
{
	port_out_value_clear_mask(&PORTF,(1<<3));//select device
	port_out_value_clear_mask(&PORTD,(1<<0));//select command
	ssd1306_sel_cmd() ;
	usart_spi_transmit_receive(&USARTD0, command);//transmit
	port_out_value_set_mask(&PORTF,(1<<3));//deselect device
	
}

/**
 * \brief Write data to the display controller
 *
 * This functions sets the pin D/C# before writing to the controller. Different
 * data write function is called based on the selected interface.
 *
 * \param data the data to write
 */
void ssd1306_write_data(uint8_t data)
{
	
	port_out_value_clear_mask(&PORTF,(1<<3));//select device
	ssd1306_sel_data() ;
	usart_spi_transmit_receive(&USARTD0, data);//transmit
	ssd1306_sel_cmd();
	port_out_value_set_mask(&PORTF,(1<<3));//deselect device

}

/**
 * \brief Read data from the controller
 *
 * \note The controller does not support read in serial mode.
 *
 * \retval 8 bit data read from the controller
 */
uint8_t ssd1306_read_data(void)
{
	return 0;
}

/**
 * \brief Read status from the controller
 *
 * \note The controller does not support read in serial mode.
 *
 * \retval 8 bit status read from the controller
 */
static inline uint8_t ssd1306_get_status(void)
{
	return 0;
}




/**
 * \brief Perform a hard reset of the OLED controller
 *
 * This functions will reset the OLED controller by setting the reset pin low.
 * \note this functions should not be confused with the \ref ssd1306_soft_reset()
 * function, this command will control the RST pin.
 */
static inline void ssd1306_hard_reset(void)
{
	PORTA.OUTCLR = (1<<3);
	delay_us(10); // At least 3us
	PORTA.OUTSET = (1<<3);
	delay_us(10); // At least 3us
}

/**
 * \brief Enable the OLED sleep mode
 */
static inline void ssd1306_sleep_enable(void)
{
	ssd1306_write_command(SSD1306_CMD_SET_DISPLAY_OFF);
}

/**
 * \brief Disable the OLED sleep mode
 */
static inline void ssd1306_sleep_disable(void)
{
	ssd1306_write_command(SSD1306_CMD_SET_DISPLAY_ON);
}

/**
 * \brief Set current page in display RAM
 *
 * This command is usually followed by the configuration of the column address
 * because this scheme will provide access to all locations in the display
 * RAM.
 *
 * \param address the page address
 */
void ssd1306_set_page_address(uint8_t address)
{
	// Make sure that the address is 4 bits (only 8 pages)
	address &= 0x0F;
	ssd1306_write_command(SSD1306_CMD_SET_PAGE_START_ADDRESS(address));
}

/**
 * \brief Set current column in display RAM
 *
 * \param address the column address
 */
void ssd1306_set_column_address(uint8_t address)
{
	// Make sure the address is 7 bits
	address &= 0x7F;
	ssd1306_write_command(SSD1306_CMD_SET_HIGH_COL(address >> 4));
	ssd1306_write_command(SSD1306_CMD_SET_LOW_COL(address & 0x0F));
}

/**
 * \brief Set the display start draw line address
 *
 * This function will set which line should be the start draw line for the OLED.
 */
static inline void ssd1306_set_display_start_line_address(uint8_t address)
{
	// Make sure address is 6 bits
	address &= 0x3F;
	ssd1306_write_command(SSD1306_CMD_SET_START_LINE(address));
}

/**
 * \brief Turn the OLED display on
 *
 * This function will turn on the OLED.
 */
static inline void ssd1306_display_on(void)
{
	ssd1306_write_command(SSD1306_CMD_SET_DISPLAY_ON);
}



/**
 * \brief Turn the OLED display off
 *
 * This function will turn off the OLED.
 */
static inline void ssd1306_display_off(void)
{
	ssd1306_write_command(SSD1306_CMD_SET_DISPLAY_OFF);
}



/**
 * \brief Set the OLED contrast level
 *
 * \param contrast a number between 0 and 0xFF
 *
 * \retval contrast the contrast value written to the OLED controller
 */
static inline uint8_t ssd1306_set_contrast(uint8_t contrast)
{
	ssd1306_write_command(SSD1306_CMD_SET_CONTRAST_CONTROL_FOR_BANK0);
	ssd1306_write_command(contrast);
	return contrast;
}



/**
 * \brief Invert all pixels on the device
 *
 * This function will invert all pixels on the OLED
 *
 */
static inline void ssd1306_display_invert_enable(void)
{
	ssd1306_write_command(SSD1306_CMD_SET_INVERSE_DISPLAY);
}

/**
 * \brief Disable invert of all pixels on the device
 *
 * This function will disable invert on all pixels on the OLED
 *
 */
static inline void ssd1306_display_invert_disable(void)
{
	ssd1306_write_command(SSD1306_CMD_SET_NORMAL_DISPLAY);
}




static inline void ssd1306_clear(void)
{
	uint8_t page = 0;
	uint8_t col = 0;

	for (page = 0; page < 4; ++page)
	{
		for (col = 0; col < 128; ++col)
		{
			ssd1306_set_page_address(page);
			ssd1306_set_column_address(col);
			ssd1306_write_data(0x00);
		}
	}
}


void usart_init_spi(USART_t *usart, const usart_spi_options_emon_t *opt)
{
	//ioport_pin_t sck_pin;
	//uint8_t invert_sck;

	(usart)->CTRLB &= ~USART_RXEN_bm;

	//invert_sck = (opt->spimode == 2) || (opt->spimode == 3);

	PORTD.DIRSET = (1<<1);//sck pin output
	PORTD.OUTSET = (1<<1);//sck pin output level high

	(usart)->CTRLC = ((usart)->CTRLC & (~USART_CMODE_gm)) | USART_CMODE_MSPI_gc;

	if (opt->spimode == 1 || opt->spimode == 3) {
		usart->CTRLC |= USART_UCPHA_bm;
	}
	else {
		usart->CTRLC &= ~USART_UCPHA_bm;
	}
	
	if (opt->data_order) {
		(usart)->CTRLC |= USART_DORD_bm;
	}
	else {
		(usart)->CTRLC &= ~USART_DORD_bm;
	}


	uint16_t bsel_value = (32000000UL/ (opt->baudrate*2)) -1 ;
	(usart)->BAUDCTRLB = (uint8_t)((~USART_BSCALE_gm) & (bsel_value >> 8));
	(usart)->BAUDCTRLA = (uint8_t)(bsel_value);
	
	(usart)->CTRLB |= USART_TXEN_bm;

}







void ssd1306_init(void)
{

	port_direction_set_mask(&PORTF,(1<<3));//ss
	port_direction_set_mask(&PORTD,(1<<0));//data_cmd
	port_direction_set_mask(&PORTA,(1<<3));//reset
	port_direction_set_mask(&PORTD,(1<<1));//sck
	port_direction_set_mask(&PORTD,(1<<3));//mosi
// 	
	port_out_value_set_mask(&PORTF,(1<<3));//ss
//	port_out_value_set_mask(&PORTD,(1<<0));//data_cmd
/*	port_out_value_set_mask(&PORTA,(1<<3));//reset*/
// 	port_out_value_set_mask(&PORTD,(1<<1));//sck
// 	port_out_value_set_mask(&PORTD,(1<<3));//mosi
// 		
	
	
	
	

	// Do a hard reset of the OLED display controller
	ssd1306_hard_reset();

	spi_flags_t spi_flags = 0;
	//board_spi_select_id_t spi_select_id = 0;
	
	//irqflags_t flags = cpu_irq_save();
	//*((uint8_t *)&PR.PRGEN + SYSCLK_PORT_D) &= ~PR_USART0_bm;
	PR.PRPD &= ~PR_USART0_bm;
	
	//cpu_irq_restore(flags);
	
	usart_spi_options_emon_t opt;
	opt.baudrate=12000000;
	opt.spimode=spi_flags;
	opt.data_order=0;
	usart_init_spi(&USARTD0, &opt);
	

	// 1/32 Duty (0x0F~0x3F)
	ssd1306_write_command(SSD1306_CMD_SET_MULTIPLEX_RATIO);
	ssd1306_write_command(0x1F);

	// Shift Mapping RAM Counter (0x00~0x3F)
	ssd1306_write_command(SSD1306_CMD_SET_DISPLAY_OFFSET);
	ssd1306_write_command(0x00);

	// Set Mapping RAM Display Start Line (0x00~0x3F)
	ssd1306_write_command(SSD1306_CMD_SET_START_LINE(0x00));

	// Set Column Address 0 Mapped to SEG0
	ssd1306_write_command(SSD1306_CMD_SET_SEGMENT_RE_MAP_COL127_SEG0);

	// Set COM/Row Scan Scan from COM63 to 0
	ssd1306_write_command(SSD1306_CMD_SET_COM_OUTPUT_SCAN_DOWN);

	// Set COM Pins hardware configuration
	ssd1306_write_command(SSD1306_CMD_SET_COM_PINS);
	ssd1306_write_command(0x02);

	//ssd1306_set_contrast(0x8F);
	ssd1306_write_command(SSD1306_CMD_SET_CONTRAST_CONTROL_FOR_BANK0);
	ssd1306_write_command(0x8F);

		
	// Disable Entire display On
	ssd1306_write_command(SSD1306_CMD_ENTIRE_DISPLAY_AND_GDDRAM_ON);

	//ssd1306_display_invert_disable();
	ssd1306_write_command(SSD1306_CMD_SET_NORMAL_DISPLAY);

	// Set Display Clock Divide Ratio / Oscillator Frequency (Default => 0x80)
	ssd1306_write_command(SSD1306_CMD_SET_DISPLAY_CLOCK_DIVIDE_RATIO);
	ssd1306_write_command(0x80);

	// Enable charge pump regulator
	ssd1306_write_command(SSD1306_CMD_SET_CHARGE_PUMP_SETTING);
	ssd1306_write_command(0x14);

	// Set VCOMH Deselect Level
	ssd1306_write_command(SSD1306_CMD_SET_VCOMH_DESELECT_LEVEL);
	ssd1306_write_command(0x40); // Default => 0x20 (0.77*VCC)

	// Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	ssd1306_write_command(SSD1306_CMD_SET_PRE_CHARGE_PERIOD);
	ssd1306_write_command(0xF1);

	//ssd1306_display_on();
	ssd1306_write_command(SSD1306_CMD_SET_DISPLAY_ON);
}



/**
 * \brief Display text on OLED screen.
 * \param string String to display.
 */
void ssd1306_write_text(char *string)
{
	uint8_t *char_ptr;
	uint8_t i;

	while (*string != '\0') {
		if (*string < 0x7F) {
			char_ptr = font_table[*string - 32];
			for (i = 1; i <= char_ptr[0]; i++) {
				ssd1306_write_data(char_ptr[i]);
			}
			ssd1306_write_data(0x00);
		}
			string++;
	}
}



#endif /* SSD1306_H_ */