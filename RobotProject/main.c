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
		getTrackDirection();
		usart_send_char('\n');
		_delay_ms(500);
    }
}


void test(void) {
	uint8_t track_dir = getTrackDirection();

	// 1 -> 100, 0
	//-1 -> 0, 100
	// 0 -> 50, 50
	set_speed((track_dir+1) * 50, (track_dir-1) * -50);

	_delay_ms(3);
}
