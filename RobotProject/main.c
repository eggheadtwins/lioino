#include <avr/io.h>
#include "IR.h"
#include "motor_control.h"
#include "usart.h"
#include <stdlib.h>
#include <util/delay.h>

void test(void);

int main(void) {
	
	pwm_timer_init();
	usart_init();
	initIRSensors();

    while (1) {
		test();
		_delay_ms(25);
    }
}


void test(void) {
	uint8_t track_dir = getTrackDirection();

	// 2 -> 40, 0
	// 1 -> 40, 40
	// 0 ->  0, 40
	set_speed(min((track_dir) * 40, 40), min((track_dir-2) * -40, 40));

	_delay_ms(3);
}



void min(uint8_t a, uint8_t b){
	return (a < b)? a : b;
}