/*
This work is licensed under the Creative Commons Attribution 4.0 International License. 
To view a copy of this license, visit http://creativecommons.org/licenses/by/4.0/
*/

#include <plib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "csp_core_timer.h"
#include "csp_core_debug.h"
#include "csp_comm.h"
#include "csp_ALUIntegrity.h"
#include "csp_RAMIntegrity.h"
#include "csp_diffMCU.h"
#include "csp_vitalVariables_task.h"
#include "csp_core_led.h"
#include "csp_flashIntegrity.h"
#include "csp_clockDrift_task.h"
#include "io.h"
#include "abztutorial_nonvital_tmp_eeprom.h"
#include "csp_abztutorial_debug.h"
#include "sections.h"
#include "link_to_replicas_functions.h"
#include "csp_stdLib_safetyAnd.h"
#include "flash.h"

void r1__csp_user_watchdog__initialisation(){}
void r2__csp_user_watchdog__initialisation(){}
void r1__user_watchdogTimer(){}
void r2__user_watchdogTimer(){}


#define ANNOUNCEMENT_PEDAL_CHANNEL 1
#define FREEING_PEDAL_CHANNEL 2
#define SIGNAL_OUTPUT_CHANNEL 1 

#define BOUNCING_FILTER_TIME_MS 250

#define PBCLK 80000000
#define TMR4_FREQUENCY_HZ 20000

#define PRESCALER_TMR4 8
#define TMR4_IPC 3
#define TMR4_IPS 3
        

void timerInit();

void SECTION_NON_REPLICATED_NON_SHARED_FUNCTION execute_BIST();

/**
Error simulation function
**/

void failure_delayCallToBIST();

//Initialize the driver for input and global variables. This api function has to be implemented by the example
void SECTION_NON_REPLICATED_NON_SHARED_FUNCTION api_init()
{
	abztutorial_initDebug();
    io_init();
    TRISECLR = _TRISE_TRISE3_MASK;
}

void SECTION_NON_REPLICATED_NON_SHARED_FUNCTION api_postInit()
{
    io_postInit();
	return;
}


void SECTION_NON_REPLICATED_NON_SHARED_FUNCTION api_main()
{
	uint64_t l_timeMs = nr_f__getTimeMs();
	uint64_t l_time = nr_f__getTime();
	uint64_t l_cycleCounter = 0;
	bool l_startup = false;
	uint8_t l_announcementPedalState = 0;
	uint8_t l_freeingPedalState = 0;
	uint64_t l_lastAnnoucementPedalEdgeTime = 0;
	uint64_t l_freeingPedalStateEdgeTime = 0;
	uint8_t l_areaIsFree = 0;
	uint8_t l_initialInhibition = 1;
	uint32_t i = 0;
	uint8_t l_msg[256];
		
	timerInit();
	


		
    while(1) {
		l_timeMs = nr_f__getTimeMs();		
		l_time = nr_f__getTime();
		
		io_task(); //Manage communication with the input device for acquiring the raw pedal state
				
		//Retrieve the state of the raw input
		uint8_t l_announcementPedalStateRaw = io_getInput(ANNOUNCEMENT_PEDAL_CHANNEL);
		uint8_t l_freeingPedalStateRaw = io_getInput(FREEING_PEDAL_CHANNEL);
		
		//Management of the filtering of the push button bouncing.
		if(l_announcementPedalState == 0)
		{
			if(l_announcementPedalStateRaw == 1)
			{
				l_announcementPedalState = l_announcementPedalStateRaw;
				l_lastAnnoucementPedalEdgeTime = l_timeMs;
			}
		}
		else
		{
			if((l_announcementPedalStateRaw == 0) && ((l_timeMs - l_lastAnnoucementPedalEdgeTime) > BOUNCING_FILTER_TIME_MS))
			{
				l_announcementPedalState = l_announcementPedalStateRaw;
			}
		}
		
		if(l_freeingPedalState == 0)
		{
			if(l_freeingPedalStateRaw == 1)
			{
				l_freeingPedalState = l_freeingPedalStateRaw;
				l_freeingPedalStateEdgeTime = l_timeMs;
			}
		}
		else
		{
			if((l_freeingPedalStateRaw == 0) && ((l_timeMs - l_freeingPedalStateEdgeTime) > BOUNCING_FILTER_TIME_MS))
			{
				l_freeingPedalState = l_freeingPedalStateRaw;
			}
		}
		

        r1__r_f__lc_treatement((uint32_t)(l_time >> 32),(uint32_t)(l_time & 0xffffffff),l_announcementPedalState,l_freeingPedalState);
        r2__r_f__lc_treatement((uint32_t)(l_time >> 32),(uint32_t)(l_time & 0xffffffff),l_announcementPedalState,l_freeingPedalState);
	
		r1__r_f__lc_get_freeZone(&l_areaIsFree);
		if(l_areaIsFree != 1) // The area is occupied the barrier shall be closed (unpowered)
		{
			io_setOutput(SIGNAL_OUTPUT_CHANNEL,true);
		}
		else
		{
			io_setOutput(SIGNAL_OUTPUT_CHANNEL,false);
		}
		
		temperature_Eeprom_task();
		
		//execute_BIST();
		failure_delayCallToBIST();
        
		if((l_cycleCounter % 10000) == 0)
        {
			nr_f__invertLedGreen();
			if(l_initialInhibition == 0)
			{
				if(l_areaIsFree != 1) // The area is occupied the barrier shall be closed (unpowered)
				{
					snprintf((char*)(l_msg),sizeof(l_msg),"Area is occuiped - barrier is closed \n");
					printDebugText(l_msg, strlen((char*)(l_msg)));	;
				}
				else
				{
					snprintf((char*)(l_msg),sizeof(l_msg),"Area is free - barrier is open \n");
					printDebugText(l_msg, strlen((char*)(l_msg)));	;
				}			
			}
		}
		l_cycleCounter++; 	   
		
		// No permissive output can be performed until all the vital built-in test have been passed at least one time.
		// This conditional block intends to speed up the start of the system.
		if(l_initialInhibition == 1)
		{
			//Speed up the startup
			for(i = 0; i<250;i++)
			{
				csp_flashIntegrity_task();
			}
			for(i = 0; i<10;i++)
			{
				csp_RAMIntegrity_task();
			}
			r1__r_f__get_CS0_outputInhibition(&l_initialInhibition);
		}
		else
		{		
			if(l_startup == false)
			{
				snprintf((char*)(l_msg),sizeof(l_msg),"All built-in test passed after %d ms\n",(uint32_t)(nr_f__getTimeMs()));
				printDebugText(l_msg, strlen((char*)(l_msg)));	
				l_startup = true;
			}
		}
	}
}


void SECTION_NON_REPLICATED_NON_SHARED_FUNCTION execute_BIST()
{
	csp_commTask();
	csp_flashIntegrity_task();
	csp_RAMIntegrity_task();
	csp_vitalVariables_task();
	csp_ALUIntegrity_task();
	csp_clockDrift_task();
	csp_diffMCU_task();      
}


//Configure the timer used for output frequency generation
void SECTION_NON_REPLICATED_NON_SHARED_FUNCTION timerInit()
{
    T4CONCLR = 0xFFFFFFFF;  
    IPC4CLR = (_IPC4_T4IP_MASK << _IPC4_T4IP_POSITION) | (_IPC4_T4IS_MASK << _IPC4_T4IS_POSITION);
    IEC0CLR = _IEC0_T4IE_MASK;
    
    //Configure the timer
    TMR4 = 0;
    PR4 = PBCLK/(TMR4_FREQUENCY_HZ*PRESCALER_TMR4);
    T4CONSET = (0 << _T4CON_SIDL_POSITION) | // Continue operation in idle mode
               (0 << _T4CON_TGATE_POSITION) | // Gate time accumulation is disabled
               (0b011 << _T4CON_TCKPS_POSITION) | // Prescaler 1::8
               (0 << _T4CON_TCS_POSITION); // Internal peripheral clock source
    IPC4SET = ( TMR4_IPC << _IPC4_T4IP_POSITION) | (TMR4_IPS << _IPC4_T4IS_POSITION) ; //Setting the priority of the interrupt       
    IEC0SET = _IEC0_T4IE_MASK;
    T4CONSET = _T4CON_ON_MASK;
}



//Interrupt Service Routine in charge of the generation of the frequency on the vital output pin.
void __ISR(_TIMER_4_VECTOR,ipl3SOFT) safetyFlasher_timer4ISR(void)
{
    static uint32_t timerCounter = 0;
    timerCounter++;
    if ((timerCounter & 0b01) == 0) {
#ifndef __CSP_DEBUG__
		csp_stdLib_safetyAnd(r1__r_f__lc_getSetAddressAndValue,&r2__r_f__lc_getSetAddressAndValue);
#endif
#ifdef __CSP_DEBUG__
		csp_stdLib_safetyAnd(&r1__r_f__lc_getSetAddressAndValue,&r2__r_f__lc_getSetAddressAndValue);
#endif
	} else {
#ifndef __CSP_DEBUG__
		csp_stdLib_safetyAnd(r1__r_f__lc_getClearAddressAndValue,&r2__r_f__lc_getClearAddressAndValue);
#endif
#ifdef __CSP_DEBUG__
		csp_stdLib_safetyAnd(&r1__r_f__lc_getClearAddressAndValue,&r2__r_f__lc_getClearAddressAndValue);
#endif
	}
	    
    IFS0CLR = _IFS0_T4IF_MASK;
}


void SECTION_NON_REPLICATED_NON_SHARED_FUNCTION failure_delayCallToBIST()
{

	
	if(nr_f__getTimeMs() <= 15000)
	{
		execute_BIST();
	}
	else
	{
		csp_commTask();
		csp_flashIntegrity_task();
		csp_RAMIntegrity_task();
		//csp_vitalVariables_task();
		csp_ALUIntegrity_task();
		csp_clockDrift_task();
		csp_diffMCU_task(); 
	}
}


