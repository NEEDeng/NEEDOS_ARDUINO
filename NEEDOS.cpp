/**
*	\file		Threads.c
*	\author		TPO
*	\date		16/06/2015
*	\brief		Arquivo responsavel pela criacao de threads.
*	\details	Este arquivo contem a logica de funcionaento das threads
*	\copyright 	GNU V2.0
*/
#include "NEEDOS.h"
#include <avr/interrupt.h>
#include <avr/io.h>

extern volatile struct task_s list_task_s[TASKS_MAX_N];
extern void (*task_functionP[TASKS_MAX_N])(void);

NEEDOS::NEEDOS()
{
	
}

NEEDOS::NEEDOS(uint8_t ocr, uint8_t CS0x)
{
	// Set the Timer Mode to CTC
	TCCR0A = (1 << WGM01);

	// Set the value that you want to count to
	OCR0A = ocr;

	TIMSK0 = (1 << OCIE0A);    //Set the ISR COMPA vect

	sei();         //enable interrupts


	TCCR0B = CS0x;
	// set prescaler to 256 and start the timer

}
void NEEDOS::task_init()
{
	 
	
	for(int i=0;i<TASKS_MAX_N;i++)
	{
		list_task_s[i].STATUS =	TASK_STATUS_N_IMPLEMENTED;
	}
}

int8_t NEEDOS::task_sleep(uint8_t index)
{
	//if(list_task_s[index].STATUS == TASK_STATUS_RUNNING)
	{
		list_task_s[index].STATUS =	TASK_STATUS_STOPPED;
		return 1;
	}
	return -1;
}
int8_t NEEDOS::task_force(uint8_t index)
{
	
	
	(*list_task_s[index].p2f)();
	list_task_s[index].count =0;
	list_task_s[index].STATUS = TASK_STATUS_RUNNING;
	return 0;
}
int8_t NEEDOS::task_wakeup(uint8_t index)
{
	if(list_task_s[index].STATUS == TASK_STATUS_STOPPED)
	{
	//	task_reset(index);
		list_task_s[index].STATUS =	TASK_STATUS_RUNNING;
		
		return 1;
	}
	else
	return -1;
	
}

int8_t NEEDOS::task_constructor(void(*p2f)(void), uint32_t dec_ms, uint8_t prioridade, uint16_t index)
{
	list_task_s[index].STATUS =	TASK_STATUS_IDLE;
	task_functionP[index] = p2f;
	list_task_s[index].p2f = p2f;
	list_task_s[index].count = 0;
	list_task_s[index].count_max = dec_ms;
	list_task_s[index].index = index;
	list_task_s[index].prioridade = prioridade;
	return 0;
}

int8_t NEEDOS::task_changeTime(uint16_t index, uint32_t dec_ms)
{
	list_task_s[index].count_max = dec_ms;
	return 0;
}

int8_t NEEDOS::task_increment(void)
{
	for(int i=0;i<TASKS_MAX_N;i++)
	{
		if(list_task_s[i].STATUS == TASK_STATUS_RUNNING)
		{
			list_task_s[i].count++;
		}
		else if(list_task_s[i].STATUS == TASK_STATUS_IDLE)
		{
			list_task_s[i].STATUS = TASK_STATUS_RUNNING;
			list_task_s[i].count = 0;
		}
	}
	return 0;
}
int8_t NEEDOS::task_run(void)
{
	for(int i=0;i<TASKS_MAX_N;i++)
	{
		if(list_task_s[i].STATUS == TASK_STATUS_RUNNING)
		{
			if(list_task_s[i].count >= list_task_s[i].count_max)
			{
				list_task_s[i].count = 0;
				//(*task_functionP[i])();
				(*list_task_s[i].p2f)();
			}
		}
	}
	return 0;
}
enum TASK_STATUS_e NEEDOS::taskStatus(uint8_t index)
{
	return list_task_s[index].STATUS;
}
int8_t NEEDOS::taskIsRuuning(uint8_t index)
{
	if(list_task_s[index].STATUS == TASK_STATUS_RUNNING)
	return 1;
	else
	return 0;
}
int8_t NEEDOS::taskIsStopped(uint8_t index)
{
	if(list_task_s[index].STATUS == TASK_STATUS_STOPPED)
	return 1;
	else
	return 0;
}
int8_t NEEDOS::task_reset(uint8_t index)
{
	if(list_task_s[index].STATUS == TASK_STATUS_RUNNING)
	{
		list_task_s[index].count = 0;
		return 1;
	}

	return-1;
}