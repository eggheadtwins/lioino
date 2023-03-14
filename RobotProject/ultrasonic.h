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

// MAX Prescaler is necessary as the timer overflows even for very close obstacle.
#define TIMER2_PRESCALER 1024  

#if TIMER2_PRESCALER == 0
#	define TIMER2_PRESCALER (_BV(CS20))

#elif TIMER2_PRESCALER == 8 
#	define TIMER2_PRESCALER (_BV(CS21))

#elif TIMER2_PRESCALER == 32
#	define TIMER2_PRESCALER (_BV(CS21) | _BV(CS20))

#elif TIMER2_PRESCALER == 64 
#	define TIMER2_PRESCALER (_BV(CS22))

#elif TIMER2_PRESCALER == 128
#	define TIMER2_PRESCALER (_BV(CS22) | _BV(CS21))

#elif TIMER2_PRESCALER == 256 
#	define TIMER2_PRESCALER (_BV(CS22) | _BV(CS21))

#elif TIMER2_PRESCALER == 1024
#	define TIMER2_PRESCALER (_BV(CS22) | _BV(CS21) | _BV(CS20))
#endif

#define TIMER2_OVERFLOW 10

volatile uint16_t pulse_width = 0; // Stores the time taken to reach the receiver.  
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
	
	// Automatic mode.
	if(TIMER2_OVERFLOW != 0){
		// Set TOP value.
		OCR2A = 255;
		
		// Enable CTC interrupt.
		TIMSK2 |= _BV(OCIE2A);
		
		// Enable CTC Mode.
		TCCR2B |= _BV(WGM12);

		// Set MAX (1024) Prescaler.
		TCCR2B |= TIMER2_PRESCALER;
	}
		
}


void trigger(void){
	TRIGGER_PORTx |= _BV(TRIGGER_PIN);
	_delay_ms(TRIGGER_FREQUENCY);
	TRIGGER_PORTx &= ~_BV(TRIGGER_PIN);	
	
}



ISR(ECHO_INTx_VECTOR){
	// ECHO pin goes from HIGH to LOW, when signal received. 
	if((ECHO_PORTx & _BV(ECHO_PIN)) == 0){
		pulse_width = TCNT2;
		
		if(pulse_width == 34){
			pulse_width = 73;
		}
		
		// Start auto-triggering again. 
		TCCR2B |= _BV(WGM12);
			
	// The ECHO pin is set HIGH when triggered. So, the timer starts here.
	}else{
		// Stop auto-triggering. 
		TCCR2B &= ~_BV(WGM12);
	}
	
	// Reset ticks.
	TCNT2 = 0;
	
	
}

// Auto-triggering every ~ 0.19s. 
ISR(TIMER2_COMPA_vect){
	i = (i > TIMER2_OVERFLOW)? 0 : i + 1;

	if(i == 0){
		TRIGGER_PORTx |= _BV(TRIGGER_PIN);
		
	}else if(i == TIMER2_OVERFLOW){
		TRIGGER_PORTx &= ~_BV(TRIGGER_PIN);

	}

}
