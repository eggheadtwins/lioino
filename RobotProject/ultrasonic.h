#include <avr/io.h>
#include <avr/sfr_defs.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define TRIGGER_DDRx DDRD
#define TRIGGER_PORTx PORTD
#define TRIGGER_PIN PIND7
#define TRIGGER_FREQUENCY 15

#define ECHO_DDRx DDRB
#define ECHO_PORTx PORTB
#define ECHO_INTx PCINT0
#define ECHO_PIN PINB0
#define ECHO_INTERRUPT_REGISTER PCIE0

#if ECHO_INTERRUPT_REGISTER == PCIE0
#	define ECHO_PIN_CHANGE_REGISTER PCMSK0
#	define ECHO_INTx_VECTOR PCINT0_vect

#elif ECHO_INTERRUPT_REGISTER == PCIE1
#	define ECHO_PIN_CHANGE_REGISTER PCMSK1
#	define ECHO_INTx_VECTOR PCINT1_vect

#elif ECHO_INTERRUPT_REGISTER == PCIE2
#	define ECHO_PIN_CHANGE_REGISTER PCMSK2
#	define ECHO_INTx_VECTOR PCINT2_vect

#endif


#define PRESCALER 8

#if PRESCALER == 1
#	define PRESCALER_BITS (_BV(CS10))
#elif PRESCALER == 8 
#	define PRESCALER_BITS (_BV(CS11))
#elif PRESCALER == 64 
#	define PRESCALER_BITS (_BV(CS11) | _BV(CS10))
#elif PRESCALER == 256 
#	define PRESCALER_BITS (_BV(CS12))
#elif PRESCALER == 1024
#	define PRESCALER_BITS (_BV(CS12) | _BV(CS10))
#endif

volatile uint16_t pulse_width = 0;
volatile uint8_t is_triggered = 0;

void ultrasonic_init(){
	// Set Trigger pin as OUTPUT, and ECHO pin as INPUT.
	TRIGGER_DDRx |= _BV(TRIGGER_PIN);
	ECHO_DDRx &= ~_BV(ECHO_PIN);
	
	// Enable PORTB pins to interrupt.
	PCICR |= _BV(ECHO_INTERRUPT_REGISTER);
	
	// Enable ECHO pin to interrupt.
	ECHO_PIN_CHANGE_REGISTER |= _BV(ECHO_INTx);
	
	// Enable global interrupts.
	sei();
	
}


void trigger(void){
	TRIGGER_PORTx |= _BV(TRIGGER_PIN);
	_delay_ms(TRIGGER_FREQUENCY);
	TRIGGER_PORTx &= ~_BV(TRIGGER_PIN);
	
}



ISR(ECHO_INTx_VECTOR){
	// ECHO pin goes from HIGH to LOW, when signal received. 
	if (is_triggered == 1){
		TCCR1B = is_triggered = TCNT1 = 0; // Stop timer, reset ticks.
		pulse_width = TCNT1;

		
	// The ECHO pin is set HIGH when triggered. So, the timer starts here.
	} else if (is_triggered == 0){
		is_triggered = 1;
		TCCR1B |= PRESCALER_BITS; // Start timer
	}
	
	
}
