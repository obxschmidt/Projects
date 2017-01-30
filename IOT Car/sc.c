//------------------------------------------------------------------------------
//
//  Description: This file contains the code related to serial communications
//
//
//  Calvin Schmidt
//  Feb 2016
//  Built with IAR Embedded Workbench Version: V7.3.1.3987 (6.40.1)
//------------------------------------------------------------------------------

#include  "msp430.h"
#include  "functions.h"
#include  "macros.h"

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
// Dispaly Globals
extern char *display_1;
extern char *display_2;
extern char *display_3;
extern char *display_4;

// #defines
//  BEGINNING (0)
//  SMALL_RING_SIZE (9)
//----------------------------------------------------------------------------
void Init_Serial_UCA0(int baud_rate){
  
  int i;
  for(i=0; i<SMALL_RING_SIZE; i++){
    USB_Char_Rx[i] = 0x00; // USB Rx Buffer
  }
  usb_rx_ring_wr = BEGINNING;
  usb_rx_ring_rd = BEGINNING;
  
  for(i=0; i<LARGE_RING_SIZE; i++){ // May not use this
    USB_Char_Tx[i] = 0x00; // USB Tx Buffer
  }
  usb_tx_ring_wr = BEGINNING;
  usb_tx_ring_rd = BEGINNING;
  // Configure UART 0
  UCA0CTLW0 = 0; // Use word register
  UCA0CTLW0 |= UCSSEL__SMCLK; // Set SMCLK as fBRCLK
  UCA0CTLW0 |= UCSWRST; // Set Software reset enable
  UCA0BRW |= baud_rate; // 
  
  if (baud_rate == 52) {
    UCA0MCTLW = 0x4911;
  }
  if (baud_rate == 4) {
    UCA0MCTLW = 0x5551;
  }
// UCA0MCTLW = UCSx concatenate UCFx concatenate UCOS16;
// UCA0MCTLW = 0x49 concatenate 1 concatenate 1;
 UCA0CTL1 &= ~UCSWRST; // Release from reset
 UCA0IE |= UCRXIE; // Enable RX interrupt
}
 
void Init_Serial_UCA1(int baud_rate){
  
  int i;
  for(i=0; i<SMALL_RING_SIZE; i++){
    USB_Char_Rx[i] = 0x00; // USB Rx Buffer
  }
  iot_rx_ring_wr = BEGINNING;
  iot_rx_ring_rd = BEGINNING;
  
  for(i=0; i<LARGE_RING_SIZE; i++){ // May not use this
    USB_Char_Tx[i] = 0x00; // USB Tx Buffer
  }
  iot_tx_ring_wr = BEGINNING;
  iot_tx_ring_rd = BEGINNING;
  // Configure UART 0
  UCA1CTLW0 = 0; // Use word register
  UCA1CTLW0 |= UCSSEL__SMCLK; // Set SMCLK as fBRCLK
  UCA1CTLW0 |= UCSWRST; // Set Software reset enable
  UCA1BRW |= baud_rate; // 
  
  if (baud_rate == 52) {
    UCA1MCTLW = 0x4911;
  }
  if (baud_rate == 4) {
    UCA1MCTLW = 0x5551;
  }
// UCA0MCTLW = UCSx concatenate UCFx concatenate UCOS16;
// UCA0MCTLW = 0x49 concatenate 1 concatenate 1;
 UCA1CTL1 &= ~UCSWRST; // Release from reset
 UCA1IE |= UCRXIE; // Enable RX interrupt
}


 
 
 