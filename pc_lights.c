#include <msp430g2553.h>

#define red_led 0x01
#define green_led 0x40

volatile unsigned int i = 0;
volatile unsigned int color_input;
struct rgb {
	unsigned int red;
	unsigned int green;
	unsigned int blue;
};

void main(void) {
	WDTCTL = WDTPW + WDTHOLD;

	// Timing crystal setup
	//BCSCTL1 &= ~XT2OFF; // turn on the LFXT2 osc. (watch crystal)
	//BCSCTL2 |= SELS; // Source SMCLK from crystal
	//BCSCTL3 |= XCAP_3; //Capacitance for watch crystal

	// Output setup
	P1DIR = red_led + green_led;
	P1OUT = 0;

	// GIE
	_enable_interrupts();

	// long tick to change brightness for demo
	//TA0CCR0 = 65535;//a large number
	TA0CTL = TASSEL_2 + MC_2 + TACLR + TAIE;

	// Timer for PWM
	TA1CCR0 = 256;
	TA1CCTL0 = CCIE; // Use this interrupt for priority reasons
	TA1CTL = TASSEL_2 + MC_1 + TACLR + ID_3;
	TA1CCTL1 = CCIE;
	TA1CCR1 = 0;
	TA1CCTL2 = CCIE;
	TA1CCR2 = 256;

	TA1CCR1 = 0;
	TA1CCR2 = (256 - color_input);
	// Main display loop
	while (1){
//		if (color_input < 256){
///			TA1CCR1 = color_input;
//			TA1CCR2 = 256 - color_input;
//		}
//		else if (color_input < 512){
//			TA1CCR1 = 512 - color_input;
//			TA1CCR2 = color_input - 256;
//		}
		LPM0;
	}
}

// long Timer
void __attribute__ ((interrupt(TIMER0_A1_VECTOR))) inc_isr(void){
	if (TA0IV == 0x0A){
		LPM0_EXIT;
		return;
	}
}

// Shorter timer
void __attribute__ ((interrupt(TIMER1_A0_VECTOR))) pwm1_isr(void){
	P1OUT |= red_led;
	return;
}
void __attribute__ ((interrupt(TIMER1_A1_VECTOR))) pwm2_isr(void){
	switch (TA1IV){
		case 0x02: //CCR1 match
		P1OUT &= ~red_led;
		break;
	
		case 0x04: //CCR2 match
		//P1OUT &= ~green_led;
		break;

		case 0x0A: //TA rollover, here just in case
		break;

		default:
		break;
	}
	return;
}

