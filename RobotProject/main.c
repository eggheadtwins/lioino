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

    while (1) {
		//set_angle(90);
		
    }
}


void test(void) {
	uint16_t track_dir = getTrackDirection();
	uint16_t scalar = 12; //min 10, because set_speed takes max 100
	uint16_t leftMotorSpeed = (1000-track_dir) / scalar;
	uint16_t rightMotorSpeed = (track_dir) / scalar;
	leftMotorSpeed = (int) leftMotorSpeed;
	rightMotorSpeed = (int) rightMotorSpeed;
	set_speed(leftMotorSpeed, rightMotorSpeed);
	_delay_ms(5);
}

void min(uint16_t a, uint16_t b){
	return (a < b)? a : b;
}


ISR(USART_RX_vect){
	command = UDR0;
	
	
}

