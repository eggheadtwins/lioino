#include <avr/io.h>
#include "IR.h"
#include "motor_control.h"
#include "usart.h"
#include <stdlib.h>
#include <util/delay.h>

void test(void);

int main(void) {
	usart_init();
	initIRSensors();
	
    while (1) {
		test();
		_delay_ms(500);
    }
}


void test(void) {
	uint8_t track_dir = getTrackDirection();

	// 2 -> 50, 0
	// 1 -> 25, 25
	// 0 -> 0, 50
	set_speed(track_dir * 25, (track_dir-2) * -25);

	_delay_ms(3);
}
