#include <avr/io.h>
#include <avr/interrupt.h>

#ifndef F_CPU
#define F_CPU 16000000UL
#endif


void adc_init(){
	sei(); // Enable global interrupts.
	ADMUX |= (1<<REFS0); // VREF to AVCC (+5v).
	ADCSRA |= (1<<ADEN) | (1<<ADPS2) | (1<<ADPS0); // Enable ADC and 32 Prescaler.
}


void change_channel(uint8_t pin){
	switch(pin){ // Sets MUX values based on pins.
		case ADC0D:
			break;
		case ADC1D:
			ADMUX = (1<<MUX0);
			break;
		case ADC2D:
			ADMUX = (1<<MUX1);
			break;
		case ADC3D:
			ADMUX = (1<<MUX1) |(1<<MUX0);
			break;
		case ADC4D:
			ADMUX = (1<<MUX2);
			break;
		case ADC5D:
			ADMUX = (1<<MUX2) | (1<<MUX0);
			break;
		default:
			break;
	}
}


void conversion_init(void){
	ADCSRA |= (1<<ADSC); // Enable Start conversion bit.
}

unsigned int get_adc(uint8_t pin){
	//DIDR0 = (1<<pin); //Disable digital input buffer for the pin.
	change_channel(pin); // Change channel
	conversion_init();
	
	while((ADCSRA & (1<<ADSC)));
	return ADC; // 10-bit.

}


