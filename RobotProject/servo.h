#include <avr/io.h>
#include <avr/sfr_defs.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#ifndef F_CPU
#define F_CPU 16000000UL
#endif


#define SERVO_DDRx DDRB

#ifdef SERVO_DDRx
#define SERVO_PIN PINB1
#define SERVO_PORT PORTB
#define SERVO_OCnx OCR1B
#endif

#define MINIMUM_ANGLE 0
#define MAXIMUM_ANGLE 180
#define AFTER_DELAY 5


volatile uint8_t previous_angle = 0;

void servo_init(void){
	SERVO_DDRx |= _BV(SERVO_PIN);
	
}

void ve(void){
	for (int i = 0; i <= 180; i++)
	{
		spin(i);
	}
	
	for (int i = 180; i >= 0; i--)
	{
		spin(i);
	}
	
}

void set_angle(int angle){
	if (angle < previous_angle){
		for (int i = angle; i >= MINIMUM_ANGLE; i--){
			spin(i);
		}
	}else if(angle > previous_angle){
		for (int i = previous_angle; i <= angle; i++){
			spin(i);	
		}	
	}
	
	previous_angle = angle;
	
}


void spin(int angle){
	const int fe = angle * 11 + 500;
	SERVO_PORT |= _BV(SERVO_PIN);
	_delay_ms(500);
	SERVO_PORT &= ~_BV(SERVO_PIN);
	_delay_ms(25);

}







