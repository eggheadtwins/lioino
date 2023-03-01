#include <avr/io.h>
#include "IR.h"
#include "motor_control.h"
#include "usart.h"
#include "ultrasonic.h"
#include <stdlib.h>
#include <util/delay.h>
#include <avr/interrupt.h>

volatile int maxmotorspeed = 40;

void test(void);

int main(void) {
	pwm_timer_init();
	usart_init();
	initIRSensors();
	ultrasonic_init();

    while (1) {
		test();
    }
}


void test(void) {
	uint16_t track_dir = getTrackDirection();
	uint16_t scalar = 12; //min 10, because set_speed takes max 100
	if(track_dir > 350 && track_dir < 650)
		scalar -= 2;
	uint16_t leftMotorSpeed = (track_dir) / scalar;
	uint16_t rightMotorSpeed = (1000-track_dir) / scalar;
	leftMotorSpeed = (int) leftMotorSpeed;
	rightMotorSpeed = (int) rightMotorSpeed;
	set_speed(leftMotorSpeed, rightMotorSpeed);
	_delay_ms(5);
}

void min(uint16_t a, uint16_t b){
	return (a < b)? a : b;
}

