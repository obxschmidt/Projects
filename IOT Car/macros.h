//------------------------------------------------------------------------------
//
//  Description: This header file contains all macros used in other files
//
//
//  Calvin Schmidt
//  Feb 2016
//  Built with IAR Embedded Workbench Version: V7.3.1.3987 (6.40.1)
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

// Required defines
// In the event the universe no longer exists, this bit will reset
#define ALWAYS                  (1)
#define CNTL_STATE_INDEX        (3) // Control States
#define CNTL_STATE_INDEX        (3)
#define LED1                    (0x01) // LED 1
#define LED2                    (0x02) // LED 2
#define LED3                    (0x04) // LED 3
#define LED4                    (0x08) // LED 4
#define CLEAR                   (0x00) // Clear
#define RESET                   (0)
#define ms250delay              (50)
#define LINE1OFFSET             (3)
#define LINE2OFFSET             (1)
#define LINE3OFFSET             (2)
#define LINE4OFFSET             (1)
#define CASE1                   (250)
#define CASE2                   (200)
#define CASE3                   (150)
#define CASE4                   (100)
#define CASE5                   (50)
#define LENGTH                  (5)
#define SWITCHOFFSET1           (1)
#define SWITCHOFFSET2           (2)
#define SWITCHOFFSET3           (3)
#define SWITCHOFFSET4           (1)
#define MAXIMUM                 (1000)
#define LSB                     (0)
#define LENGTHOFDISPLAY         (11)
#define START                   (0)
#define NUMBEROFSHAPES          (3)

// Switch
#define CIRCLE                  (1)
#define TRIANGLE                (2)
#define FIGUREEIGHT             (3)

// LCD
#define LCD_HOME_L1	        0x80
#define LCD_HOME_L2         	0xA0
#define LCD_HOME_L3         	0xC0
#define LCD_HOME_L4         	0xE0

// Port 1
#define  SMCLK_OUT              (0x01) // SMCLK Out signal
#define  V_DETECT_R             (0x01) // 
#define  V_DETECT_L             (0x02) // 
#define  IR_LED                 (0x04) // 
#define  V_THUMB                (0x08) // 
#define  SPI_CS_LCD             (0x10) // LCD Chip Select
#define  RESET_LCD              (0x20) // LCD Reset
#define  SIMO_B                 (0x40) // SPI mode - slave in/master out of USCI_B0
#define  SOMI_B                 (0x80) // SPI mode - slave out/master in of USCI_B0

// Port 2
#define  USB_TXD                (0x01) // 
#define  USB_RXD                (0x02) // 
#define  SPI_SCK                (0x04) //  
#define  CPU_TXD                (0x20) // 
#define  CPU_RXD                (0x40) // 

// Port 3
#define  EXCEL_X                (0x01) //
#define  EXCEL_Y                (0x02) //
#define  EXCEL_Z                (0x04) //
#define  LCD_BACKLITE           (0x08) //
#define  R_FORWARD              (0x10) //
#define  R_REVERSE              (0x20) //
#define  L_FORWARD              (0x40) //
#define  L_REVERSE              (0x80) //

// Port 4
#define SW1                     (0x01) // Switch 1
#define SW2                     (0x02) // Switch 2

// Port J
#define  IOT_WAKEUP             (0x01) //            
#define  IOT_FACTORY            (0x02) //
#define  IOT_STA_MINIAP         (0x04) //
#define  IOT_RESET              (0x08) //

/*
// wheels.c
#define  HALFSECOND             (35) //
#define  ONESECOND              (70)  // 
#define  TWOSECOND              (140) //
#define  THREESECOND            (210) //
#define  ONEISHSECOND           (80) //
#define  FIFTYMSEC              (10)
#define  TENMSEC                (2) // USED TO BE 2
*/

// shapes.c
#define  CIRCLESTART            (1)
#define  ENDOFCIRCLE            (30)
#define  SECONDCIRCLE           (38)
#define  THIRDCIRCLE            (32)

//timer.c
#define TA0CCR0_INTERVAL        (25000)
#define TA0CCR1_INTERVAL        (625) 
#define TA0CCR2_INTERVAL        (0)
#define DEBOUNCE_INTERVAL       (5)
#define MAX                     (14)
#define FIRST                   (2)
#define SECOND                  (4)

// Interrupts
#define TRUE                    (1)
#define DEBOUNCED               (1)
#define FALSE                   (0)
#define BEGIN                   (1)

// ADC
#define RESET_STATE             (0)
#define BLACKLINE               (400)
#define ONE_BIT                 (1)
#define TWO                     (2)
#define FIFTYMSEC               (10)

// PWM
#define OFF                     (0)
#define WHEEL_PERIOD            (8000)
#define HALF_ISH_SPEED          (3000)
#define QUARTER_SPEED           (2000)
#define QUARTER_SPEED_ISH       (2500)
#define FULL_SPEED              (8000)
#define HALF_SPEED              (4000)

// five_msec_delay
#define ONESECOND               (200)
#define TWOSECONDS              (400)
#define THREESECONDS            (600)
#define FOURSECONDS             (800)
#define FOURISHSECONDS          (900)
#define INTERVAL                (200)
#define SHORT                   (10)
#define QUARTERSECOND           (50)

// Serial Communications
#define BEGINNING (0)
#define SMALL_RING_SIZE (9)
#define LARGE_RING_SIZE (20)
#define IOT_LENGTH (40)

// IOT
#define IP_START (11)
#define IP_END (14)
#define IP2_START (10)
#define IP2_END (13)
#define END_IP (3)
#define IOT_COMMAND_LENGTH (8)
#define COMMAND_LENGTH (10)
#define BUFFER (2)
#define PIN_BUFFER (4)
#define FORWARD1_BUFFER (5)
#define FORWARD2_BUFFER (6)
#define FORWARD3_BUFFER (7)
#define ASCII_OFFSET (0x30)
#define HUNDRED (100)
#define TEN (10)
#define ONE (1)
#define BAUD_9600 (52)
#define MSEC_LENGTH (6)
#define ADC_LENGTH (5)
#define SLASH (0x0D)















