#include <avr/io.h>
#include "IR.h"
#include "motor_control.h"
#include "usart.h"
#include "ultrasonic.h"
#include "servo.h"
#include <stdlib.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define START 'A'
#define PENALTY 'B'
#define STOP 'C'

volatile int maxmotorspeed = 40;
volatile uint8_t command;
void test(void);

int main(void) {
	pwm_timer_init();
	usart_init();
	initIRSensors();
	ultrasonic_init();
	servo_init();
	sei();

	while(1){
		if(command == START){
			test();
			
		}else if (command  == PENALTY){
			set_speed(0, 0);
			_delay_ms(5000);
			test();
			
		}else if(command == STOP){
			set_speed(0, 0);
		}


	}
}


void test(void) {
	uint16_t track_dir = getTrackDirection();
	uint16_t leftMotorSpeed;
	uint16_t rightMotorSpeed;
	
	if(track_dir > 2 && track_dir < 998) {
		// the closer to 500, scalar moves closer to 6 -> higher speed
		int middleDist = ((int)track_dir - 500);
		if(middleDist < 0)
			middleDist *= -1;
		// range from [0 ~ 500]: 0 -> 6, 500 -> 11
		uint16_t scalar = (uint16_t) (middleDist / 100 + 6);

		leftMotorSpeed  = (int) min((1000-track_dir) / scalar, 100);
		rightMotorSpeed = (int) min((track_dir) / scalar, 100);
	} else {
		leftMotorSpeed  = (1000-track_dir) / 100;
		rightMotorSpeed = track_dir / 100;
	}
	set_speed(leftMotorSpeed, rightMotorSpeed);
}

int min(int a, int b){
	return (a < b)? a : b;
}


ISR(USART_RX_vect){
	command = UDR0;	
	
}

