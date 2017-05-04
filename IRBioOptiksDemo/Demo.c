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
DebugBoard fSMCLK: 585000Hz
DebugBoard BlinkRate: 120.63Hz
DebugBoard BlinkSpeed: 0.0041452s
DebugBoard Cycle Time: 60.33 sec
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
static const int LEDCounter = 4875;         //Set equal to (Divider*ClockSpeed)/BlinksPerSec)), ((1*585000Hz)/120)=4875
static const double MaxCycles = 14475;      //Set equal to ((TreatmentTime*2)/BlinkTime), ((30s*2)/.0041452s)=14475
int CycleCounter = 0;                       //Declare CycleCounter variable,
int TreatmentState = 0x01;                  //Declare treatment state variable


//Method: main
//Description: This method contains the main code, set up the clocks and interrupt.
//Args: void
//Returns: void
void main(void) {                           //Start of Main
  volatile unsigned int count;              //Counter is an integer. It is volatile to trick the compiler into not optimizing the empty loop
  WDTCTL = WDTPW + WDTHOLD;                 //Stop hardware Watchdog Timer
  P1DIR |= BIT0 + BIT4 + BIT5;              //Set GPIO P1.0, P1.4, & P1.5 to "output" direction
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
    if (TreatmentState == 0x01) {               //If TreatmentState is true
        P1OUT ^= BIT0 + BIT4 + BIT5;            //then Toggle P1.0, P1.4 & P1.5 Debug & Implant LEDs
    }
    CycleCounter++;                             //Increment CycleCounterA
    if (CycleCounter == MaxCycles) {            //Check if CycleCounter is greater that MaxCycles
        P1OUT &= 0x00;                          //Turn off all Outputs including Debug & Implant LEDs
        CycleCounter = 0;                       //Reset CycleCounter
        TreatmentState ^= 0x01;                 //Toggle TreatmentState
    }                                           //End CycleCounter Loop
}                                               //End of TimerA interrupt


