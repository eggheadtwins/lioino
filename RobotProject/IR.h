#include <avr/io.h>
#include "adc.h"

#define sensor_l PINC5
#define sensor_c PINC4
#define sensor_r PINC3

#define WEIRD_MEASUREMENT -100

#define FLIP_DIRECTION 1


void initIRSensors() {
	adc_init();
}

// return a negative value when we are on the left side of the track,
// a positive value when we are on the right side of the track
// the higher the absolute value, the more outward we are

float getTrackDirection() {
	// the higher the blacker
	uint16_t left_black   = get_adc(sensor_l);
	uint16_t right_black  = get_adc(sensor_r);
	uint16_t center_black = get_adc(sensor_c);

	if(left_black > 950 || left_black < 10)
		return -1;
	if(right_black > 950 || right_black < 10)
		return 1;

	// positive if left is the blackest
	uint16_t gradient = (left_black - center_black) + (center_black - right_black);
	// divide by the approx. max./2 to get val. between 0 and 2, minus one to get range from -1 to 1;
	gradient = gradient / 250 - 1;
	// clip the value
	if(gradient > 1) {
		gradient = 1;
	} else if(gradient < -1) {
		gradient = -1;
	}
	
	return gradient;
}