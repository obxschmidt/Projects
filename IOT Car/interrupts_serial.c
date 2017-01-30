//------------------------------------------------------------------------------
//
//  Description: This file contains the code related to serial communications interrupts
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
extern int buff;
extern int com_ind;
extern int catch;
extern int check_com;
extern char iot_command[IOT_LENGTH];
extern char iot_ip[IOT_LENGTH];
int ip_counter = CLEAR;
int ip_flag = CLEAR;
extern int iot_flag;
extern char car_command[100];
extern int iot_reset_flag;
// Display Globals
extern char *display_1;
extern char *display_2;
extern char *display_3;
extern char *display_4;
int car_counter = 0;
int start_command = 0;

//------------------------------------------------------------------------------
#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void){
  unsigned int temp = CLEAR;
  switch(__even_in_range(UCA0IV,0x08)){
  case 0: // Vector 0 - no interrupt
    break;
  case 2: // Vector 2 - RXIFG
    // Circular buffer back to beginning
    UCA0TXBUF = UCA0RXBUF;
    temp = usb_rx_ring_wr;
    USB_Char_Rx[temp] = UCA0RXBUF; // RX -> USB_Char_Rx character
    if(USB_Char_Rx[temp] == '/'){
      catch = CLEAR;
      com_ind = CLEAR;
      check_com = TRUE; 
    }
    if(catch){
      iot_command[com_ind] = USB_Char_Rx[temp];
      com_ind++;
    }
    if(USB_Char_Rx[temp] == '.'){
      catch=TRUE;
    }
    if (++usb_rx_ring_wr >= (SMALL_RING_SIZE)){
      usb_rx_ring_wr = BEGINNING; // Circular buffer back to beginning
    }
    break;
  case 4: // Vector 4 – TXIFG
    break;
  default: break;
  }
}
//------------------------------------------------------------------------------

// Transmitting
//------------------------------------------------------------------------------
#pragma vector=USCI_A1_VECTOR
__interrupt void USCI_A1_ISR(void){
  unsigned int temp = CLEAR;
  // unsigned int temp;
  switch(__even_in_range(UCA1IV,0x08)){
    case 0: // Vector 0 - no interrupt
    break;
    case 2: // Vector 2 - RXIFG
      UCA0TXBUF = UCA1RXBUF;
      
      temp = iot_rx_ring_wr;
      IOT_Char_Rx[temp] = UCA0TXBUF; // RX -> USB_Char_Rx character
      
      if (IOT_Char_Rx[temp] == '*') {
        iot_flag = TRUE;
        car_counter = CLEAR;
      }
      
      if (IOT_Char_Rx[temp] == 'p' && IOT_Char_Rx[temp - ONE] == 'i') {
        ip_flag = TRUE;
      }
      
      if (IOT_Char_Rx[temp] == 'a' && IOT_Char_Rx[temp - ONE] == 's' && IOT_Char_Rx[temp - 2] == 'i' && IOT_Char_Rx[temp - 3] == 'D')  {
        iot_reset_flag = TRUE;
      }
      
      if (ip_flag && (IOT_Char_Rx[temp] != '\n')) { // if it's not equal to newline character
        iot_ip[ip_counter] = IOT_Char_Rx[temp];
        ip_counter++;
      }
      
      if (iot_flag) {
        car_command[car_counter] = IOT_Char_Rx[temp];
        car_counter++;
        if(IOT_Char_Rx[temp] == '\r') {
          start_command = TRUE;
        }
      }
      
      if (++iot_rx_ring_wr >= (SMALL_RING_SIZE)){
        iot_rx_ring_wr = BEGINNING; // Circular buffer back to beginning 
      }
    break;
    case 4: // Vector 4 – TXIFG
      temp = iot_tx_ring_wr;
      UCA1TXBUF = USB_Char_Tx[temp];
      if (++iot_tx_ring_wr >= SMALL_RING_SIZE) {
        iot_tx_ring_wr = BEGINNING;
      }
    break;
    default: break;
  }
}
//------------------------------------------------------------------------------
 