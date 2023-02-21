#include <avr/io.h>
#include "IR.h"
#include "motor_control.h"

int main(void)
{
	initIRSensors();
	pwm_timer_init();
	
    /* Replace with your application code */
    while (1) 
    {
		
		float track_dir = getTrackDirection();
		
		
		if(track_dir == MIDDLEISH){
			set_speed(50,70);
		}
		
		
		if(track_dir == -MIDDLEISH){
			set_speed(70, 50);
			
		}
		
		
		if(track_dir == OUTWARD){
			set_speed(30,70);
			
			
		}
		
		
		if(track_dir == -OUTWARD){
			set_speed(70,30);
			
			
		}
		
		
		
		if(track_dir == BORDER){
			set_speed(30, 100);
			
			
		}
		
		
		if(track_dir == -BORDER){
			set_speed(100, 30);
			
		}
		
		if(track_dir == WEIRD_MEASUREMENT){
			set_speed(50, 50);
		}
		
		
    }
}

