#include <avr/io.h>
#include <avr/sfr_defs.h>
#include <util/delay.h>

#ifndef F_CPU
#define F_CPU 16000000UL
#endif


#define SERVO_DDRx DDRB

#ifdef SERVO_DDRx
#define SERVO_PIN PINB2
#define SERVO_PORT PORTB
#define SERVO_OCnx OCR1B
#endif

#define MINIMUM_ANGLE 0
#define MAXIMUM_ANGLE 180



void servo_init(void){
	SERVO_DDRx |= _BV(SERVO_PIN);
	TCCR1A |= _BV(COM1B1) | _BV(WGM11); // PWM Phase Correct. 
	ICR1 = 4999; // Top value
}

void set(uint8_t angle){
	SERVO_OCnx = 
}


void set_angle(int angle){
	angle = (angle > MAXIMUM_ANGLE)? 180 : angle;
	
	for (uint8_t i = 0; i <= MAXIMUM_ANGLE; i++)
	{
		int pulseWidth = angle * 11 + 500;
			
		SERVO_PORT |= _BV(SERVO_PIN);
		_delay_ms(pulseWidth);
		SERVO_PORT &= ~_BV(SERVO_PIN);
		_delay_ms(20);
	}
	
	
}



