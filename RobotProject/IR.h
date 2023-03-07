#include <avr/io.h>
#include <avr/sfr_defs.h>
#include "adc.h"

#define sensor_l ADC5D 
#define sensor_c ADC4D
#define sensor_r ADC3D

#define sensor_bl ADC1D
#define sensor_br ADC2D

void initIRSensors() {
	adc_init();
}

// return a value under 1 ([0-500]) when we are on the left side of the track,
// a value over 1 ([500-1000]) when we are on the right side of the track
// the farther away from 500, the more outward we are

uint16_t getTrackDirection() {
	// the higher the blacker, 8bit value
	uint16_t left_black   = read_adc(sensor_l);
	uint16_t right_black  = read_adc(sensor_r);
	uint16_t center_black = read_adc(sensor_c);
	uint16_t back_right_black = read_adc(sensor_br);
	uint16_t back_left_black = read_adc(sensor_bl);
	
	// right-sensor calibration
	uint16_t rightCalibration = right_black - ((left_black + center_black) / 2);
	right_black -= rightCalibration;
			
			/*
	uint8_t left[] = "\n\nLEFT: ";
	uint8_t right[] = "\nRIGHT: ";
	uint8_t center[] = "\nCENTER: ";
	uint8_t backleft[] = "\nBACK LEFT: ";
	uint8_t backright[] = "\nBACK RIGHT: ";
	
	usart_send_chars(left);
	usart_send_16bit(left_black);
	usart_send_chars(center);
	usart_send_16bit(center_black);
	usart_send_chars(right);
	usart_send_16bit(right_black);
	usart_send_chars(backleft);
	usart_send_16bit(back_left_black);
	usart_send_chars(backright);
	usart_send_16bit(back_right_black);*/
	
	// border checking??

	// outmost
	if(left_black > 670)
		return 1000;
	if(right_black < 360)
		return 0;
		
	// inner
	int average = ((int)(left_black + right_black + center_black))/3;
	
	// to be sure, I take borders again
	if(average > 670)
		return 1000;
	else if(average < 360)
		return 0;	
	
	// average range is 360 : 670
	average -= 360;		// 0 :  310
	average *= 3;		// 0 :  930
	average += 35;		// 35:  965
	
	// push middles towards the outside
	float pushIt = (average-500);
	pushIt = (pushIt * 1.32) + 500;
	
	average = (uint16_t) pushIt;	
	
	// clip borders
	if(average < 0)
		average = 0;
	else if(average > 1000)
		average = 1000;
	
	return average;
}