#include <avr/io.h>
#include "IR.h"
#include "motor_control.h"
#include "usart.h"
#include "ultrasonic.h"
#include "servo.h"
#include <stdlib.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdbool.h>

volatile bool lapDetection;

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
	lapDetection = false;
	
	while(0){
		set_angle();
	}

	while(1){
		if(command == START){
			set_speed(40, 40);
			_delay_ms(200);
			
			while(1){
				test();
				
				if(command == STOP){
					set_speed(0, 0);
					break;
				}
			}
		}
	}
	
}


void test(void) {
	int track_dir = (int) getTrackDirection();
	
	if(track_dir == 0) {
		lapDetection = true;
	} else if(lapDetection && track_dir == 1000)	{ // lap detected!
		set_speed(0,0);
		_delay_ms(2000);
		return;
	} else if(lapDetection && track_dir < 650 && track_dir > 350)
		lapDetection = false;
	
	int leftMotorSpeed;
	int rightMotorSpeed;
	
	if(track_dir != 0 && track_dir != 1000) {
		// the closer to 500, scalar moves closer to 6 -> higher speed
		int middleDist = track_dir - 500;
		if(middleDist < 0)
			middleDist *= -1;
		// range from [0 ~ 500]: 0 -> 7, 500 -> 12
		int scalar = (middleDist / 100) + 9;
		
		leftMotorSpeed  = (1000-track_dir) / scalar;
		rightMotorSpeed = track_dir / scalar;
	} else {
		if(track_dir == 0) {
			leftMotorSpeed = 99;
			rightMotorSpeed = 20;
		} else {
			leftMotorSpeed = 0;
			rightMotorSpeed = 99;
		}
	}
	set_speed(leftMotorSpeed, rightMotorSpeed);
}

int min(int a, int b){
	return (a < b)? a : b;
}


ISR(USART_RX_vect){
	command = UDR0;
}
