
#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>
#include "IR.h"
#include "motor_control.h"
#include "usart.h"
#include "servo.h"


void init(void){
	initIRSensors();
	pwm_timer_init();
	usart_init();
}


void test_IR(void){
	usart_send_8bit(adc_read(ADC5D));
	usart_send_8bit(adc_read(ADC4D));
	usart_send_8bit(adc_read(ADC3D));
	_delay_ms(500);
}



void test_motors(void){
	set_speed(10, 10);
	_delay_ms(500);
	set_speed(30, 30);
	_delay_ms(500);
	set_speed(50, 50);
	_delay_ms(500);	
	set_speed(10, 10);
	_delay_ms(500);
	set_speed(0, 30);
	_delay_ms(500);	
	set_speed(30, 0);	
		
}


void test_servo(){
	// Rotate 0 to 180.
	
	
}
