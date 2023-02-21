

#include <avr/io.h>
#include <avr/interrupt.h>

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#define LEFT_MOTOR_PIN PIND5
#define RIGHT_MOTOR_PIN PIND6
#define MOTOR_PWM_TIMER_TOP 255
#define MOTORS_DDR DDRD

#define MOTOR_MINIMUM_SPEED 0
#define MOTOR_MAXIMUM_SPEED 100

#ifdef LEFT_MOTOR_PIN
#define LEFT_MOTOR_OCnx OCR0B
#endif


#ifdef RIGHT_MOTOR_PIN
#define RIGHT_MOTOR_OCnx OCR0A
#endif


void set_speed(int left_motor_speed, int right_motor_speed){			
	if (left_motor_speed >= MOTOR_MINIMUM_SPEED && left_motor_speed <= MOTOR_MAXIMUM_SPEED && 
		right_motor_speed >= MOTOR_MINIMUM_SPEED && right_motor_speed <= MOTOR_MAXIMUM_SPEED){
		
		//Set OCnx values
		LEFT_MOTOR_OCnx = (left_motor_speed * MOTOR_PWM_TIMER_TOP) / 100;
		RIGHT_MOTOR_OCnx = (right_motor_speed * MOTOR_PWM_TIMER_TOP) / 100;
	
		//Start Timer by setting no Prescaler.
		TCCR0B |= (1<<CS00) | (1<<CS02);		
	}
}



void pwm_timer_init(void){
	//Just to make sure the pins are set as OUTPUT.
	MOTORS_DDR |= (1<<LEFT_MOTOR_PIN) | (1<<RIGHT_MOTOR_PIN);
	
	//Enable global interrupts
	sei();
	
	//Enable PWM for PIND5 and PIND6. Set timer to FAST PWM mode.
	TCCR0A |= (1<<COM0A1) | (1<<COM0B1) | (1<<WGM00) | (1<<WGM01);
	
	//Enable Overflow interrupt.
	TIMSK0 = (1<<TOIE0);	
}


ISR(TIMER0_OVF_vect){
	//DO NOTHING
	
	
}