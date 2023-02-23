#include <avr/io.h>
#include <avr/sfr_defs.h>

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#define PRESCALER 4
#define AREF REFS0

#define ADC_SIZE 10


void set_prescaler(void) {
	switch(PRESCALER){ // Set prescaler.
		case 2:
			ADCSRA |= _BV(ADPS0);
			break;
		case 4:
			ADCSRA |= _BV(ADPS1);
			break;
		case 8:
			ADCSRA |= _BV(ADPS1) | _BV(ADPS0);
			break;
		case 16:
			ADCSRA |= _BV(ADPS2);
			break;
		case 32:
			ADCSRA |= _BV(ADPS2) | (ADPS0);
			break;
		case 64:
			ADCSRA |= _BV(ADPS2) | _BV(ADPS1);
			break;
		case 128:
			ADCSRA |= _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);
			break;
		default:
			break;
	}	
	
}

void adc_init() {
	ADCSRA |= _BV(ADEN); // Enable ADC
	set_prescaler();
}



void pin_to_mux(uint8_t pin) {
	ADCSRA = _BV(ADEN); // Enable ADC
	set_prescaler();
	ADMUX |= _BV(AREF) | _BV(ADLAR) ; // VREF to AVCC (+5v) and Left adjust bits. 
	
	switch(pin){ // Sets MUX values based on pins.
		case ADC0D:
			break;
		case ADC1D:
			ADMUX |= _BV(MUX0);
			break;
		case ADC2D:
			ADMUX |= _BV(MUX1);
			break;
		case ADC3D:
			ADMUX |= _BV(MUX1)| _BV(MUX0);
			break;
		case ADC4D:
			ADMUX |= _BV(MUX2);
			break;
		case ADC5D:
			ADMUX |= _BV(MUX2) | _BV(MUX0); 
			break;
		default:
			break;
	}
	
	// Disable digital input buffer. Not necessary as it an analog signal. Just to save power.
	DIDR0 = _BV(pin);
}


void conversion_init(void) {
	ADCSRA |= _BV(ADSC); // Enable Start conversion bit.
}


uint8_t is_converting(void) {
	return (ADCSRA & _BV(ADSC));
}


uint8_t adc_read(uint8_t pin) {
	pin_to_mux(pin); // Change channel
	conversion_init();
	
	while(is_converting());
	
	return ADCH;

}


