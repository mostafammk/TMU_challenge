/*
 * TMU_Prog.c
 *
 *  Created on: Oct 12, 2019
 *      Author: Mahmoud Rezk Mahmoud
 */

#include "TMU.h"
#include "TIMER.h"


static volatile uint8 g_systemTick_flag		= 0;
static volatile uint8 g_matchingTickCount	= 0;
static volatile uint8 g_bufferSize			= 0;
static volatile uint8 TMU_state 			= TMU_PAUSED;

TMU_SConfg TMU_cnfg_arr [NUM_OF_CONSUMER] = {{0,0,0}};


void TMU_callback(void)
{
	static uint8 g_timerTick_flag=0;

	g_timerTick_flag++;
	if(g_timerTick_flag >= g_matchingTickCount)
	{
		g_systemTick_flag++;
		g_timerTick_flag=0;
	}
}

EnmTMUError_t TMU_init(const TMU_ConfigType * ConfigPtr)
{
	uint8 retval = OK;
	uint32 maxApplicableTickTime;

	TIMER_init();
	TIMER_setCallback(ConfigPtr->timerID,TMU_callback);
	TIMER_calculateOverFlowTime(ConfigPtr->timerID,&maxApplicableTickTime);

	if(maxApplicableTickTime >= ConfigPtr->TMU_resolution_us)
	{
		g_matchingTickCount = 1;
	}
	else
	{
		g_matchingTickCount = ConfigPtr->TMU_resolution_us/maxApplicableTickTime;
	}

	return retval;

}


EnmTMUError_t TMU_Start_Timer(uint16 timeDelay,ptrToFunc EVENT_Consumer,uint8 Periodicity)
{
	uint8 retval = OK;

	timeDelay=timeDelay/TMU_init_cnfg_ptr->TMU_resolution_us;

	if(TMU_cnfg_arr[g_bufferSize].EVENT_Consumer==NULL)
	{
		TMU_cnfg_arr[g_bufferSize].timeDelay=timeDelay;
		TMU_cnfg_arr[g_bufferSize].EVENT_Consumer=EVENT_Consumer;
		TMU_cnfg_arr[g_bufferSize].Periodicity=Periodicity;
		TMU_cnfg_arr[g_bufferSize].timeOfNextExecution=g_systemTick_flag+1;
		TMU_cnfg_arr[g_bufferSize].overFlowFlag=0;
		g_bufferSize++;
		if(TMU_state==TMU_PAUSED)
		{
			retval=TIMER_start(TMU_init_cnfg_ptr->timerID);
			TMU_state = TMU_RUNNING;
		}
		else
		{

		}
	}
	else
	{
		retval=NOK;
	}




	return retval;


}

EnmTMUError_t TMU_Stop_Timer( ptrToFunc EVENT_Consumer)
{
	uint8 retval = OK;
	uint8 loopIndx;

	for(loopIndx=0 ; loopIndx<g_bufferSize ; loopIndx++)
	{
		if(TMU_cnfg_arr[loopIndx].EVENT_Consumer == EVENT_Consumer)
		{
			TMU_cnfg_arr[loopIndx] = TMU_cnfg_arr[g_bufferSize-1];
			TMU_cnfg_arr[g_bufferSize-1].EVENT_Consumer=NULL;
			g_bufferSize--;

			if(g_bufferSize == 0)
			{
				TIMER_stop(TMU_init_cnfg_ptr->timerID);
				TMU_state = TMU_PAUSED;
			}

			loopIndx = g_bufferSize+1;
		}
	}

	return retval;

}

EnmTMUError_t TMU_Dispatch(void)
{
	uint8 retval = OK;
	uint8 loopIndx=0;
	static uint8 local_systemTick_flag=0;
	void (*fncCall)(void);

	if(local_systemTick_flag > g_systemTick_flag)
	{
		for(loopIndx=0 ; loopIndx<g_bufferSize ; loopIndx++)
		{
			TMU_cnfg_arr[loopIndx].overFlowFlag=0;
		}
	}

	if(local_systemTick_flag != g_systemTick_flag)
	{
		local_systemTick_flag = g_systemTick_flag;
		for(loopIndx=0 ; loopIndx<g_bufferSize ; loopIndx++)
		{

			/***********************************************************************
			 * Execute the function if the system tick flag passed the time of the
			 * next execution,
			 * If the system tick flag is less than the previous execution time,
			 * this means that the system tick variable made an unexpected overflow,
			 * then the event has to be executed immediately
			 ***********************************************************************/


			if( g_systemTick_flag >= TMU_cnfg_arr[loopIndx].timeOfNextExecution
				||g_systemTick_flag < (TMU_cnfg_arr[loopIndx].timeOfNextExecution - TMU_cnfg_arr[loopIndx].timeDelay))
			{
				if(TMU_cnfg_arr[loopIndx].overFlowFlag == 0)
				{

					if(TMU_cnfg_arr[loopIndx].Periodicity ==PERIODIC)
					{
						fncCall = TMU_cnfg_arr[loopIndx].EVENT_Consumer;
						fncCall();
					}
					else if(TMU_cnfg_arr[loopIndx].Periodicity == ONE_SHOT)
					{
						fncCall = TMU_cnfg_arr[loopIndx].EVENT_Consumer;
						fncCall();
						TMU_Stop_Timer(fncCall);
						loopIndx--;
					}
					TMU_cnfg_arr[loopIndx].timeOfNextExecution = g_systemTick_flag + TMU_cnfg_arr[loopIndx].timeDelay;
					if(TMU_cnfg_arr[loopIndx].timeOfNextExecution < g_systemTick_flag)
					{
						TMU_cnfg_arr[loopIndx].overFlowFlag=1;
					}
				}

			}
			else
			{

			}

		}
	}

	return retval;

}

EnmTMUError_t TMU_DeInit(void)
{
	uint8 Status=OK;
	Status=TIMER_stop(TMU_init_cnfg_ptr->timerID);
	return Status;
}
