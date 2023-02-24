#include <avr/io.h>
#include <avr/sfr_defs.h>
#include "adc.h"

#define sensor_l ADC5D 
#define sensor_c ADC4D
#define sensor_r ADC3D

#define IR_threshold 600
#define max_IR_val 1024

#define FLIP_DIRECTION 1



void initIRSensors() {
	adc_init();
}

// return a value under 1 ([0-1]) when we are on the left side of the track,
// a value over 1 ([1-2]) when we are on the right side of the track
// the higher the absolute value, the more outward we are

uint8_t getTrackDirection() {
	// the higher the blacker, 8bit value
	conversion_init();
	uint16_t left_black   = read_adc(sensor_l);
	conversion_init();
	uint16_t right_black  = read_adc(sensor_r);
	conversion_init();
	uint16_t center_black = read_adc(sensor_c);
	
	uint8_t left[] = "\n\nLEFT: ";
	uint8_t right[] = "\nRIGHT: ";
	uint8_t center[] = "\nCENTER: ";
	
	usart_send_chars(left);
	usart_send_16bit(left_black);
	usart_send_chars(right);
	usart_send_16bit(right_black);
	usart_send_chars(center);
	usart_send_16bit(center_black);
	
	
	if(left_black > 700)// || 
		return 0;
	if(right_black < 500)//|| right_black > max_IR_val - IR_threshold)
		return 2;
/*
	// positive if left is the blackest
	float gradient = (left_black - center_black) + (center_black - right_black);
	// divide by the approx. max. to normalize and get range ~[-1 : 1]
	//gradient = gradient / 180; // edit this to calibrate sensors
	// clip the value
	if(gradient > 1) {
		gradient = 1;
	} else if(gradient < -1) {
		gradient = -1;
	}
	
	return gradient * FLIP_DIRECTION;
*/
	return 1;
}