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
		test();
		
		
    }
}


void test(void) {
	uint16_t track_dir = getTrackDirection();
	uint16_t scalar = 10;
	if(track_dir < 680 && track_dir > 320)
		scalar /= 2;
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
	usart_send_char(UDR0);
	if(UDR0 == 'A'){
		while(1){
			test();
			
		}
	}else if(UDR0 == 'B'){
		while(1){
			
			
		}
		
	}
	
	
	
}

