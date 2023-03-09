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

uint8_t lapdetected[] = "Lap detected!\n";
volatile uint8_t command;

#define START 'A'
#define PENALTY 'B'
#define STOP 'C'

void followTrack(void);

int main(void) {
	pwm_timer_init();
	usart_init();
	initIRSensors();
	ultrasonic_init();
	//servo_init();
	lapDetectionWhite = 0;
	lapDetectionBlack = 0;
	laps = 0;

	while(1){
		usart_send_16bit(pulse_width);
		usart_send_char('\n');
		if(command == START){
			// wheelie!!!
			/*
			set_speed(100, 100);
			_delay_ms(200);
			*/
			
			while(1){
				followTrack();
				
				if(command == STOP){
					set_speed(0, 0);
					break;
				}
			}
		}
	}
}


void followTrack(void) {
	int track_dir = (int) getTrackDirection();
	
	//lap detection
	/*
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
		usart_send_chars(lapdetected);
		laps++;
	} else if(track_dir < 580 && track_dir > 420) {
		lapDetectionBlack = 0;
		lapDetectionWhite = 0;
	}
	*/
	
	int leftMotorSpeed;
	int rightMotorSpeed;
	
	if(track_dir != 0 && track_dir != 1000) {
		// the closer to 500, scalar moves closer to 6 -> higher speed
		float middleDist = (float)track_dir - 500;
		if(middleDist < 0)
			middleDist *= -1;
		// range from [0 ~ 500]: 0 -> 7, 500 -> 13
		float scalar = (middleDist / 100.0) + 7;
		if(scalar <= 7.2)
			scalar -= 0.8;
		//free(&middleDist);
		
		leftMotorSpeed  = (int) (( (float) (1000-track_dir)	) / scalar);
		rightMotorSpeed = (int) (( (float)  track_dir		) / scalar);
		//free(&scalar);
	} else {
		if(track_dir == 0) {
			leftMotorSpeed = 99;
			rightMotorSpeed = 30;
		} else {
			leftMotorSpeed = 0;
			rightMotorSpeed = 99;
		}
	}
	//free(&track_dir);
	
	if(pulse_width > 80 && pulse_width < 450) {
		// brake because obstacle is close
		// 450 -> 1
		//  80 -> 100
		
		float breakMultiplier = -1 * ((float) pulse_width);
		breakMultiplier += 450;	// 450 -> 0, 80 -> 380
		breakMultiplier /= 3.8;	// 450 -> 0, 80 -> 100
		breakMultiplier += 1;	// 450 -> 1, 80 -> 101
		
		if(breakMultiplier < 90) {
			leftMotorSpeed /= (int) breakMultiplier;
			rightMotorSpeed /= (int) breakMultiplier;
		} else {
			leftMotorSpeed = 0;
			rightMotorSpeed = 0;
		}
	}
	
	set_speed(leftMotorSpeed, rightMotorSpeed);
}

ISR(USART_RX_vect){
	command = UDR0;
}
