#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>


#include "lcd/lcd44780.h"
#include "uart/uart.h"

#define UART_BAUD_RATE	38400

void get_uart_message();
void print_message(char *str);
void mystrcpy(char * src,char * dst,uint8_t from,uint8_t to);

int main(void)
{
	lcd_init();
	lcd_str("Uruchamiam");
	uart_init(UART_BAUD_SELECT(UART_BAUD_RATE,F_CPU));
	sei();
	lcd_cls();
	lcd_str("Polacz sie");
	while(1){
		get_uart_message();
	}
}

void mystrcpy(char * src,char * dst,uint8_t from,uint8_t to){
	for(uint8_t i=0,j=0 ; to>=i ;i++ ){
		if(src[i] == '\r'){
			return;
		}
		if(from<=i){
			dst[j]=src[i];
			j++;
		}
	}
}

void print_message(char *str){
	static char lcdbuffor[36];
	if(strlen(str)-2>16){
		strncpy(lcdbuffor,str,16);
		lcd_cls();
		lcd_str(lcdbuffor);
		memset(lcdbuffor,0,36);
		mystrcpy(str,lcdbuffor,16,32);
		lcd_locate(1,0);
		lcd_str(lcdbuffor);
	}
	else{
		strncpy(lcdbuffor,str,strlen(str)-2);
		lcd_cls();
		lcd_str(lcdbuffor);
	}
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
