//------------------------------------------------------------------------------
//
//  Description: This file contains the interrupts for switches
//
//
//  Calvin Schmidt
//  Feb 2016
//  Built with IAR Embedded Workbench Version: V7.3.1.3987 (6.40.1)
//------------------------------------------------------------------------------

#include  "msp430.h"
#include  "functions.h"
#include  "macros.h"

// Display Globals
extern char display_line_1[LENGTHOFDISPLAY];
extern char display_line_2[LENGTHOFDISPLAY];
extern char display_line_3[LENGTHOFDISPLAY];
extern char display_line_4[LENGTHOFDISPLAY];
extern char *display_1;
extern char *display_2;
extern char *display_3;
extern char *display_4;
extern char posL1;
extern char posL2;
extern char posL3;
extern char posL4;
// PWM Globals
extern int right_forward_rate;
extern int right_reverse_rate;
extern int left_forward_rate;
extern int left_reverse_rate;
// Serial Communications Globals
extern int usb_rx_ring_wr;
extern int usb_rx_ring_rd;
extern int usb_tx_ring_wr;
extern int usb_tx_ring_rd;
extern char USB_Char_Tx[SMALL_RING_SIZE];
extern char USB_Char_Rx[SMALL_RING_SIZE];
extern char message[SMALL_RING_SIZE];
extern char IOT_Char_Tx[SMALL_RING_SIZE];
extern char IOT_Char_Rx[SMALL_RING_SIZE];
extern int iot_tx_ring_wr;
extern int iot_rx_ring_wr;
extern int iot_tx_ring_rd;
extern int iot_rx_ring_rd;
//
extern char ip_display[14];
extern char iot_ip[IOT_LENGTH];
extern char ip_display[14];
//------------------------------------------------------------------------------
//  Description: Port 4 interrupt. Switch 1 Press begins movement for Project 4.
//               
//
//  Calvin Schmidt
//  Feb 2016
//  Built with IAR Embedded Workbench Version: V7.3.1.3987 (6.40.1) 
//------------------------------------------------------------------------------

//Include #pragma vector = [Assigned Vector]
#pragma vector=PORT4_VECTOR
//Create Interrupt Service Routine Function with “__interrupt”
__interrupt void switch_interrupt(void) {
// Switch 1
  if (P4IFG & SW1) { 
    P4IFG &= ~SW1;
    
  }
// Switch 2
 if (P4IFG & SW2) {
  P4IFG &= ~SW2; // Clear any current timer interrupt.
  
  display_1 = "";
  display_2 = "";
  Display_Process();
 }
}
//------------------------------------------------------------------------------