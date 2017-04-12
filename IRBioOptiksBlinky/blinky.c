//******************************************************************************
//  MSP430F20xx Demo - Software Toggle P1.0
//  Description; Toggle P1.0 by xor'ing P1.0 inside of a software loop.
//  ACLK = n/a, MCLK = SMCLK = default DCO
//
//                MSP430F20xx
//             -----------------
//         /|\|              XIN|-
//          | |                 |
//          --|RST          XOUT|-
//            |                 |
//            |             P1.0|-->Debug LED
//
//  M.Buccini / L. Westlund
//  Texas Instruments, Inc
//  October 2005
//  Built with CCE Version: 3.2.0 and IAR Embedded Workbench Version: 3.40A
// Modified:
// 2015.01.12: R. Scheidt PhD; Added more comments
// 2014.01.16: R. Scheidt PhD; Added comments
// 2017.03.26: J. Battle Bs; Cleaned Code, Cleaned Comments for MSP430F2013IRSAT
//******************************************************************************

#include  <msp430f2013.h>           //Includes the preset msp430f2013 variables

void main(void) {                   //Start of Main
  volatile unsigned int counter;	// counter is an integer. It is volatile to trick the compiler into not optimizing the empty loop
						            // A volatile variable is one that can change due to hardware action, so compiler optimizations should leave it alone.
  WDTCTL = WDTPW + WDTHOLD;         // Stop hardware watchdog timer
  P1DIR |= 0x01;                    // Set GPIO P1.0 to "output" direction
	
  for (;;) {                        //Infinite loop
    P1OUT ^= 0x01;                  // Toggle P1.0 debug Led using exclusive-OR
    counter = 50000;                // Set the counter
    do (counter--);					// Decrement the counter, Empty loop
    while (counter != 0);			// Until counter equals zero
  }									// End For Loop

}                                   //End of Main


