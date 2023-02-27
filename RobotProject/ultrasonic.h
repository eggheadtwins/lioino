#include <avr/io.h>
#include <avr/sfr_defs.h>
#include <util/delay.h>
#include <time.h>

#define TRIGGER_DDRx
#define ECHO_DDRx
#define TRIGGER_PIN
#define ECHO_PIN
#define TRIGGER_PORTx
#define ECHO_PORTx

#define HIGH 1
#define LOW 0



void ultrasonic_init(){
	TRIGGER_DDRx |= _BV(TRIGGER_PIN);
	ECHO_PIN &= ~_BV(ECHO_PIN);
	
}

uint16_t get_distance(){
	TRIGGER_PORTx &= ~_BV(TRIGGER_PIN);
	_delay_ms(2);
	TRIGGER_PORTx |= _BV(TRIGGER_PIN);
	_delay_ms(10);
	TRIGGER_PORTx &= ~_BV(TRIGGER_PIN);
	
	int duration = pulseIn(ECHO_PIN, HIGH);
	
	
}

void pulseIn(uint8_t echo_pin, uint8_t is_high){
	int duration;
	int msec;
	clock_t start = clock();
	
	while(!bit_is_low());
	
	duration = clock() - start;

	
	
}

void bit_is_low(){
	// 
	
}