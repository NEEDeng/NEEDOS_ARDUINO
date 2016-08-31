#include <NEEDOS.h>
#include <avr/interrupt.h>
#include <avr/io.h>
//Este programa esta em fase de testes e foi desenvolvido pela NEED Tecnologia
//Este exemplo básico pisca quatro LEDS, utilizando-se das tasks, 
//task_blink1,task_blink2, task_blink3, task_blink4. 
//sendo que a task3 é ativada pela task 1 e é posta para dormir por ela mesma toda vez que e executada;

//**********PRECISA SER INCLUIDO**********//
volatile uint8_t TC0_FLAG = 0;
volatile struct task_s list_task_s[TASKS_MAX_N];
void (*task_functionP[TASKS_MAX_N])(void);
//***************************************//

#define  TASK_BLINK1_TIMER  1000  //Tempo em milisegundos
#define  TASK_BLINK1_IDX    1     //Prioridade da task

#define  TASK_BLINK2_TIMER  500   //Tempo em milisegundos
#define  TASK_BLINK2_IDX    2     //Prioridade da task

#define  TASK_BLINK3_TIMER  100   //Tempo em milisegundos
#define  TASK_BLINK3_IDX    3     //Prioridade da task

#define  TASK_BLINK4_TIMER  100   //Tempo em milisegundos
#define  TASK_BLINK4_IDX    4     //Prioridade da task

class NEEDOS need;
volatile static uint16_t incrementClogged=0;

#define LED1  8
#define LED2  9
#define LED3  10
#define LED4  11

void task_blink1();
void task_blink2();
void task_blink3();
void task_blink4();


void setup() {
  // put your setup code here, to run once:
  
  
  
  
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  //Ainda vou melhorar esse método NEEDOS
  NEEDOS(0x1E,(1<<CS02)); //para 1khz ou 1ms de delay
  
  need.task_init(); //inicializa gestor de tasks.
  //Inicializa-se as tasks e indexa as funcoes desejadas.
  need.task_constructor(task_blink1,TASK_BLINK1_TIMER, TASK_BLINK1_IDX,TASK_BLINK1_IDX);
  need.task_constructor(task_blink2,TASK_BLINK2_TIMER, TASK_BLINK2_IDX,TASK_BLINK2_IDX);
  need.task_constructor(task_blink3,TASK_BLINK3_TIMER, TASK_BLINK3_IDX,TASK_BLINK3_IDX);
  need.task_sleep(TASK_BLINK3_IDX); //Task 3 foi posta para dormir.
  need.task_constructor(task_blink4,TASK_BLINK1_TIMER, TASK_BLINK4_IDX,TASK_BLINK4_IDX);
}

void loop() {
  // put your main code here, to run repeatedly:

//**********PRECISA SER INCLUIDO**********//
if(TC0_FLAG)
    {
      need.task_increment();
      need.task_run();
      TC0_FLAG = 0;
      incrementClogged =0;
    }
    incrementClogged++;
    if(incrementClogged>30000)
    {
      TC0_FLAG = 1;
      incrementClogged=0;
      //Coloque aqui o código se alguma fault ocorrer
    }
//****************************************//
      

}


void task_blink1()
{
  //THIS
  static uint8_t toggle =0;
  digitalWrite(LED1,toggle);
  toggle = !toggle;
  //OR
  //PORTB ^=  (1<<0);
}
void task_blink2()
{
  //THIS
  static uint8_t toggle =0;
  digitalWrite(LED2,toggle);
  toggle = !toggle;
  //OR
  //PORTB ^=  (1<<1);
  need.task_wakeup(TASK_BLINK3_IDX);
}
void task_blink3()
{
  //THIS
  static uint8_t toggle =0;
  digitalWrite(LED3,toggle);
  toggle = !toggle;
  //OR
  //PORTB ^=  (1<<2);
  need.task_sleep(TASK_BLINK3_IDX);
}
void task_blink4()
{
  //THIS
  static uint8_t toggle =0;
  digitalWrite(LED4,toggle);
  toggle = !toggle;
  //OR
  //PORTB ^=  (1<<3);
}

//**********PRECISA SER INCLUIDO**********//
ISR(TIMER0_COMPA_vect)  // timer0 overflow interrupt
{
  TC0_FLAG = 1;
}
//****************************************//
