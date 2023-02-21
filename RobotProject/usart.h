
#include <avr/io.h>

#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1


void usart_init(void){
	UBRR0H = (MYUBRR >> 8); //eight least significant bits of BAUD.
	UBRR0L = MYUBRR; // 4 significant bits of BAUD.
	UCSR0B = (1<<TXEN0) | (1<<RXEN0); //enable transmit and receive
	UCSR0C |= (1<<UCSZ01)|(1<<UCSZ00);
	UCSR0C &= ~(1<<UMSEL00) & ~(1<<UPM00) & ~(1<<USBS0);
}


void usart_send_char(uint8_t data){
	while (!(UCSR0A & (1<<UDRE0)));
	UDR0 = data;
}


void usart_send_chars(uint8_t * s){
	while (*s){
		usart_send_char(*s++);
	}

}


uint8_t usart_recieve(){
	while((UCSR0A & (1<<7)) != 0);
	return UDR0;
}