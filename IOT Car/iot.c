//------------------------------------------------------------------------------
//
//  Description: This file contains the code related to the IOT device
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
extern int iot_length;
extern char iot_command[IOT_LENGTH];
extern char car_command[HUNDRED];

void Write_USB(char character){
  while(!(UCA0IFG & UCTXIFG));
  UCA0TXBUF = character;
}

void Write_IOT(char character){
  while(!(UCA1IFG & UCTXIFG));
  UCA1IE |= UCTXIE;
  UCA1TXBUF = character;
}

void Read_USB(void){
  while (usb_rx_ring_rd != usb_rx_ring_wr){
    Write_IOT(USB_Char_Rx[usb_rx_ring_rd]);
    usb_rx_ring_rd++;
    if (usb_rx_ring_rd >= (SMALL_RING_SIZE)){
      usb_rx_ring_rd = BEGINNING;
    }
  } 
}
void Read_IOT(void){
  while (iot_rx_ring_rd != iot_rx_ring_wr){
    Write_USB(IOT_Char_Rx[iot_rx_ring_rd]);
    iot_rx_ring_rd++;
    if (iot_rx_ring_rd >= (SMALL_RING_SIZE)){
      iot_rx_ring_rd = BEGINNING;
    }
  }
}

void check_command(void) {
  if(iot_command[CLEAR] == 'A'){
    send_IOT(Command_length());
    Clear_command();
  }
}

void send_IOT(int length){
  for(int i=CLEAR; i<length;i++){
    UCA1TXBUF = iot_command[i];
    five_msec_delay(QUARTERSECOND);
  }
}

void Clear_command(void){
  for(int i=CLEAR; i<IOT_LENGTH; i++){
    iot_command[i] = SLASH;
  }
}

int Command_length(void){
  iot_length=CLEAR;
  while(iot_command[iot_length] != (SLASH)){
    iot_length++;
  }
  return (IOT_LENGTH);
}

void Clear_Car_Command(void) {
  for (int i = CLEAR; i < 99; i++) {
    car_command[i] = 0x00;
  }
}

void IOT_Reset(void) {
  PJOUT &= ~IOT_RESET;
  five_msec_delay(200);
  PJOUT |= IOT_RESET;
}





