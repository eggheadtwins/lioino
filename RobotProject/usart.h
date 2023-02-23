
#include <avr/io.h>
#include <stdlib.h>
#include <avr/sfr_defs.h>

#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1

 
void usart_init(void){
	UBRR0H = (MYUBRR >> 8); //eight least significant bits of BAUD.
	UBRR0L = MYUBRR; // 4 significant bits of BAUD.
	UCSR0B = _BV(TXEN0) | _BV(RXEN0); //enable transmit and receive
	UCSR0C |= _BV(UCSZ01) | _BV(UCSZ00);
	UCSR0C &= ~_BV(UMSEL00) & ~_BV(UPM00) & ~_BV(USBS0);
}


void usart_send_char(uint8_t data){
	while (!(UCSR0A & _BV(UDRE0)));
	UDR0 = data;
}


void usart_send_chars(uint8_t * s){
	while (*s){
		usart_send_char(*s++);
	}

}


uint8_t usart_recieve(){
	while((UCSR0A & _BV(7)) != 0);
	return UDR0;
}


void uart_puti16B (uint16_t value){
	char _buffer[17];
	itoa( value, _buffer, 2 );
	usart_send_char(_buffer);
	free(_buffer);
}