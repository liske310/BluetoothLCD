#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>


//TODO: change library to Peter Fluery library
//#include "lcd/lcd44780.h"
#include "lcd/lcd44780.h"
#include "uart/uart.h"

#define UART_BAUD_RATE	38400

void get_uart_message();
void print_message(char *str);

int main(void)
{
	lcd_init();
	lcd_str("Uruchamiam");
	uart_init(UART_BAUD_SELECT(UART_BAUD_RATE,F_CPU));
	sei();
	lcd_cls();
	lcd_locate(0,0);
	lcd_str("Polacz sie");
	while(1){
		get_uart_message();
	}
}

void print_message(char *str){
	/*
	 * TODO:
	 * add 2 lines
	 * if len > 16 but 1:line12:line2 then nothing :)
	 * if len > 16 16 chars in 1 line other 16 in line 2
	 */
	static char lcdbuffor[36];
	strncpy(lcdbuffor,str,strlen(str)-2);
	lcd_cls();
	lcd_str(lcdbuffor);
	memset(lcdbuffor,0,36);
}

void get_uart_message(){
	static unsigned int get_data;
	static uint16_t counter = 0;
	static char uartbuffor[36];
	do{
			get_data = uart_getc();
			if ( get_data & UART_NO_DATA )  return;
			else
			{
					if ( get_data & UART_FRAME_ERROR )      return;
					if ( get_data & UART_OVERRUN_ERROR )    return;
					if ( get_data & UART_BUFFER_OVERFLOW )  return;
			}
			uartbuffor[counter] = get_data;
			counter++;
	}while(get_data != '\n');
	counter = 0;
	print_message(uartbuffor);
	memset(uartbuffor,0,36);
}
