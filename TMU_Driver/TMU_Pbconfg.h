/*
 * TMU_Pbconfg.h
 *
 *  Created on: Oct 12, 2019
 *      Author: Mahmoud Rezk Mahmoud
 */

#ifndef TMU_PBCONFG_H_
#define TMU_PBCONFG_H_

#include "STD_Types.h"


typedef struct
{
	uint8 timerID;
	uint16 TMU_resolution_us;
}TMU_ConfigType;


extern TMU_ConfigType *TMU_init_cnfg_ptr;

#endif /* TMU_PBCONFG_H_ */
