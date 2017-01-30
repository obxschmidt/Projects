#include "queue.h"
int sentence_buffer_size = 0;
extern char sentence_buffer[];

void Q_Init(Q_T * q) {
  unsigned int i;
  for (i=0; i<Q_SIZE; i++)  
    q->Data[i] = 0;  // to simplify our lives when debugging
  q->Head = 0;
  q->Tail = 0;
  q->Size = 0;
}

int Q_Empty(Q_T * q) {
  return q->Size == 0;
}

int Q_Full(Q_T * q) {
  return q->Size == Q_SIZE;
}

int Q_Size(Q_T * q) {
	return q->Size;
}

char Q_Char(Q_T * q) {
	int previous_char = q->Tail - 1;
	return q->Data[previous_char];
}

int Q_Enqueue(Q_T * q, uint8_t d) {
  // if queue is full, abort rather than overwrite and return
  // an error code
  if (!Q_Full(q)) {
    q->Data[q->Tail++] = d;
		//sentence_buffer[sentence_buffer_size++] = d;
    q->Tail %= Q_SIZE;
    q->Size++;
    return 1; // success
  } else 
    return 0; // failure
}

uint8_t Q_Dequeue(Q_T * q) {
  // Must check to see if queue is empty before dequeueing
  uint8_t t=0;
  if (!Q_Empty(q)) {
    t = q->Data[q->Head];
    q->Data[q->Head++] = 0; // empty unused entries for debugging
    q->Head %= Q_SIZE;
    q->Size--;
  }
  return t;
}


// *******************************ARM University Program Copyright © ARM Ltd 2013*************************************   
