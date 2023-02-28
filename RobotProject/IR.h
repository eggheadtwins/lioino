#include <avr/io.h>
#include <avr/sfr_defs.h>
#include "adc.h"

#define sensor_l ADC5D 
#define sensor_c ADC4D
#define sensor_r ADC3D

#define sensor_bl ADC1D
#define sensor_br ADC0D

#define IR_threshold 600
#define max_IR_val 1024

#define FLIP_DIRECTION 1

#define outmost_smoothness 8 // lower values will make the robot turn slower in corners



void initIRSensors() {
	adc_init();
}

// return a value under 1 ([0-500]) when we are on the left side of the track,
// a value over 1 ([500-1000]) when we are on the right side of the track
// the higher the absolute value, the more outward we are

uint16_t getTrackDirection() {
	// the higher the blacker, 8bit value
	conversion_init();
	uint16_t left_black   = read_adc(sensor_l);
	conversion_init();
	uint16_t right_black  = read_adc(sensor_r);
	conversion_init();
	uint16_t center_black = read_adc(sensor_c);
	conversion_init();
	uint16_t back_left_black = read_adc(sensor_bl);
	conversion_init();
	uint16_t back_right_black = read_adc(sensor_br);
	
	// right-sensor calibration
	uint8_t rightCalibration = right_black - ((left_black + center_black) / 2);
	right_black -= rightCalibration;
	
	// back sensors go below 800 if outward, otherwise higher
	
	
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
	usart_send_16bit(back_right_black);
	
	
	// border checking??

	// outmost
	if(left_black > 600)
		return (1000-left_black)/outmost_smoothness;
	if(right_black < 300)
		return 1000-(right_black/outmost_smoothness)-100;
		
	// inner
	uint16_t average = left_black + center_black + right_black;
	average/=3;
	
	if(average > 700)
		average = 700;
	else if(average < 500)
		average = 500;
	
	// we are in between 500 and 700 here, we want range from 700 to 300 (middle is 500)
	average -= 700;
	average *= -2;
	average += 300;
	
	if(average < 500 && average > 420)
		average -= 100;
	else if(average > 500 && average < 580)
		average += 100;

	return average;
}