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
#	define SERVO_OCnx OCR1A
#endif

#define TOP 20000
#define SERVO_MIN 950
#define SERVO_MAX 4700
#define RANGE (SERVO_MAX - SERVO_MIN)/180  

void servo_init(void){
	// Set Servo pin as OUTPUT.
	SERVO_DDRx |= _BV(SERVO_PIN);
	
	// Timer 1 - PWM - Mode 14 - PRESCALER 8 - 50 Hz
	TCCR1A |= _BV(WGM11) | _BV(COM1A1);
	TCCR1B |= _BV(WGM13) | _BV(WGM12) | _BV(CS11);
	ICR1 = TOP;
}


void set_angle(int angle){
	angle += 15;
	if(angle > 180)
		angle = 180;
	//			   (SERVO_MAX - SERVO_MIN)
	// SERVO_MIN + ----------------------- x angle
	//                        180
	SERVO_OCnx = SERVO_MIN + ((int) RANGE * angle);
}

