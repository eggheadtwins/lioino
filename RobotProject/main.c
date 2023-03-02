#include <avr/io.h>
#include "IR.h"
#include "motor_control.h"
#include "usart.h"
#include "ultrasonic.h"
#include "servo.h"
#include <stdlib.h>
#include <util/delay.h>
#include <avr/interrupt.h>

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

    while (1) {
		if(command == 'A'){
			test();
		}else if(command == 'B'){
			set_speed(0, 0);
		}else{
			usart_send_chars(command);
		}
		
		
		//test();
		
		
    }
}


void test(void) {
	uint16_t track_dir = getTrackDirection();
	// the closer to 500, scalar moves closer to 6 -> higher speed
	int middleDist = ((int)track_dir - 500);
	if(middleDist < 0)
		middleDist *= -1;
	// range from [0 ~ 500]: 0 -> 6, 500 -> 11
	uint16_t scalar = middleDist / 100 + 6;
	
	usart_send_16bit(scalar);
	

	uint16_t leftMotorSpeed = min((1000-track_dir) / scalar, 1000);
	uint16_t rightMotorSpeed = min((track_dir) / scalar, 1000);
	leftMotorSpeed = (int) leftMotorSpeed;
	rightMotorSpeed = (int) rightMotorSpeed;
	set_speed(leftMotorSpeed, rightMotorSpeed);
	_delay_ms(1);
}

int min(int a, int b){
	return (a < b)? a : b;
}


ISR(USART_RX_vect){
//	usart_send_char(UDR0);
	command = UDR0;
//	if(UDR0 == 'A'){
//		while(1){
//			test();
//			
//			if(UDR0 == 'B'){
//				set_speed(0, 0);
//				return;
//			}
//		}
//		
//	}
	
	
	
}

