#include <avr/io.h>
#include <avr/sfr_defs.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define TRIGGER_DDRx DDRB
#define TRIGGER_PORTx PORTB
#define TRIGGER_PIN PINB4
#define TRIGGER_FREQUENCY 15 // Manual trigger delay between Trigger HIGH and LOW. 

#define ECHO_DDRx DDRB
#define ECHO_PORTx PORTB
#define ECHO_INTx PCINT5
#define ECHO_PIN PINB5
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


#define PRESCALER 64

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

#define TIMER2_OVERFLOW 10

#define PULSE_WIDTH_MAX
#define PULSE_WIDTH_MIN

volatile int16_t pulse_width = 0; // Stores the time taken to reach the receiver. 
volatile uint8_t is_triggered = 0; // If the transmitter is set HIGH. 
volatile uint8_t i = 0;

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
	
	// Can be manually or automatically triggered. 
	if(TIMER2_OVERFLOW != 0){
		// Set CTC Mode.
		TCCR2B |= _BV(WGM12);

		// Enable CTC interrupt.
		TIMSK2 |= _BV(OCIE2A);

		// Set TOP value.
		OCR2A = 255;

		// Set MAX (1024) Prescaler.
		TCCR2B |= _BV(CS22) | _BV(CS21) | _BV(CS20);		
	}

	
}


void trigger(void){
	TRIGGER_PORTx |= _BV(TRIGGER_PIN);
	_delay_ms(TRIGGER_FREQUENCY);
	TRIGGER_PORTx &= ~_BV(TRIGGER_PIN);
	
}



ISR(ECHO_INTx_VECTOR){
	// ECHO pin goes from HIGH to LOW, when signal received. 
	if (is_triggered == 1){
		pulse_width = TCNT1;
		
//		if(pulse_width >= PULSE_WIDTH_MAX){
//			pulse_width = 1;
//		}else if(pulse_width <= PULSE_WIDTH_MIN){
//			pulse_width = -1;
//		}
		
		TCCR1B = is_triggered = TCNT1 = 0; // Stop timer, reset ticks.

	// The ECHO pin is set HIGH when triggered. So, the timer starts here.
	} else if (is_triggered == 0){
		is_triggered = 1;
		TCCR1B |= PRESCALER_BITS; // Start timer
	}
	
	
}

ISR(TIMER2_COMPA_vect){
	i = (i > TIMER2_OVERFLOW)? 0 : i + 1;

	if(i == 0){
		TRIGGER_PORTx |= _BV(TRIGGER_PIN);

	}else if(i == TIMER2_OVERFLOW){
		TRIGGER_PORTx &= ~_BV(TRIGGER_PIN);

	}

}
