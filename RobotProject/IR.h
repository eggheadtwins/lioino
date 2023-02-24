#include <avr/io.h>
#include <avr/sfr_defs.h>
#include "adc.h"

#define sensor_l ADC5D 
#define sensor_c ADC4D
#define sensor_r ADC3D

#define IR_threshold 120

#define FLIP_DIRECTION 1



void initIRSensors() {
	adc_init();
}

// return a negative value when we are on the left side of the track,
// a positive value when we are on the right side of the track
// the higher the absolute value, the more outward we are

uint8_t getTrackDirection() {
	// the higher the blacker, 8bit value
	uint8_t left_black   = adc_read(sensor_l);
	conversion_init();
	uint8_t right_black  = adc_read(sensor_r);
	conversion_init();
	uint8_t center_black = adc_read(sensor_c);
	conversion_init();
	
	
	usart_send_8bit(left_black);
	usart_send_char('\n');
	usart_send_8bit(right_black);
	usart_send_char('\n');
	usart_send_8bit(center_black);
	usart_send_char('\n');
	
	
	if(left_black > 255 - IR_threshold || left_black < IR_threshold)
		return 0;
	if(right_black > 255- IR_threshold || right_black < IR_threshold)
		return 1;
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