/*
 * TMU_Pbconfg.c
 *
 *  Created on: Oct 12, 2019
 *      Author: Mahmoud Rezk Mahmoud
 */
#include "TMU_Pbconfg.h"
#include "TIMER.h"

TMU_ConfigType TMU_init_cnfg = {TIMER0,500};
TMU_ConfigType *TMU_init_cnfg_ptr = &TMU_init_cnfg;



