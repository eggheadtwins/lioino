#include <avr/io.h>
#include <avr/sfr_defs.h>

#ifndef F_CPU
#define F_CPU 16000000UL
#endif


#define SERVO_DDRx

#ifdef SERVO_DDRx
#define SERVO_PIN
#endif


#ifdef SERVO_PIN
#define SERVO_OCnx
#endif

#define SERVO_MINIMUM_ANGLE 0
#define SERVO_MAXIMUM_ANGLE 180

#define PRESCALER 8



void start_timer(){
	
	
	
}


void servo_pwm_init(void){
	
	
	
}


void set_angle(int angle){
	
	
	
}



