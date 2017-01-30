#include "UART.h"
#include <stdio.h>

Q_T TxQ, RxQ;
int checksum_counter = 0;
int buffer_state = EMPTY;
int fsm_isr_counter = SENTENCE_START;
int sentence_type_counter = 0;
//extern int sentence_buffer_size; // used for testing

struct __FILE
{
  int handle;
};

FILE __stdout;  //Use with printf
FILE __stdin;		//use with fget/sscanf, or scanf


//Retarget the fputc method to use the UART0
int fputc(int ch, FILE *f){
	while(!(UART0->S1 & UART_S1_TDRE_MASK) && !(UART0->S1 & UART_S1_TC_MASK));
	UART0->D = ch;
	return ch;
}

//Retarget the fgetc method to use the UART0
int fgetc(FILE *f){
	while(!(UART0->S1 & UART_S1_RDRF_MASK));
	return UART0->D;
}

void Init_UART0(uint32_t baud_rate) {
	int sysclk = 48000000;
	// int baud = 115200;
	uint32_t osr = 15;
	uint16_t sbr;
	uint8_t temp;
	
	SIM->SCGC4 |= SIM_SCGC4_UART0_MASK; 										// enable clock gating to uart0 module 
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;											// enable clock gating to port A 	
	UART0->C2 &= ~UART0_C2_TE_MASK & ~UART0_C2_RE_MASK; 		// disable rx and tx for register programming 
	// set uart clock to oscillator clock 
	SIM->SOPT2 |= SIM_SOPT2_UART0SRC(01);
	SIM->SOPT2 |= SIM_SOPT2_PLLFLLSEL_MASK;
	// set pins to uart0 rx/tx 
	PORTA->PCR[1] = PORT_PCR_ISF_MASK | PORT_PCR_MUX(2); // Rx
	PORTA->PCR[2] = PORT_PCR_ISF_MASK | PORT_PCR_MUX(2); // Tx
	
	sbr = (uint16_t)((sysclk)/(baud_rate * osr+1));
	temp = UART0->BDH & ~(UART0_BDH_SBR(0x1F));
	UART0->BDH = temp | UART_BDH_SBR(((sbr & 0x1F00) >> 8));
	UART0->BDL = (uint8_t)(sbr & UART_BDL_SBR_MASK);
	UART0->C4 |= UART0_C4_OSR(osr);													// set oversampling ratio 
	
	// keep default settings for parity and loopback
	UART0->C1 = 0;
	UART0->C3 |= 0;
	UART0->MA1 = 0;
	UART0->MA2 = 0;
	UART0->S1 |= 0x00;
	UART0->S2 = 0x00;
	UART0->C2 |= UART0_C2_TE_MASK | UART0_C2_RE_MASK;				//enable UART
	
	// Interrupt initialization code
	NVIC_SetPriority(UART0_IRQn, 2); // 0, 1, 2, or 3
	NVIC_ClearPendingIRQ(UART0_IRQn); 
	NVIC_EnableIRQ(UART0_IRQn);

	UART0->C2 |= UART_C2_TIE_MASK;
	UART0->C2 |= UART_C2_RIE_MASK;
	Q_Init(&TxQ);
	Q_Init(&RxQ);
}

void Init_UART2(uint32_t baud_rate) {
	uint32_t divisor;
	
	// enable clock to UART and Port E
	SIM->SCGC4 |= SIM_SCGC4_UART2_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;


	// select UART pins
	PORTE->PCR[22] = PORT_PCR_MUX(4); // Tx
	PORTE->PCR[23] = PORT_PCR_MUX(4); // Rx
	
	UART2->C2 &=  ~(UARTLP_C2_TE_MASK | UARTLP_C2_RE_MASK);
		
	// Set baud rate to 4800 baud
	divisor = BUS_CLOCK/(baud_rate*16);
	UART2->BDH = UART_BDH_SBR(divisor>>8);
	UART2->BDL = UART_BDL_SBR(divisor);
	
	// No parity, 8 bits, two stop bits, other settings;
	UART2->C1 = 0; 
	UART2->S2 = 0;
	UART2->C3 = 0;
	
// Enable transmitter and receiver but not interrupts
	UART2->C2 = UART_C2_TE_MASK | UART_C2_RE_MASK;
	
#if USE_UART_INTERRUPTS
	NVIC_SetPriority(UART2_IRQn, 2); // 0, 1, 2, or 3
	NVIC_ClearPendingIRQ(UART2_IRQn); 
	NVIC_EnableIRQ(UART2_IRQn);

	UART2->C2 |= UART_C2_TIE_MASK | UART_C2_RIE_MASK;
	//	UART2->C2 |= UART_C2_RIE_MASK;
	Q_Init(&TxQ);
	Q_Init(&RxQ);
#endif

}

void UART0_Transmit_Poll(uint8_t data) {
		while (!(UART0->S1 & UART_S1_TDRE_MASK))
			;
		UART0->D = data;
}	

uint8_t UART0_Receive_Poll(void) {
		while (!(UART0->S1 & UART_S1_RDRF_MASK))
			;
		return UART0->D;
}	

// Returns true if character is alphabetical
int Is_Text(uint8_t d) {
  return (((d >= 'A') && (d <= 'Z')) || 
		     ((d >= 'a') && (d <= 'z')));
}
// Returns true if character is valid
int Is_Valid_Character(uint8_t d) {
	return (((d >= 'A') && (d <= 'Z')) || 
		     ((d >= 'a') && (d <= 'z'))  ||
				 ((d >= '0') && (d <= '9'))  ||
				 ((d == ',') || (d == '.')));
}

int Is_Number(uint8_t d) {
	return ((d >= '0' && d <= '9') || (d == '.'));
}

void Task_Start() {
	Q_Enqueue(&RxQ, UART0->D);
	if (Q_Char(&RxQ) == '$') { 
		buffer_state = LOADING;
		fsm_isr_counter = SENTENCE_TYPE;
		Q_Dequeue(&RxQ); // Remove '$', only needed to signify start of message
	}
  else {
    Q_Dequeue(&RxQ); // remove char from queue if its not message starter
  }	
}

void Task_Sentence_Type() {
	Q_Enqueue(&RxQ, UART0->D);
	if (!Is_Text(Q_Char(&RxQ))) {
		// Non alphabetical character received
		Clear_Queue(&RxQ);
		buffer_state = EMPTY; // Set buffer state to empty
		fsm_isr_counter = SENTENCE_START; // Restart FSM
		printf("Error. Non-Alphabetical sentence type.\n");
		Init_UART0(115200); 
	}
  else
		sentence_type_counter++;
	if (sentence_type_counter >= 5)
		fsm_isr_counter = SENTENCE_BODY; // 5 valid characters received, proceed to next state
}

void Task_Sentence_Body() {
	Q_Enqueue(&RxQ, UART0->D);
	if (Q_Char(&RxQ) == '*') {
		// * received, move FSM to checksum
		fsm_isr_counter = SENTENCE_CHECKSUM;
	}
	if ((!Is_Valid_Character(Q_Char(&RxQ))) && !(Q_Char(&RxQ) == '*')) {
		// Non valid character received
		Clear_Queue(&RxQ);
		buffer_state = EMPTY; // Set buffer state to empty
		fsm_isr_counter = SENTENCE_START; // Restart FSM
		printf("Error. Invalid sentence body.\n");
		/* Should an invalid character be received the UART ISR breaks on subsequent calls
		Not sure why this happens but reinitalizing it here seems to fix it*/
		Init_UART0(115200); 
	}
}

void Task_Checksum() {
	Q_Enqueue(&RxQ, UART0->D);
	checksum_counter++;
	if (checksum_counter >= 2) {
		// Received both checksum chars
		buffer_state = FULL; // Set buffer state to full, ending FSM
	}
}

void Clear_Queue(Q_T * q) {
	int i;
	for(i = q->Head; i < q->Tail; i++) {
		Q_Dequeue(q);
	}
	q->Head = 0;
	q->Tail = 0;
}

void UART0_IRQHandler(void) {
	NVIC_ClearPendingIRQ(UART0_IRQn);
	if (UART0->S1 & UART_S1_TDRE_MASK) {
		// can send another character
		if (!Q_Empty(&TxQ)) {
			UART0->D = Q_Dequeue(&TxQ);
		} else {
			// queue is empty so disable transmitter
			UART0->C2 &= ~UART_C2_TIE_MASK;
		}
	}
	if (UART0->S1 & UART_S1_RDRF_MASK) {
		// received a character
		if (!Q_Full(&RxQ)) {
			if (buffer_state == EMPTY) {
				fsm_isr_counter = SENTENCE_START; // restarts FSM if buffer state is empty
				checksum_counter = 0; // resets counter on FSM restart
			}
			switch (fsm_isr_counter) {
				case SENTENCE_START:
					Task_Start(); 
				  break;
				case SENTENCE_TYPE:
					Task_Sentence_Type(); 
				  break;
				case SENTENCE_BODY:
					Task_Sentence_Body(); 
				  break;
				case SENTENCE_CHECKSUM:
					Task_Checksum(); 
				  break;
				default:
					break;
			}
		}
		else {
			// error - queue full.
			Clear_Queue(&RxQ); // Clears queue if its full (might not be a good idea)
			printf("Error. Queue Full. Resend message.\n");
		}
	}
	if (UART0->S1 & (UART_S1_OR_MASK | UART_S1_NF_MASK | 
		UART_S1_FE_MASK | UART_S1_PF_MASK)) {
			// handle the error
			// clear the flag
			/*
			UART2->S1 = UART_S1_OR_MASK | UART_S1_NF_MASK | 
				UART_S1_FE_MASK | UART_S1_PF_MASK;
			*/
		}
}
void Send_String(char * str) {
	// enqueue string
	while (*str != '\0') { // copy characters up to null terminator
		while (Q_Full(&TxQ))
			; // wait for space to open up
		Q_Enqueue(&TxQ, *str);
		str++;
	}
	// start transmitter if it isn't already running
	if (!(UART0->C2 & UART_C2_TIE_MASK)) {
		UART0->C2 |= UART_C2_TIE_MASK;
		UART0->D = Q_Dequeue(&TxQ); 
	}
}

uint32_t Get_Num_Rx_Chars_Available(void) {
	return Q_Size(&RxQ);
}

uint8_t	Get_Char(void) {
	return Q_Dequeue(&RxQ);
}
// *******************************ARM University Program Copyright © ARM Ltd 2013*************************************   
