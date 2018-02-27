#include <msp430g2553.h>

#define LED1 0x01
#define LED2 0x02
#define LED3 0x04
#define LED4 0x10
#define LED5 0x20
#define BTN 0x08

//Parameters
#define SECONDS_TO_ADVANCE 3600 //3600 for one hour

volatile unsigned int i = 0;
volatile unsigned int color_input;

void main(void) {
	WDTCTL = WDTPW + WDTHOLD;

	// Timing crystal setup
	//BCSCTL1 &= ~XT2OFF; // turn on the LFXT2 osc. (watch crystal)
	//BCSCTL2 |= SELS; // Source SMCLK from crystal
	//BCSCTL3 |= XCAP_3; //Capacitance for watch crystal

	// Output setup
	P1DIR = 0x41;  //1.6 and 1.0

	// GIE
	_enable_interrupts();

	// long tick to change brightness for demo
	TA0CCR0 = 512;//a large number
	TA0CTL = TASSEL_1 + MC_1 + TACLR + TAIE;

	// Timer for PWM
	TA1CCR0 = 256;// smaller
	TA1CTL = TASSEL_1 + MC_1 + TACLR + TAIE;
	TA1CCTL1 = CCIE;
	TA1CCR1 = 0;
	TA1CCTL2 = CCIE;
	TA1CCR2 = 256;

	// Main display loop
	while (1){
		if (color_input < 256){
			TA1CCR1 = color_input;
			TA1CCR2 = 256 - color_input;
		}
		else if (color_input < 512){
			TA1CCR1 = 512 - color_input;
			TA1CCR2 = color_input - 256;
		}
		LPM0;
	}
}

// long Timer
void __attribute__ ((interrupt(TIMER0_A1_VECTOR))) delay_isr(void){
	if (TA0IV == 0x0A){
		if (color_input < 512) color_input++;
		else color_input = 0;
		LPM0_EXIT;
		return;
	}
}

// Shorter timer
void __attribute__ ((interrupt(TIMER1_A1_VECTOR))) hour_isr(void){
	int vector = TA1IV;
	if (vector == 0x0A){ // TA Rollover
		P1OUT &= ~0x41;
	}
	else if (vector == 0x02){//CCR1 match
		P1OUT |= 0x01;
	}
	else if (vector == 0x04){//CCR2 match
		P1OUT |= 0x40;
	}
	
}
