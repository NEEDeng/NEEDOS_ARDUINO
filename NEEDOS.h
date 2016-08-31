/**
*	\file		Threads.h
*	\author		TPO
*	\date		16/06/2015
*	\brief		Arquivo responsavel pela criacao de threads.
*	\details	Este arquivo contem a logica de funcionaento das threads
*	\copyright 	GNU V2.0
*/

#ifndef NEEDOS_H
#define NEEDOS_H
#include <stdint.h>
#define TASKS_MAX_N		8
enum TASK_STATUS_e  {TASK_STATUS_IDLE, TASK_STATUS_RUNNING, TASK_STATUS_STOPPED, TASK_STATUS_END, TASK_STATUS_ABORTED, TASK_STATUS_N_IMPLEMENTED };
enum TASK_STATUS_e taskStatus(uint8_t index);
/** Tempo minimo de um milisegundo para a thread. */
#define TASK_TEMPO_MIN	1
//char (*p2f[TASKS_MAX_N]) (void); /**< Array of pointers to function with void param and return char. */
//char (**pp2f)(void); /**< Array of pointers to function with void param and return char. */
		struct task_s
		{
			uint32_t		count_interactions; /**< Valor da contagem de invocacao da task */
			uint32_t		count;		/**< Valor atual da contagem da task. */
			uint32_t		count_max;	/**< Valor maximo do contador da task. */
			enum TASK_STATUS_e	STATUS;		/**< Estado da thread. */
			uint8_t	prioridade;	/**< Prioridade da thread */
			uint16_t		index;		/**< Index da thread. */
			void (*p2f)(void);
		};

class NEEDOS
{
	public:
	

	NEEDOS();
	NEEDOS(uint8_t ocr, uint8_t CS0x);
	void task_init(void);
	int8_t task_run(void);
	int8_t task_constructor(void(*p2f)(void), uint32_t dec_ms, uint8_t prioridade, uint16_t index);
	int8_t task_increment(void);
	int8_t task_changeTime(uint16_t index, uint32_t dec_ms);
	int8_t task_reset(uint8_t index);
	int8_t task_sleep(uint8_t index);
	int8_t task_wakeup(uint8_t index);
	int8_t taskIsRuuning(uint8_t index);
	int8_t taskIsStopped(uint8_t index);
	int8_t task_force(uint8_t index);
	enum TASK_STATUS_e taskStatus(uint8_t index);
private:

};
#endif /* NEEDOS_H */