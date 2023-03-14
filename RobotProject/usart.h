#include <avr/io.h>
#include <stdlib.h>
#include <avr/sfr_defs.h>
#include <avr/interrupt.h>

#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1

volatile uint8_t command;

void usart_init(void){
	UBRR0H = (MYUBRR >> 8); //eight least significant bits of BAUD.
	UBRR0L = MYUBRR; // 4 significant bits of BAUD.
	UCSR0B = _BV(TXEN0) | _BV(RXEN0) | _BV(RXCIE0); //enable transmit and receive, and On receive interrupt.
	UCSR0C |= _BV(UCSZ01) | _BV(UCSZ00);
	UCSR0C &= ~_BV(UMSEL00) & ~_BV(UPM00) & ~_BV(USBS0);
}


void usart_send_char(uint8_t data){
	while (!(UCSR0A & _BV(UDRE0)));
	UDR0 = data;
}


void usart_send_chars(uint8_t * data){
	while (*data){
		usart_send_char(*data++);
	}
	
}

void usart_send_8bit(uint8_t data){
	uint8_t string[4]; // Max 255, 4 Chars with '/0'.
	itoa(data, string, 10); // Convert Int to String.
	usart_send_chars(string);
	free(string);

}


void usart_send_16bit(uint16_t data){
	uint8_t string[10]; // Max 1024, 5 Chars with '/0'.
	itoa(data, string, 10); // Convert Int to String.
	
	usart_send_chars(string);

	free(string);
	
	
}

void usart_send_int(int data){
	uint8_t string[24];
	
	itoa(data, string, 10);
	
	usart_send_chars(string);
	free(string);
	
}


uint8_t usart_recieve(){
	while(!(UCSR0A & _BV(RXC0)));
	return UDR0;
}

ISR(USART_RX_vect){
	command = UDR0;
	
}
