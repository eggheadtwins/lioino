#include <avr/io.h>
#include "IR.h"
#include "motor_control.h"
#include "usart.h"
#include "ultrasonic.h"
#include "servo.h"
#include <stdlib.h>
#include <util/delay.h>
#include <avr/interrupt.h>

volatile int lapDetectionWhite;
volatile int lapDetectionBlack;
volatile int laps;

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
	lapDetectionWhite = 0;
	lapDetectionBlack = 0;
	laps = 0;
	
	while(0){
		set_angle();
	}

	while(1){
		if(command == START){
			set_speed(100, 100);
			_delay_ms(600);
			
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
	
	if(track_dir == 1000 && lapDetectionBlack == 0) {
		lapDetectionBlack = 1;
		lapDetectionWhite = 0;
	} else if(track_dir == 0 && lapDetectionBlack == 1) {
		lapDetectionWhite = 1;
	} else if(track_dir == 1000 && lapDetectionWhite == 1) {
		lapDetectionBlack = 2;
	} else if(track_dir == 0 && lapDetectionBlack == 2) {
		// lap detected!
		lapDetectionBlack = 0;
		lapDetectionWhite = 0;
		laps++;
		set_speed(0,0);
		_delay_ms(500);
	}
	
	int leftMotorSpeed;
	int rightMotorSpeed;
	
	if(track_dir != 0 && track_dir != 1000) {
		// the closer to 500, scalar moves closer to 6 -> higher speed
		int middleDist = track_dir - 500;
		if(middleDist < 0)
			middleDist *= -1;
		// range from [0 ~ 500]: 0 -> 6, 500 -> 11
		int scalar = (middleDist / 100) + 6;
		
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
