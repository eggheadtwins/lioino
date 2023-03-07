#include <avr/io.h>
#include <avr/sfr_defs.h>
#include <util/delay.h>

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#define SERVO_DDRx DDRB

#ifdef SERVO_DDRx
#	define SERVO_PIN PINB1
#	define SERVO_PORT PORTB
#endif

#define PRESCALER (_BV(CS11));


void servo_init(void){
	// Set Servo pin as OUTPUT.
	SERVO_DDRx |= _BV(SERVO_PIN);
	
	// Timer 1 - PWM - Mode 14
	TCCR1A |= _BV(WGM11) | _BV(COM1A1);
	TCCR1B |= _BV(WGM13) | _BV(WGM12);
	TCCR1B |= PRESCALER;
}


void set_angle(){
	for (int i = 0; i <= 4999; i++)
	{
		ICR1 = i;
		_delay_ms(100);
	}
	
	
}






