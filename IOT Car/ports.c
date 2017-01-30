//------------------------------------------------------------------------------
//
//  Description: This file contains the initialization for all port pins
//
//
//  Calvin Schmidt
//  Feb 2016
//  Built with IAR Embedded Workbench Version: V7.3.1.3987 (6.40.1)
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

#include  "msp430.h"
#include  "functions.h"
#include  "macros.h"

void Init_Ports(void){
//------------------------------------------------------------------------------
//
// Description: This file contains the function to call all port initialization functions
//
// Passed: No variables passed
// Locals: No local variables
// Returned: No values returned
//  
//
//  Calvin Schmidt
//  Feb 2016
//  Built with IAR Embedded Workbench Version: V7.3.1.3987 (6.40.1)
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
  Init_Port1();
  Init_Port2();
  Init_Port3();
  Init_Port4();
  Init_PortJ();
}

void Init_Port1(void){
//------------------------------------------------------------------------------
//Configure Port 1
// SMCLK_OUT (0x01) // SMCLK Out signal
// V_DETECT_R (0x01) //
// V_DETECT_L (0x02) //
// IR_LED (0x04) //
// V_THUMB (0x08) //
// SPI_CS_LCD (0x10) // LCD Chip Select
// RESET_LCD (0x20) // LCD Reset
// SIMO_B (0x40) // SPI mode - slave in/master out of USCI_B0
// SOMI_B (0x80) // SPI mode - slave out/master in of USCI_B0
//  
// Passed: No variables passed
// Locals: No local variables
// Returned: No values returned
//   
//  Calvin Schmidt
//  Feb 2016
//  Built with IAR Embedded Workbench Version: V7.3.1.3987 (6.40.1)
//------------------------------------------------------------------------------
 P1SEL0 = CLEAR; // P1 set as I/0
 P1SEL1 = CLEAR; // P1 set as I/0
 P1DIR = CLEAR; // Set P1 direction to input
 // Port 1.0
 P1SEL0 |= V_DETECT_R; // V_DETECT_R selected
 P1SEL1 |= V_DETECT_R; // V_DETECT_R selected
 // Port 1.1
 P1SEL0 |= V_DETECT_L; // V_DETECT_L selected
 P1SEL1 |= V_DETECT_L; // V_DETECT_L selected
 // Port 1.2
 P1SEL0 &= ~IR_LED; // IR_LED GPI/O selected
 P1SEL1 &= ~IR_LED; // IR_LED GPI/O selected
 P1OUT |= IR_LED; // P1 IR_LED Port Pin set low
 P1DIR |= IR_LED; // Set P1 IR_LED direction to output
 // Port 1.3
 P1SEL0 |= V_THUMB; // V_THUMB selected
 P1SEL1 |= V_THUMB; // V_THUMB selected
 // Port 1.4
 P1SEL0 &= ~SPI_CS_LCD; // SPI_CS_LCD GPI/O selected
 P1SEL1 &= ~SPI_CS_LCD; // SPI_CS_LCD GPI/O selected
 P1OUT |= SPI_CS_LCD; // P1 SPI_CS_LCD Port Pin set high
 P1DIR |= SPI_CS_LCD; // Set SPI_CS_LCD output direction
 // Port 1.5
 P1SEL0 &= ~RESET_LCD; // RESET_LCD GPI/O selected
 P1SEL1 &= ~RESET_LCD; // RESET_LCD GPI/O selected
 P1OUT &= ~RESET_LCD; // RESET_LCD Port Pin set low
 P1DIR |= RESET_LCD; // Set RESET_LCD output direction
 // Port 1.6
 P1SEL0 &= ~SIMO_B; // SIMO_B selected
 P1SEL1 |= SIMO_B; // SIMO_B selected
 P1DIR |= SIMO_B; // SIMO_B set to Output
 // Port 1.7
 P1SEL0 &= ~SOMI_B; // SOMI_B is used on the LCD
 P1SEL1 |= SOMI_B; // SOMI_B is used on the LCD
 P1DIR &= ~SOMI_B; // SOMI_B set to Input
 P1REN |= SOMI_B; // Enable pullup resistor
//--------------------------------------------------------------
} 
 
void Init_Port2(void){
//------------------------------------------------------------------------------
//Configure Port 2
// USB_TXD (0x01) // PIN 0
// USB_RXD (0x02) // PIN 1
// SPI_SCK (0x04) // PIN 2
// CPU_TXD (0x20) // PIN 5
// CPU_RXD (0x40) // PIN 6
//
// Passed: No variables passed
// Locals: No local variables
// Returned: No values returned
//  
//  Calvin Schmidt
//  Feb 2016
//  Built with IAR Embedded Workbench Version: V7.3.1.3987 (6.40.1)  
//------------------------------------------------------------------------------
 P2SEL0 = CLEAR; // P1 set as I/0
 P2SEL1 = CLEAR; // P1 set as I/0
 P2DIR = CLEAR; // Set P1 direction to input
 // Port 2.0
 P2SEL1 |= USB_TXD; // USB_TXD selected
 P2SEL0 &= ~USB_TXD; // USB_TXD selected
 // Port 2.1
 P2SEL1 |= USB_RXD; // USB_RXD selected
 P2SEL0 &= ~USB_RXD; // USB_RXD selected
 // Port 2.2
 P2SEL1 |= SPI_SCK; // IR_LED GPI/O selected
 P2SEL0 &= ~SPI_SCK; // IR_LED GPI/O selected
 P2OUT  |= SPI_SCK; // SPI_SCK Port pin set high
 // Port 2.5
 P2SEL1 |= CPU_TXD; // SPI_CS_LCD GPI/O selected
 P2SEL0 &= ~CPU_TXD; // SPI_CS_LCD GPI/O selected
 // Port 2.6
 P2SEL1 |= CPU_RXD; // P1 SPI_CS_LCD Port Pin set high
 P2SEL0 &= ~CPU_RXD; // Set SPI_CS_LCD output direction
//--------------------------------------------------------------
}

void Init_Port3(void){
//------------------------------------------------------------------------------
//Configure Port 3
// EXCEL_X (0x01) // PIN 0
// EXCEL_Y (0x02) // PIN 1
// EXCEL_Z (0x04) // PIN 2
// LCD_BACKLITE (0x08) // PIN 3
// R_FORWARD (0x10) // PIN 4
// R_REVERSE (0x20) // PIN 5
// L_FORWARD (0x40) // PIN 6
// L_REVERSE (0x80) // PIN 7 
//
// Passed: No variables passed
// Locals: No local variables
// Returned: No values returned
//  
//  Calvin Schmidt
//  Feb 2016
//  Built with IAR Embedded Workbench Version: V7.3.1.3987 (6.40.1)  
//------------------------------------------------------------------------------
 P3SEL1 = CLEAR; // P1 set as I/0
 P3SEL0 = CLEAR; // P1 set as I/0
 P3DIR = CLEAR; // Set P1 direction to input
 // Port 3.0
 P3SEL1 &= ~EXCEL_X; // EXCEL_X selected
 P3SEL0 &= ~EXCEL_X; // EXCEL_X selected
 P3DIR  &= ~EXCEL_X; // Set EXCEL_X set direction to Input
 P3OUT  &= ~EXCEL_X; // P3 EXCEL_X Port Pin set low
 P3REN  &= ~EXCEL_X; // pullup resistor
 // Port 3.1
 P3SEL1 &= ~EXCEL_Y; // EXCEL_Y selected
 P3SEL0 &= ~EXCEL_Y; // EXCEL_Y selected
 P3DIR  &= ~EXCEL_Y; // Set EXCEL_Y set direction to Input
 P3OUT  &= ~EXCEL_Y; // P3 EXCEL_X Port Pin set low
 P3REN  &= ~EXCEL_Y; // pullup resistor
 // Port 3.2
 P3SEL1 &= ~EXCEL_Z; // EXCEL_Z selected
 P3SEL0 &= ~EXCEL_Z; // EXCEL_Z selected
 P3DIR  &= ~EXCEL_Z; // Set EXCEL_Z set direction to Input
 P3OUT  &= ~EXCEL_Z; // P3 EXCEL_X Port Pin set low
 P3REN  &= ~EXCEL_Z; // pullup resistor
 // Port 3.3
 P3SEL1 &= ~LCD_BACKLITE; // LCD_BACKLITE selected
 P3SEL0 &= ~LCD_BACKLITE; // LCD_BACKLITE selected
 P3DIR  |= LCD_BACKLITE; // Set LCD_BACKLITE direction to output
 P3OUT  &= ~LCD_BACKLITE; // P3 LCD_BACKLITE Port Pin set low
 // Port 3.4
 P3SEL1 &= ~R_FORWARD; // R_FORWARD selected
 P3SEL0 |= R_FORWARD; // R_FORWARD selected
 P3DIR  |= R_FORWARD; // Set R_FORWARD direction to Output
 P3OUT  &= ~R_FORWARD; //P3 R_FORWARD Port Pin set low
 // Port 3.5
 P3SEL1 &= ~R_REVERSE; // R_REVERSE selected
 P3SEL0 |= R_REVERSE; // R_REVERSE selected
 P3DIR  |= R_REVERSE; // Set R_REVERSE direction to Output
 P3OUT  &= ~R_REVERSE; //P3 R_REVERSE Port Pin set low
 // Port 3.6
 P3SEL1 &= ~L_FORWARD; // L_FORWARD selected
 P3SEL0 |= L_FORWARD; // L_FORWARD selected
 P3DIR  |= L_FORWARD; // Set L_FORWARD direction to Output
 P3OUT  &= ~L_FORWARD; //P3 L_FORWARD Port Pin set low
 // Port 3.7
 P3SEL1 &= ~L_REVERSE; // L_REVERSE selected
 P3SEL0 |= L_REVERSE; // L_REVERSE selected
 P3DIR  |= L_REVERSE; // Set L_REVERSE direction to Output
 P3OUT  &= ~L_REVERSE; //P3 L_REVERSE Port Pin set low
//--------------------------------------------------------------
}

 void Init_Port4(void){
//------------------------------------------------------------------------------
// Configure Port 4 Pins
// SW1  (0x01) // Switch 1
// SW2  (0x02) // Switch 2
//
// Passed: No variables passed
// Locals: No local variables
// Returned: No values returned
//   
//  Calvin Schmidt
//  Feb 2016
//  Built with IAR Embedded Workbench Version: V7.3.1.3987 (6.40.1)  
//------------------------------------------------------------------------------
 // Port 4.0
 P4SEL1 &= ~SW1; // SW1 selected
 P4SEL0 &= ~SW1; // SW1 selected
 P4DIR  &= ~SW1; // Set SW1 direction to Input
 P4OUT  |= SW1; //P4 SW1 Port Pin set high
 P4REN  |= SW1; // enabled pullup resistor
 P4IES |= SW1; // SW1 Hi/Lo edge interrupt
 P4IFG &= ~SW1; // IFG SW1 cleared
 P4IE |= SW1; // SW1 interrupt Enabled
 // Port 4.1
 P4SEL1 &= ~SW2; // SW2 selected
 P4SEL0 &= ~SW2; // SW2 selected
 P4DIR  &= ~SW2; // Set SW2 direction to Input
 P4OUT  |= SW2; //P4 SW2 Port Pin set high
 P4REN  |= SW2; // enabled pullup resistor
 P4IES |= SW2; // SW2 Hi/Lo edge interrupt
 P4IFG &= ~SW2; // IFG SW2 cleared
 P4IE |= SW2; // SW2 interrupt enabled
 //------------------------------------------------------------------------------
 }
  
 
 void Init_PortJ(void){
//------------------------------------------------------------------------------
// Configure Port J Pins
// IOT_WAKEUP             (0x01) //            
// IOT_FACTORY            (0x02) //
// IOT_STA_MINIAP         (0x04) //
// IOT_RESET              (0x08) //
// XINR                   (0x10) // XINR
// XOUTR                  (0x20) // XOUTR
//
// Passed: No variables passed
// Locals: No local variables
// Returned: No values returned
//   
//  Calvin Schmidt
//  Feb 2016
//  Built with IAR Embedded Workbench Version: V7.3.1.3987 (6.40.1)   
//------------------------------------------------------------------------------
 PJSEL0 = CLEAR; // PJ set as I/0
 PJSEL1 = CLEAR; // PJ set as I/0
 PJDIR = CLEAR; // Set PJ direction to output
 // Port J.0
 PJSEL0 &= ~IOT_WAKEUP;
 PJSEL1 &= ~IOT_WAKEUP;
 PJOUT  &= ~IOT_WAKEUP;
 PJDIR  |= IOT_WAKEUP; // Set PJ Pin 1 direction to output
 // Port J.1
 PJSEL0 &= ~IOT_FACTORY;
 PJSEL1 &= ~IOT_FACTORY;
 PJOUT  &= ~IOT_FACTORY;
 PJDIR  |= IOT_FACTORY; // Set PJ Pin 2 direction to output
 // Port J.2
 PJSEL0 &= ~IOT_STA_MINIAP;
 PJSEL1 &= ~IOT_STA_MINIAP;
 PJOUT  |= IOT_STA_MINIAP;
 PJDIR  |= IOT_STA_MINIAP; // Set PJ Pin 3 direction to output
 // Port J.3
 PJSEL0 &= ~IOT_RESET;
 PJSEL1 &= ~IOT_RESET;
 PJOUT  &= ~IOT_RESET;
 PJDIR  |= IOT_RESET; // Set P3 Pin 4 direction to output
// XT1 Setup
// PJSEL0 |= XINR;
// PJSEL0 |= XOUTR;
//------------------------------------------------------------------------------
}
