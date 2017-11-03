#ifndef GPIO_DEFS_H
#define GPIO_DEFS_H
#include <MKL25Z4.h>

#define ABS(x) (x<0? (-x):x)

#define MASK(x) (1UL << (x))

#define DEBUG1_POS (1) 	// on port B
#define DEBUG2_POS (2) 	// on port B
#define DEBUG3_POS (3) 	// on port B

#define CONFIG1_POS (5) // on port E
#define CONFIG2_POS (4) // on port E
#define CONFIG3_POS (3) // on port E

#define SET_BIT(x) {PTB->PSOR = MASK(x);}
#define CLEAR_BIT(x) {PTB->PCOR = MASK(x);}
#define TOGGLE_BIT(x) {PTB->PTOR = MASK(x);}

extern volatile int idle_counter;

#endif
