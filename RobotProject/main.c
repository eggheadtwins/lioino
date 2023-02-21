#include <avr/io.h>
#include "IR.h"
#include "motor_control.h"
#include "usart.h"
#include <util/delay.h>

void test(void);

int main(void)
{
	initIRSensors();
	pwm_timer_init();	
	set_speed(30, 30);
	_delay_ms(20);

    while (0) 
    {
		usart_send_chars(getTrackDirection());
    }
}



void test(void){
	float track_dir = getTrackDirection();

	// 1 -> 100, 0
	//-1 -> 0, 100
	// 0 -> 50, 50
	set_speed((track_dir+1) * 50, (track_dir-1) * -50);

	_delay_ms(3);
}
