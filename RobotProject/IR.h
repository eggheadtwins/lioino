#include <avr/io.h>
#include <avr/sfr_defs.h>
#include "adc.h"

#define sensor_l PINC5
#define sensor_c PINC4
#define sensor_r PINC3

#define IR_threshold 10

#define FLIP_DIRECTION 1



void initIRSensors() {
	adc_init();
}

// return a negative value when we are on the left side of the track,
// a positive value when we are on the right side of the track
// the higher the absolute value, the more outward we are

float getTrackDirection() {
	// the higher the blacker, 8bit value
	uint8_t left_black   = adc_read(sensor_l);
	uint8_t right_black  = adc_read(sensor_r);
	uint8_t center_black = adc_read(sensor_c);

	if(left_black > 255 - IR_threshold || left_black < IR_threshold)
		return -1;
	if(right_black > 255- IR_threshold || right_black < IR_threshold)
		return 1;

	// positive if left is the blackest
	uint16_t gradient = (left_black - center_black) + (center_black - right_black);
	// divide by the approx. max. to normalize and get range ~[-1 : 1]
	//gradient = gradient / 180; // edit this to calibrate sensors
	// clip the value
	if(gradient > 1) {
		gradient = 1;
	} else if(gradient < -1) {
		gradient = -1;
	}
	
	return gradient * FLIP_DIRECTION;
}