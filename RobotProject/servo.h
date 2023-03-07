#include <avr/io.h>
#include <avr/sfr_defs.h>
#include <util/delay.h>

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#define SERVO_DDRx DDRB

#ifdef SERVO_DDRx
#	define SERVO_PIN PINB2
#	define SERVO_PORT PORTB
#	define SERVO_OCnx OCR1B
#endif

#define TOP 19999
#define SERVO_MAX 40000
#define SERVO_MIN 8000


void servo_init(void){
	// Set Servo pin as OUTPUT.
	SERVO_DDRx |= _BV(SERVO_PIN);
	
	// Timer 1 - PWM - Mode 14
	TCCR1A |= _BV(WGM11) | _BV(COM1B1) | _BV(COM1A1);
	TCCR1B |= _BV(WGM13) | _BV(WGM12);
	
	// Set top value
	ICR1 = TOP;
}


void set_angle(int angle){
	SERVO_OCnx = (SERVO_MAX - SERVO_MIN) * angle;
	TCCR1B |= _BV(CS10);
}


void spin(int angle){
	const int fe = angle * 11 + 500;
	SERVO_PORT |= _BV(SERVO_PIN);
	_delay_ms(500);
	SERVO_PORT &= ~_BV(SERVO_PIN);
	_delay_ms(25);

}







