/*
Author: Jay Battle
Company: Team B41: IRBioOptiks
Project: Senior Design
Date Created: March 26, 2017
Name: main.c
Program Description: This program is a demo that blinks the leds at the desired speed
References: M.Buccini, L. Westlund, R. Scheidt PhD, Texas Instruments Inc, Y. Wang, E. Garante
Hardware: MSP430F2013IRSAT
*/

/*
Important Info:
Warning! Do not plug or unplug the device during flashing!
Warning! Do not plug or unplug the device drom the debugging tool while powered!
fACLK = 32768 Hz, the speed of ACLK during active mode at 1 MHz of current
fMCLK = 1 MHz, the speed of MCLK during active mode at 1 MHz of current
fSMCLK = 1 MHz, the speed of SMCLK during active mode at 1 MHz of current
fDCO = 1 MHz, the speed of DCO during active mode at 1 MHz of current
BlinkRate = ClockSpeed/(Divider*LEDCounter)
*/

/*
Measurements:
Measured clocks:
VCSELBoard fSMCLK: 509420Hz
VCSELBoard BlinkRate: 119.99Hz
VCSELBoard BlinkSpeed: 0.0041672s
VCSELBoard Cycle Time: ? sec
*/

/*
Ports:
P1.0 Debug LED
P1.4 Implant LED
P1.5 Implant LED PWM
*/

//Declare External File References:
#include  <msp430f2013.h>                   //Includes the preset msp430f2013 variables

//Declare Global Variables:
static const int LEDCounter = 4245;         //Set equal to (Divider*ClockSpeed)/BlinksPerSec)), ((1*509420Hz)/120)=4245
static const double MaxCycles = 14398;      //Set equal to ((TreatmentTime*2)/BlinkTime), ((30s*2)/.0041672s)=14398
int Minutes = 0;                            //Declare Minute Count variable, counts to Total Treatment Time (10 minutes)
int DebugLedCounter = 0;                           //Declare CounterA variable,
int CycleCounter = 0;                       //Declare CycleCounter variable,

//Method: main
//Description: This method contains the main code, set up the clocks and interrupt.
//Args: void
//Returns: void
void main(void) {                           //Start of Main
  volatile unsigned int count;              //Counter is an integer. It is volatile to trick the compiler into not optimizing the empty loop
  WDTCTL = WDTPW + WDTHOLD;                 //Stop hardware Watchdog Timer
  P1DIR |= BIT0 + BIT4;                     //Set GPIO P1.0, P1.4, & P1.5 to "output" direction
  P1OUT |= BIT4;                            //Start P1.4 Implant LEDs
  CCTL0 = CCIE;                             //CCR0 Interrupt Enabled
  CCR0 = LEDCounter;                        //Set CCR0 Register to starting value
  TACTL = TASSEL_2 + MC_1 + ID_0;           //Set SMCLK to Up mode with a divider of 0
  _EINT();                                  //Enable Global Interrupts
  LPM0;                                     //Enable Low Power
  for (;;) {}                               //Infinite Loop
}                                           //End of Main

//Method: main
//Description: Timer_A TACCR0 interrupt vector handler
//Args: void
//Returns: void
#pragma vector=TIMERA0_VECTOR
__interrupt void TimerAInterrupt(void){
    DebugLedCounter++;                        //Increment DebugLedCounter
    if (DebugLedCounter == 250) {             //If TreatmentState is true
        P1OUT ^= BIT0;                        //then Toggle P1.0 Debug LED
        DebugLedCounter =0;                   //reset DebugLedCounter
    }
    CycleCounter++;                           //Increment CycleCounterA
    if (CycleCounter == MaxCycles) {          //Check if CycleCounter is greater that MaxCycles
        CycleCounter = 0;                     //Reset CycleCounter
        Minutes++;
        if (Minutes == 10) {
            P1OUT &= 0x00;                    //Turn off all Outputs including Debug & Implant LEDs
            CCTL0 &= ~CCIE;                   //End CCR0 Interrupt Enabled
        }
    }                                         //End CycleCounter Loop
}                                             //End of TimerA interrupt


