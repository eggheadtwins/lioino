

#include <avr/io.h>

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

void adc_init(uint8_t * pin){
	sei();
	ADMUX |= (1<<REFS0); // VREF to AVCC (+5v).

	while(*pin++){
		switch(*pin){ // Sets MUX values based on pins.
			case ADC0D:
				break;
			case ADC1D:
				ADMUX |= (1<<MUX0);
				break;
			case ADC2D:
				ADMUX |= (1<<MUX1);
				break;
			case ADC3D:
				ADMUX |= (1<<MUX1) |(1<<MUX0);
				break;
			case ADC4D:
				ADMUX |= (1<<MUX2);
				break;
			case ADC5D:
				ADMUX |= (1<<MUX2) | (1<<MUX0);
				break;
			default:
				break;
		}
		DIDR0 |= (1<<*pin);

	}
	ADCSRA = (1<<ADEN) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0); // Enable ADC and 128 Prescaler.

}


void conversion_init(void){
	ADCSRA |= (1<<ADSC); // Enable Start conversion bit.
}

uint16_t get_adc(){
	while(!(ADCSRA & (1<<ADIF)));
	return ADC; // 10-bit.

}
