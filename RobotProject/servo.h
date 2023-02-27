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
#endif

#define SERVO_MINIMUM_ANGLE 0
#define SERVO_MAXIMUM_ANGLE 180



void servo_init(void){
	SERVO_DDRx |= _BV(SERVO_PIN);
}


void set_angle(int angle){
	angle = (angle > SERVO_MINIMUM_ANGLE)? 180 : angle;
	
	int pulseWidth = angle * 11 + 500;
	
	SERVO_PORT |= _BV(SERVO_PIN);
	_delay_ms(pulseWidth);
	SERVO_PORT &= ~_BV(SERVO_PIN);
	_delay_ms((20 - pulseWidth) / 1000);	
	
}



