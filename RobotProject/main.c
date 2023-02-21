#include <avr/io.h>
#include "IR.h"
#include "motor_control.h"
#include "usart.h"
#include <stdlib.h>
#include <util/delay.h>

void test(void);
unsigned char * tofrray(uint8_t number);

int main(void)
{
	initIRSensors();
	pwm_timer_init();	
	set_speed(30, 30);
	_delay_ms(20);
	usart_init();

    while (1) 
    {

		test();
		
		
    }
}



unsigned char * tofrray(uint8_t number)
{
	int n = log10(number) + 1;
	int i;
	char *numberArray = calloc(n, sizeof(char));
	for (i = n-1; i >= 0; --i, number /= 10)
	{
		numberArray[i] = (number % 10) + '0';
	}
	return numberArray;
}


void test(void){
	float track_dir = getTrackDirection();

	// 1 -> 100, 0
	//-1 -> 0, 100
	// 0 -> 50, 50
	set_speed((track_dir+1) * 50, (track_dir-1) * -50);

	_delay_ms(3);
}
