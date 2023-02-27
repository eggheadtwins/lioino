#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sfr_defs.h>

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#define LEFT_MOTOR_PIN PIND5
#define RIGHT_MOTOR_PIN PIND6
#define MOTOR_PWM_TIMER_TOP 255
#define MOTORS_DDRx DDRD

#define MOTOR_MINIMUM_SPEED 0
#define MOTOR_MAXIMUM_SPEED 100

#ifdef LEFT_MOTOR_PIN
#define LEFT_MOTOR_OCnx OCR0B
#endif


#ifdef RIGHT_MOTOR_PIN
#define RIGHT_MOTOR_OCnx OCR0A
#endif

#define PRESCALER 8

void start_timer(void) {
	switch(PRESCALER){ // Sets MUX values based on pins.
		case 0:
			TCCR0B |= _BV(CS00);
			break;
		case 8:
			TCCR0B |= _BV(CS01);
			break;
		case 64:
			TCCR0B |= _BV(CS01) | _BV(CS00);	
			break;
		case 256:
			TCCR0B |= _BV(CS02);	
			break;
		case 1024:
			TCCR0B |= _BV(CS00) | _BV(CS02);	
			break;
		default:
			break;
	}
}


void set_speed(int left_motor_speed, int right_motor_speed) {			
	if (left_motor_speed >= MOTOR_MINIMUM_SPEED && left_motor_speed <= MOTOR_MAXIMUM_SPEED && 
		right_motor_speed >= MOTOR_MINIMUM_SPEED && right_motor_speed <= MOTOR_MAXIMUM_SPEED){
		
		
		//Set OCnx values
		LEFT_MOTOR_OCnx = (left_motor_speed * MOTOR_PWM_TIMER_TOP) / 100;
		RIGHT_MOTOR_OCnx = (right_motor_speed * MOTOR_PWM_TIMER_TOP) / 100;
	
		//Start Timer by setting Prescaler.
		start_timer();	
	}
}

void pwm_timer_init(void) {
	//Just to make sure the pins are set as OUTPUT.
	MOTORS_DDRx |= _BV(LEFT_MOTOR_PIN) | _BV(RIGHT_MOTOR_PIN);
	
	//Enable global interrupts
	sei();
	
	//Enable PWM for PIND5 and PIND6. Set timer to FAST PWM mode.
	TCCR0A |= _BV(COM0A1) | _BV(COM0B1) | _BV(WGM00) | _BV(WGM01);
	
	//Enable Overflow interrupt.
	TIMSK0 = _BV(TOIE0);	
}

ISR(TIMER0_OVF_vect) {
	//DO NOTHING
}