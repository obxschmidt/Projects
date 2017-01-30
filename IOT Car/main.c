//------------------------------------------------------------------------------
//
//  Description: This file contains the Main Routine - "While" Operating System
//
//  Calvin Schmidt
//  Feb 2016
//  Built with IAR Embedded Workbench Version: V7.3.1.3987 (6.40.1)
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#include  "msp430.h"
#include  "functions.h"
#include  "macros.h"

// Global Variables
volatile unsigned char control_state[CNTL_STATE_INDEX];
volatile unsigned int Time_Sequence;
volatile unsigned int counter = START;
char led_smclk;
// Display Globals
extern char display_line_1[LENGTHOFDISPLAY];
extern char display_line_2[LENGTHOFDISPLAY];
extern char display_line_3[LENGTHOFDISPLAY];
extern char display_line_4[LENGTHOFDISPLAY];
extern char *display_1;
extern char *display_2;
extern char *display_3;
extern char *display_4;
char posL1;
char posL2;
char posL3;
char posL4;
// Interrupt Globals
int msec_count; // used for 5msec delay
// ADC Globals
int ADC_Thumb;
char adc_char[ADC_LENGTH];
int Current_Time;
int ADC_Channel = CLEAR;
int ADC_Left_Detector;
int ADC_Right_Detector;
char msec_display[MSEC_LENGTH]; // For dec_to_char (hextobcd)
int SW_Count = CLEAR;
int Black_Line;
int White_Line;
int ADC_Flag = FALSE; // Start/Disables black line detection code in main
// PWM Globals
int right_forward_rate;
int right_reverse_rate;
int left_forward_rate;
int left_reverse_rate;
// Serial Communications globals
char message[SMALL_RING_SIZE] = "NCSU  #1";
int usb_rx_ring_wr;
int usb_rx_ring_rd;
int usb_tx_ring_wr;
int usb_tx_ring_rd;
char USB_Char_Tx[SMALL_RING_SIZE];
char USB_Char_Rx[SMALL_RING_SIZE];
char IOT_Char_Tx[SMALL_RING_SIZE];
char IOT_Char_Rx[SMALL_RING_SIZE];
int iot_tx_ring_wr;
int iot_rx_ring_wr;
int iot_tx_ring_rd;
int iot_rx_ring_rd;
int First_Char_Received = FALSE;
int check_com;
int buff = CLEAR;
int com_ind = CLEAR;
int catch;
char iot_command[IOT_LENGTH];
int iot_length;
char iot_ip[IOT_LENGTH];
char ip_display[IP_END];
int ip_display_counter = CLEAR;
char ip_display_2[PIN_BUFFER];
char ip_command_array[IOT_COMMAND_LENGTH] = "AT+S.STS";
int iot_flag;
char car_command[HUNDRED];
extern int start_command;
int time_forward;
int time_forward1;
int time_forward2;
int time_forward3;
char* left_adc_ptr;
char* right_adc_ptr;
char left_adc_data[MSEC_LENGTH];
char right_adc_data[MSEC_LENGTH];
int Controller_Flag = TRUE;
int iot_reset_flag = FALSE;

// other globals
int adc_display_flag = FALSE;



void main(void){
//------------------------------------------------------------------------------
// Main Program
// This is the main routine for the program. Execution of code starts here.
// The operating system is Back Ground Fore Ground.
// 
//  Calvin Schmidt
//  Feb 2016
//  Built with IAR Embedded Workbench Version: V7.3.1.3987 (6.40.1)  
//------------------------------------------------------------------------------
  Init_Ports();                             // Initialize Ports
  Init_Clocks();                            // Initialize Clock System 
  Init_Conditions();                        //
  Init_Timers();                            // Initialize Timers
  Init_LCD();                               // Initialize LCD
  Init_ADC();                               // Initialize ADC
  Init_Serial_UCA0(BAUD_9600);              // Baud Rate Value to get 9600
  Init_Serial_UCA1(BAUD_9600);
  ADC_Process();
  
  display_1 = "Project 08";
  posL1 = CLEAR;
  Display_Process();
  
  PJOUT |= IOT_RESET; // Pull IOT_RESET Pin high
  Clear_command();
  
  //--------------
  // Temporary test stuff
 
  //--------------
  
//------------------------------------------------------------------------------
// Begining of the "While" Operating System
//------------------------------------------------------------------------------
 while(ALWAYS) {                            // Can the Operating system run
   
   if (iot_reset_flag) { // Handles resetting IOT due to Dissassociation
     iot_reset_flag = FALSE;
     IOT_Reset();
   }
   
   if (car_command[ONE+PIN_BUFFER] == 'E') { // Allows exiting black line detection
           ADC_Flag = FALSE;
           Controller_Flag = TRUE;
           Right_Wheel_Off();
           Left_Wheel_Off();
         } 
   
   if (Controller_Flag) { // Flag to enable sending car commands via controller
     if (!(P4IN & SW1)) { // Puts IP address into 2 arrays and displays them
       for (int i = IP_START; i <= (IP_START+IP_END); i++) {
         ip_display[ip_display_counter] = iot_ip[i];
         ip_display_counter++;
         five_msec_delay(SHORT);
       }
       ip_display[IP_END - TRUE] = '\0';
       display_3 = ip_display;
       
       ip_display_counter = CLEAR;
       for (int i = IP2_START; i <= IP2_END; i++) {
         ip_display_2[ip_display_counter] = ip_display[i];
         ip_display_counter++;
         five_msec_delay(SHORT);
       }
       ip_display_2[END_IP] = '\0';
       display_4 = ip_display_2;
       
       Display_Process();
     }
     
     if (!(P4IN & SW2)) { // sends command to IOT to get current IP address
       five_msec_delay(QUARTERSECOND);
       for (int i = CLEAR; i < IOT_COMMAND_LENGTH; i++) {
         iot_command[i] = ip_command_array[i];
       }
       check_command();
     }
     
     if (start_command == TRUE) { // flag which enables commands
       for (int n = BEGIN; n <= 10; n = n+BUFFER) { 
         if (car_command[n+PIN_BUFFER] == 'F') { // Forward
           Right_Wheel_Forward(FULL_SPEED);
           Left_Wheel_Forward(FULL_SPEED);
           //
           display_2 = "Forward";
           Display_Process();
         }
         if (car_command[n+PIN_BUFFER] == 'B') { // Back
           Right_Wheel_Reverse(FULL_SPEED);
           Left_Wheel_Reverse(FULL_SPEED);
           //
           display_2 = "Reverse";
           Display_Process();
         }
         if (car_command[n+PIN_BUFFER] == 'R') { // Reverse
           Left_Wheel_Forward(FULL_SPEED);
           Right_Wheel_Reverse(FULL_SPEED);
           //
           display_2 = "Right";
           Display_Process();
         }
         if (car_command[n+PIN_BUFFER] == 'L') { // Left
           Right_Wheel_Forward(FULL_SPEED);
           Left_Wheel_Reverse(FULL_SPEED);
           //
           display_2 = "Left";
           Display_Process();
         }
         if (car_command[n+PIN_BUFFER] == 'S') { // Stopped
           Right_Wheel_Off();
           Left_Wheel_Off();
           //
           display_2 = "Stopped";
           Display_Process();
         }    
         if (car_command[n+PIN_BUFFER] == 'D') { // Switch to automatic mode
           ADC_Flag = TRUE;
           Controller_Flag = FALSE;
           Right_Wheel_Off();
           Left_Wheel_Off();
         }
       }
       start_command = FALSE;       
       Clear_Car_Command();
     }
   }
   
   
   /*// Old code for testing black line detection and debugging
   if (!(P4IN & SW1)) { // enables black line detections
     five_msec_delay(100);
     ADC_Flag = TRUE;
     adc_display_flag = FALSE;
   }
   
   if (adc_display_flag) { // enables the display to show adc values
     ADC_Flag = FALSE;
     
     display_1 = "L_Detector";
     display_3 = "R_Detector";
     dec_to_char(ADC_Left_Detector);
     offload_num(left_adc_data);
     left_adc_ptr = left_adc_data;
     display_2 = left_adc_ptr;
     dec_to_char(ADC_Right_Detector);
     offload_num(right_adc_data);
     right_adc_ptr = right_adc_data;
     display_4 = right_adc_ptr;
     five_msec_delay(10);
     Display_Process();
   }
   if (!(P4IN & SW2)) {
     adc_display_flag = TRUE;
   }
    */
   // Black line detection code
   if (ADC_Flag) {
     Right_Wheel_Forward(QUARTER_SPEED);
     Left_Wheel_Forward(QUARTER_SPEED);
     
     if (ADC_Left_Detector <= BLACKLINE) {
       while (ADC_Left_Detector <= BLACKLINE) {
         Right_Wheel_Off();
       }
     }
     
     if (ADC_Right_Detector <= BLACKLINE) {
       while (ADC_Right_Detector <= BLACKLINE){
         Left_Wheel_Off();
       }
     }
     // Both detectors detects white
     
     //if (ADC_Left_Detector <= BLACKLINE && ADC_Right_Detector <= BLACKLINE) {
       //Right_Wheel_Reverse(QUARTER_SPEED);
       //Left_Wheel_Reverse(QUARTER_SPEED);
     //}
   }
 }
}
//------------------------------------------------------------------------------









