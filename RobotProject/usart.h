
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


void usart_send_8bit(uint8_t data){
	uint8_t string[4]; // Max 255, 4 Chars with '/0'.
	int i = 0;
	
	itoa(data, string, 10); // Convert Int to String.
	
	while (string[i] != '\0'){
		usart_send_char(string[i++]);
	}
	
	free(string);

}


void send(uint8_t * data){
	while (*data){
		usart_send_char(*data++);
	}
	
}

void usart_send_16bit(uint16_t data){
	uint8_t string[10]; // Max 255, 4 Chars with '/0'.
	int i = 0;
	
	itoa(data, string, 10); // Convert Int to String.
	
	while (string[i] != '\0'){
		usart_send_char(string[i++]);
	}
	
	free(string);	
	
	
}


uint8_t usart_recieve(){
	while(!(UCSR0A & _BV(RXC0)));
	return UDR0;
}
