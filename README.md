# pc-case-lights
PC Case light controller firmware for msp430

This is a small firmware for the MSP430 to run as an RGB LED controller. The end goal is to have 8 bit color output to an RGB LED driver circuit and create effects based on, among other things, internal case temperature. 

# Prerequisites
Development was performed on an Arch Linux system with AUR packages `mspgcc-ti` and `mspdebug` installed, and the Makefile expects resources from the same to be in the default location. This can easily be modified per distribution if needed.
