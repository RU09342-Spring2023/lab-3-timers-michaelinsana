/*
 * Part2.c
 *
 *  Created on: Feb 11, 2023
 *      Author: Russell Trafford
 *
 *      This code will need to change the speed of an LED between 3 different speeds by pressing a button.
 */

#include <msp430.h>

void gpioInit();
void timerInit();
int i = 0;

void main(){

    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer

    gpioInit();
    timerInit();

    // Disable the GPIO power-on default high-impedance mode
    // to activate previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;

    __bis_SR_register(LPM3_bits | GIE);

}


void gpioInit(){
      // Configure RED LED on P1.0 as Output
      P1OUT &= ~BIT0;                         // Clear P1.0 output latch for a defined power-on state
      P1DIR |= BIT0;                          // Set P1.0 to output direction

      // Configure Green LED on P6.6 as Output
      P6OUT &= ~BIT6;                         // Clear P6.6 output latch for a defined power-on state
      P6DIR |= BIT6;                          // Set P6.6 to output direction


      // Configure Button on P2.3 as input with pullup resistor
      P2OUT |= BIT3;                          // Configure P2.3 as pulled-up
      P2REN |= BIT3;                          // P2.3 pull-up register enable
      P2IES |= BIT3;                         // P2.3 High --> Low edge
      P2IE |= BIT3;                           // P2.3 interrupt enabled

}

void timerInit(){
    // @TODO Initialize Timer B1 in Continuous Mode using ACLK as the source CLK with Interrupts turned on

    TB1CCTL0 = CCIE;                          // TBCCR0 interrupt enabled
    TB1CCR0 = 50000;
    TB1CTL = TBSSEL_1 | MC_1;                 // ACLK, continuous mode

}


/*
 * INTERRUPT ROUTINES
 */

// Port 2 interrupt service routine
#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{
    P2IFG &= ~BIT3;                         // Clear interrupt flag

    // @TODO When the button is pressed, you can change what the CCR0 Register is for the Timer. You will need to track what speed you should be flashing at.

    switch(i) {

    case 0:
        TB1CCR0 = 50000;            //sets timer count value to 50000
        break;

    case 1:
        TB1CCR0 = 30000;            //sets timer count value to 30000
        break;

    case 2:
        TB1CCR0 = 10000;            //sets timer count value to 10000
        break;
    }

    if(i < 2)                       //if the mode isn't at 3, go to the next mode, if it is at 3, go to 1
        i++;
    else
        i = 0;

}


// Timer B1 interrupt service routine
#pragma vector = TIMER1_B0_VECTOR
__interrupt void Timer1_B0_ISR(void)
{
    P1OUT ^= BIT0;          //toggle the LED

}


