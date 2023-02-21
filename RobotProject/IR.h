#include <avr/io.h>
#include <stdbool.h>


#define sensor_l PINC5
#define sensor_c PINC4
#define sensor_r PINC3

#define WEIRD_MEASUREMENT -100

#define MIDDLEISH 0.3
#define OUTWARD 0.75
#define BORDER 1

#define FLIP_DIRECTION 1


void initIRSensors() {
	adc_init([sensor_c, sensor_l, sensor_r]);
}

// return a negative value when we are on the left side of the track,
// a positive value when we are on the right side of the track
// the higher the absolute value, the more outward we are
// if -100 is returned, the measurement was weird.

float getTrackDirection() {
	conversion_init();
	bool left_black   = (PINB == (1<<sensor_l));
	bool right_black  = (PINB == (1<<sensor_r));
	bool center_black = (PINB == (1<<sensor_c));
	
	// Middleish left
	if(!left_black && center_black && right_black ) {
		return -MIDDLEISH * FLIP_DIRECTION;
	} 
	// Middleish right
	else if(!left_black && !center_black && right_black) {
		return MIDDLEISH * FLIP_DIRECTION;
	}
	// right
	else if(left_black && center_black && right_black) {
		return OUTWARD * FLIP_DIRECTION;
	}
	// left
	else if(!left_black && !center_black && !right_black) {
		return -OUTWARD * FLIP_DIRECTION;
	}
	// right border
	else if(left_black && center_black && !right_black) {
		return BORDER * FLIP_DIRECTION;
	}
	// left border
	else if(left_black && !center_black && !right_black ) {
		return -BORDER * FLIP_DIRECTION;
	} else {
		return WEIRD_MEASUREMENT;
	}
}