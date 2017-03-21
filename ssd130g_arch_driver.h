/*
 * ssd130g_arch_driver.h
 *
 * Created: 08-May-16 3:17:30 PM
 *  Author: emon1
 */ 


#ifndef SSD130G_ARCH_DRIVER_H_
#define SSD130G_ARCH_DRIVER_H_



#include <avr/io.h>
#include "../XMEGA_API/xmega_api.h"
//#define delay_us(us) _delay_us(us)


#define port_direction(port,val) (port)->DIR = val
#define port_direction_set_mask(port,mask) (port)->DIRSET = mask
#define port_direction_clear_mask(port,mask) (port)->DIRCLR = mask
#define port_direction_toggle_mask(port,mask) (port)->DIRTGL = mask

#define port_out_value(port,val) (port)->OUT = val
#define port_out_value_set_mask(port,mask) (port)->OUTSET = mask
#define port_out_value_clear_mask(port,mask) (port)->OUTCLR = mask
#define port_out_value_toggle_mask(port,mask) (port)->OUTTGL = mask

#define port_in_value (port) (port)->IN



//////////////////////////////////////////////////////////////////////////




/*
 *	usart setup.
 * setup for the usart
 * @param usart usart unit for which the setting is done
 * @param mode
 * @param pmode parity mode
 * @param chsize character size
 * @param clk2en enable double speed
 * @param mpcen enable multiprocessor communication
 */
void usart_config(USART_t *usart_unit,PORT_t *port_unit, USART_CMODE_t _nrf24l01p_RxTxMode, USART_PMODE_t pmode, USART_CHSIZE_t chsize, uint8_t clk2xen, uint8_t mpcmen){
	usart_unit->CTRLB = USART_RXEN_bm | USART_TXEN_bm;
	usart_unit->CTRLB |= (clk2xen<<USART_CLK2X_bp) | (mpcmen<<USART_MPCM_bp);
	usart_unit->CTRLC = _nrf24l01p_RxTxMode |pmode | chsize;

	
	if( (usart_unit == &USARTC0) || (usart_unit == &USARTD0) || (usart_unit == &USARTE0) || (usart_unit == &USARTF0) ){
		port_unit->DIRSET = (1<<3);//USARTx0 TXD pin output
		port_unit->DIRCLR = (1<<2);//USARTx0 RXD pin input
	}
	else if( (usart_unit == &USARTC1) /*|| (usart_unit == &USARTD1) || (usart_unit == &USARTE1) || (usart_unit == &USARTF1)*/ ) {
		port_unit->DIRSET = (1<<7);//USARTx1 TXD pin output
		port_unit->DIRCLR = (1<<6);//USARTx1 RXD pin input
	}
}

/*
 *	config baud rate
 * @param usart
 * @param baud_rate
 * @param bscale_val
 */
void usart_baud(USART_t *usart, unsigned long baud_rate, int bscale_val){
	uint16_t bsel_val = BAUD_VALUE(baud_rate,bscale_val);
	usart->BAUDCTRLA = bsel_val;
	usart->BAUDCTRLB = (bscale_val<<USART_BSCALE_gp) | (bsel_val>>8);
}



/*
 *	usart interrupt level.
 * @param usart
 * @param rxcintlvl
 * @param txcintlvl
 * @param dreintlvl
 */
void usart_set_interrupt_level(USART_t *usart, USART_RXCINTLVL_t rxcintlvl, USART_TXCINTLVL_t txcintlvl, USART_DREINTLVL_t dreintlvl){
	//usart->CTRLA = (rxcintlvl<<USART_RXCINTLVL_gp) | (txcintlvl<<USART_TXCINTLVL_gp) | (dreintlvl<<USART_DREINTLVL_gp);
	usart->CTRLA = (rxcintlvl) | (txcintlvl) | (dreintlvl);
}


/**
 * transmit data over usart by value
 @param data value of the data to be sent
 */
void usart_transmit_by_val(USART_t *usart_unit, uint8_t data){
	while(!(usart_unit->STATUS&USART_DREIF_bm));
	usart_unit->DATA = data;
	//USART_PutChar(usart_unit, data);
		
}

/**
 * transmit data address over usart by refereence
 @param data value of the data address to be sent
 */
void usart_transmit_by_ref(USART_t *usart_unit, uint8_t *data){
	while(!(usart_unit->STATUS&USART_DREIF_bm));
	usart_unit->DATA = *data;
	//USART_PutChar(usart_unit, *data);
}

/**
 * transmit address to receive datay into
 @param data address of the data to be received
 */
void usart_receive_by_ref(USART_t *usart_unit, uint8_t *data){
	while(!(usart_unit->STATUS&USART_RXCIF_bm));
	*data = usart_unit->DATA;
	//*data = USART_GetChar(usart_unit);
}

/**
 * receve data over usart by return value
 @return data value of the data received
 */
uint8_t usart_receive_by_val(USART_t *usart_unit){
	while(!(usart_unit->STATUS&USART_RXCIF_bm));
	return usart_unit->DATA;
	//return USART_GetChar(usart_unit);	
}

/*
 *	usart spi transmit recieve.
 *
 * @param usart
 * @param txdata
 * @return data received from slave
 */
uint8_t usart_spi_transmit_receive(USART_t * usart,uint8_t txdata){
	
	while ((usart->STATUS & USART_DREIF_bm) == 0);
	usart->DATA = txdata;
	while((usart->STATUS & USART_TXCIF_bm) == 0);
	(usart)->STATUS = USART_TXCIF_bm;
	return (usart)->DATA;
}







#endif /* SSD130G_ARCH_DRIVER_H_ */