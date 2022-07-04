// Only modify this file to include
// - function definitions (prototypes)
// - include files
// - extern variable definitions
// In the appropriate section

#ifndef _test_H_
#define _test_H_
#include "Arduino.h"
//add your includes for the project test here


//end of add your includes here


//add your function definitions for the project test here

/************** Scheduler ******************/
#define time_base_loop		10				// Time cycle in milliseconds
#define num_phases			1000			// Number of phases

uint16_t phase_loop=0;
uint32_t tmr_loop = 0;

#define EXECUTELOOP()	if((millis()-tmr_loop) > time_base_loop )
#define UPDATELOOP()	tmr_loop = millis();	\
						phase_loop = (phase_loop + 1) % num_phases


#define	LOOP_x10ms(n)	if (!(phase_loop % n))
#define	LOOP_x100ms		if (!(phase_loop % 9))


//Do not add code below this line
#endif /* _test_H_ */
