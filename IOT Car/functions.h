// Function prototypes main
void main(void);
void Init_Conditions(void);
void Switches_Process(void);
void Init_Timers(void);
void Init_LEDs(void);
void Display_Process(void);
void five_msec_sleep(unsigned int fivemsec);
void Timer_code(void);

// Function prototypes clocks
void Init_Clocks(void);

// Function prototypes systems
void enable_interrupts(void);

// Function prototypes
__interrupt void Timer2_B0_ISR(void);
 __interrupt void TIMER2_B1_ISR(void);
void Init_Timer_A0(void);
void Init_Timer_B2(void);
void measure_delay(void);
void usleep10(unsigned int usec);

// Function prototypes ports
void Init_Ports(void);
void Init_Port1(void);
void Init_Port2(void);
void Init_Port3(void);
void Init_Port4(void);
void Init_PortJ(void);

// LCD
void Init_LCD(void);
void WriteIns(char instruction);
void WriteData(char data);
void ClrDisplay(void);
void SetPostion(char pos);
void DisplayOnOff(char data);
void lcd_BIG_mid(void);
void lcd_4line(void);
void lcd_out(char *s, char line, char position);

void grab_a_character(void);
int wait_for_character(void);
void menu(void);

// SPI
void Init_SPI_B0(void);
void SPI_B0_write(char byte);

// Switches
void Init_Switches(void);
void switch_control(void);
void enable_switch_SW1(void);
void enable_switch_SW2(void);
void disable_switch_SW1(void);
void disable_switch_SW2(void);
void Switches_Process(void);

// Wheels
void Right_Wheel_Off(void);
void Left_Wheel_Off(void);
void Right_Wheel_Forward(int right_wheel_forward);
void Right_Wheel_Reverse(int right_wheel_reverse);
void Left_Wheel_Forward(int left_wheel_forward);
void Left_Wheel_Reverse(int left_wheel_reverse);


// Shapes
void Triangle(void);
void Circle(void);
void Figure_Eight(void);

// ADC
void ADC_Process(void);
void Init_ADC(void);
void HEXtoBCD(int hex_value);

// Timers
void five_msec_delay(int delay);

// PWM
void Init_Timer_B1(void);
void Init_Timer_B2(void);

// Converting integers and char arrays
void dec_to_char(int value);
void char_to_dec(char value[6]);

// Serial communications
void Init_Serial_UCA0(int baud_rate);
void Init_Serial_UCA1(int baud_rate);

// IOT
void Write_USB(char character);
void Write_IOT(char character);
void Read_USB(void);
void Read_IOT(void);
int Command_length(void);
void Clear_command(void);
void send_IOT(int length);
void check_command(void);
int compare_strings(char a[], char b[]);
void Clear_Car_Command(void);
void IOT_Reset(void);
void offload_num(char data[]);




