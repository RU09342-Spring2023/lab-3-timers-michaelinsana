/*
 * Part3.c
 *
 *  Created on: Feb 12, 2023
 *      Author: Michael Insana
 *
 *
 */

#include <msp430.h>

int i = 0;
int button_press = 0;
void gpioInit();
void timerInit();


void main(){

    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer

    gpioInit();
    timerInit();

    PM5CTL0 &= ~LOCKLPM5;

    //__bis_SR_register(LPM3_bits | GIE);     //low power mode and interrupts
    __bis_SR_register(GIE);     //low power mode and interrupts


    while(1) {
        if(button_press) {   //count up while button pressed
            i++;

        }

    }
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

    TB1CCTL0 = CCIE;                          // TBCCR0 interrupt enabled
    TB1CCR0 = 10000;
    TB1CTL = TBSSEL_1 | MC_1 | ID_3;                 // ACLK, up mode, /8

}




// Port 2 interrupt service routine
#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{
    P2IFG &= ~BIT3;                         // Clear P2.3 IFG


    if (P2IES & BIT3)      //looks for rising edge
    {
        button_press = 0;       //switches edge search
        P2IES &= ~BIT3;
        P6OUT |= BIT6;          //turns on green LED
        P1OUT &= ~BIT0;         //turns off red LED
        i = 0;                  //resets counter variable

    }

    else if (!(P2IES & BIT3))   //looks for falling edge
    {
        button_press = 1;
        P2IES |= BIT3;          //switches edge search
        TB1CCR0 = i;            //sets timer to value of i
        P6OUT &= ~BIT6;         //turns green LED off

    }

   /* if (P2IES & BIT3)                      //looks for rising edge
        {
            P2IES &= ~BIT3;

            TB1CCTL1 &= CCIS0;              //triggers capture register
            start = TB1CCR1;                //records value of capture register
           // P1OUT |= BIT0;                   //toggle red led


        }

        else if (!(P2IES & BIT3))           //looks for falling edge
        {
            P2IES &= ~BIT3;


            TB1CCTL1 &= CCIS0;              //triggers capture register
            end = TB1CCR1;                  //records value of capture register

            time = end - start;
            TB1CCR0 = time;                 //set timer equal to total time pressed

        }

    */

}


// Timer B1 interrupt service routine
#pragma vector = TIMER1_B0_VECTOR
__interrupt void Timer1_B0_ISR(void)
{

    P1OUT ^= BIT0;  //toggle red led

}








