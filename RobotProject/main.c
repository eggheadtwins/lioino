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
	//set_speed(30, 30);
	/*
	
	
		NOT BELOW 70 WHEN OTHER TURN OFF
		UNDER 35 FUCK WHEN OTHER IS TURNED ON
	*/
    /* Replace with your application code */
	
	
		
	
    while (0) 
    {
		
		
		
		usart_send_chars(getTrackDirection());
    }
}



void test(void){
	float track_dir = getTrackDirection();
		
		
	if(track_dir == MIDDLEISH){
		set_speed(30,35);
	}
		
		
	if(track_dir == -MIDDLEISH){
		set_speed(35, 30);
			
	}
		
		
		if(track_dir == OUTWARD){
			set_speed(18,35);
			
			
		}
		
		
		if(track_dir == -OUTWARD){
			set_speed(35,18);
			
			
		}
		
		
		
		if(track_dir == BORDER){
			set_speed(0, 100);
			
			
		}
		
		
		if(track_dir == -BORDER){
			set_speed(100, 0);
			
		}
		
		if(track_dir == WEIRD_MEASUREMENT){
			set_speed(0, 0);
		}
		
		_delay_ms(3);	
	
	
}
