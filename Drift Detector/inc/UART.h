#ifndef UART_H
#define UART_H

#include <stdint.h>
#include <MKL25Z4.H>
#include "queue.h"

#define USE_UART_INTERRUPTS (0)

#define UART_OVERSAMPLE (16)
#define BUS_CLOCK 			(24e6)
#define EMPTY						(0)
#define LOADING					(1)
#define FULL						(2)
#define CHECKSUM				(3)
#define BUFFER_SIZE			(150)
#define FIVE_CHARS		  (5)
#define TRUE						(1)
#define NEWLINE					(0x0A)
#define CARRIAGE_RETURN (0x0D)
// UART0 ISR FSM Macros
#define SENTENCE_START	(0)
#define SENTENCE_TYPE		(1)
#define SENTENCE_BODY		(2)
#define SENTENCE_CHECKSUM	(3)

void Init_UART0(uint32_t baud_rate);

void Init_UART2(uint32_t baud_rate);
void UART2_Transmit_Poll(uint8_t data);
uint8_t UART2_Receive_Poll(void);

void Send_String(char * str);
uint32_t Get_Num_Rx_Chars_Available(void);
uint8_t	Get_Char(void);
void Clear_Queue(Q_T * q);

int Is_Text(uint8_t d);
int Is_Valid_Character(uint8_t d);
int Is_Number(uint8_t d);

extern Q_T Tx_Data, Rx_Data;

extern char sentence_buffer[BUFFER_SIZE];

#endif
// *******************************ARM University Program Copyright © ARM Ltd 2013*************************************   
