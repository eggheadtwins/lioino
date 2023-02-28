#include <avr/io.h>
#include <avr/sfr_defs.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define TRIGGER_DDRx
#define TRIGGER_PORTx
#define TRIGGER_PIN
#define TRIGGER_FREQUENCY

#define ECHO_DDRx
#define ECHO_PORTx
#define ECHO_INTx
#define ECHO_PIN
#define ECHO_INTx_VECTOR

#define INTERRUPT_SENSE_CONTROL (_BV(ISC00));

volatile uint16_t pulse_width = 0;
volatile uint8_t is_pulse_recieved = 0;

void ultrasonic_init(){
	TRIGGER_DDRx |= _BV(TRIGGER_PIN);
	ECHO_PIN &= ~_BV(ECHO_PIN);
	
	PCICR |= _BV(PCIE0);
	PCMSK0 |= _BV(ECHO_INTx);
	
	sei();
	
}


void trigger(void){
	TRIGGER_PORTx &= ~_BV(TRIGGER_PIN);
	_delay_ms(TRIGGER_FREQUENCY);
	TRIGGER_PORTx |= _BV(TRIGGER_PIN);
	
}

uint16_t get_pulse_width(void){
	while(is_pulse_recieved == 0);
	
	return pulse_width;
	
}

ISR(ECHO_INTx_VECTOR){
	if ((ECHO_PORTx & (1<<ECHO_PIN)) == 0){
		TCCR1B = 0;	// Stop timer.
		pulse_width = TCNT1;
		is_pulse_recieved = 1;
		TCNT1 = 0;
	} else {
		is_pulse_recieved = 0;
		TCCR1B |= (1<<CS10); // Start timer
	}	
	
	
}