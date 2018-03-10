#include <msp430g2553.h>

#define red_led 0x02 //P2.1 (pin 9)
#define blue_led 0x40 //P1.6 (pin 14)
#define green_led 0x10 //P2.4 (pin 12)

#define red_out TA1CCR1
#define blue_out TA0CCR1
#define green_out TA1CCR2

#define timer_params TASSEL_2 + MC_1 + TACLR
#define pwm_period 256

volatile unsigned int i = 0;
volatile unsigned int color_input;

void main(void) {
	WDTCTL = WDTPW + WDTHOLD;

	// Timing crystal setup
	//BCSCTL1 &= ~XT2OFF; // turn on the LFXT2 osc. (watch crystal)
	//BCSCTL2 |= SELS; // Source SMCLK from crystal
	//BCSCTL3 |= XCAP_3; //Capacitance for watch crystal

	// P1 Setup
	P1DIR |= blue_led;
	P1OUT |= blue_led;
	P1SEL |= blue_led; // TimerA0.1
		
	// P2 Setup
	P2DIR |= red_led | green_led;
	P2OUT |= red_led | green_led;
	P2SEL |= red_led | green_led; // TimerA1.1-2

	// GIE
	_enable_interrupts();

	// long tick to change brightness for demo
	//TA0CCR0 = 65535;//a large number
	//TA0CTL = TASSEL_2 + MC_2 + TACLR + TAIE;

	// Timer config
	TA0CTL = TA1CTL = TAIE + timer_params; //TAIE is included for debugging or timing
	TA0CCTL1 = TA1CCTL1 = TA1CCTL2 = OUTMOD_7;
	TA0CCR0 = TA1CCR0 = pwm_period;

	// Main display loop
	while (1){
		for (blue_out = 0; blue_out < pwm_period; blue_out++){
		for (int i = 0; i < 16; i++){
			LPM0;
		}
		}
		blue_out = 0;
		for (red_out = 0; red_out < pwm_period; red_out++){
		for (int i = 0; i < 16; i++){
			LPM0;
		}
		}
		red_out = 0;
		for (green_out = 0; green_out < pwm_period; green_out++){
		for (int i = 0; i < 16; i++){
			LPM0;
		}
		}
		green_out = 0;
	}
}

// Shorter timer
void __attribute__ ((interrupt(TIMER1_A0_VECTOR))) T1A0_isr(void){
	return;
}
void __attribute__ ((interrupt(TIMER0_A1_VECTOR))) T0A1_isr(void){
	switch (TA0IV){
		case 0x02: //CCR1 match
		break;
	
		case 0x04: //CCR2 match
		break;

		case 0x0A: //TA rollover
		break;

		default:
		break;
	}
	return;
}

void __attribute__ ((interrupt(TIMER1_A1_VECTOR))) T1A1_isr(void){
	switch (TA1IV){
		case 0x02: //CCR1 match
		break;
	
		case 0x04: //CCR2 match
		break;

		case 0x0A: //TA rollover
		LPM0_EXIT;
		break;

		default:
		break;
	}
	return;
}
