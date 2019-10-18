/*
 * TMU.h
 *
 *  Created on: Oct 12, 2019
 *      Author: Mahmoud Rezk Mahmoud
 */

#ifndef TMU_H_
#define TMU_H_

#include "TMU_Pbconfg.h"
#include "TMU_PreConfg.h"

#define PERIODIC 1
#define ONE_SHOT 0
#define TMU_RUNNING 1
#define TMU_PAUSED 0

typedef void (* ptrToFunc)(void);

typedef enum
{
	hamada
}EnmTMUError_t;

typedef struct
{
	uint8 timeDelay;
	ptrToFunc EVENT_Consumer;
	uint8 Periodicity;
	uint8 timeOfNextExecution;
	uint8 overFlowFlag;
}TMU_SConfg;


/*extern TMU_SConfg TMU_cnfg_arr [NUM_OF_CONSUMER];*/

EnmTMUError_t TMU_init(const TMU_ConfigType * ConfigPtr);
uint8 TMU_Start_Timer(uint16 TIME_Delay,void (* EVENT_Consumer)(void),uint8 Periodicity);
uint8 TMU_Stop_Timer(void (*EVENT_Consumer)(void));
uint8 TMU_Dispatch(void);
uint8 TMU_DeInit(void);

#endif /* TMU_H_ */
