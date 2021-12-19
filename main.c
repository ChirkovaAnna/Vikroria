#include "stm32f10x.h"
#include <stdio.h>		/* ???????????? ???? ????????? ????????? ????????????? ????? ? ???????? */
#include <stdint.h>		/* ???????????? ???? ????????? ????????? ????????????? ????? ? ???????? */
#include <string.h>
#include <stdbool.h>
//#include "core_cm3.h"
#define false 0
#define true 1
#define GPIO_PIN_0                 ((uint16_t)0x0001)
#define GPIO_PIN_1                 ((uint16_t)0x0002)  
#define GPIO_PIN_2                 ((uint16_t)0x0004)  
#define GPIO_PIN_3                 ((uint16_t)0x0008)  
#define GPIO_PIN_4                 ((uint16_t)0x0010)  
#define GPIO_PIN_5                 ((uint16_t)0x0020)  
#define GPIO_PIN_6                 ((uint16_t)0x0040) 
#define GPIO_PIN_7                 ((uint16_t)0x0080) 
#define GPIO_PIN_8                 ((uint16_t)0x0100)  
#define GPIO_PIN_9                 ((uint16_t)0x0200)  
#define GPIO_PIN_10                ((uint16_t)0x0400) 
#define GPIO_PIN_11                ((uint16_t)0x0800)  
#define GPIO_PIN_12                ((uint16_t)0x1000)  
#define GPIO_PIN_13                ((uint16_t)0x2000)  
#define GPIO_PIN_14                ((uint16_t)0x4000)  
#define GPIO_PIN_15                ((uint16_t)0x8000)  
#define GPIO_PIN_All               ((uint16_t)0xFFFF) 

#define PIN_CODE_0                 (GPIOA->IDR & GPIO_PIN_0)  
#define PIN_CODE_1                 ((GPIOA->IDR & GPIO_PIN_4)>>3)  
#define PIN_CODE_2                 ((GPIOA->IDR & GPIO_PIN_6)>>4)  
#define PIN_CODE_3                 ((GPIOA->IDR & GPIO_PIN_7)>>4)  
#define PIN_CODE_4                 ((GPIOA->IDR & GPIO_PIN_8)>>4)  
#define PIN_CODE_5                 ((GPIOA->IDR & GPIO_PIN_9)>>4) 
#define PIN_CODE_6                 ((GPIOA->IDR & GPIO_PIN_10)>>4) 
#define PIN_CODE_7                 ((GPIOA->IDR & GPIO_PIN_12)>>5)

#define LED1_ON() SET_BIT(GPIOC->ODR,GPIO_ODR_ODR0)
#define LED1_OFF() CLEAR_BIT(GPIOC->ODR,GPIO_ODR_ODR0)
#define LED2_ON() SET_BIT(GPIOC->ODR,GPIO_ODR_ODR1)
#define LED2_OFF() CLEAR_BIT(GPIOC->ODR,GPIO_ODR_ODR1)
#define LED3_ON() SET_BIT(GPIOC->ODR,GPIO_ODR_ODR2)
#define LED3_OFF() CLEAR_BIT(GPIOC->ODR,GPIO_ODR_ODR2)
#define LED4_ON() SET_BIT(GPIOC->ODR,GPIO_ODR_ODR3)
#define LED4_OFF() CLEAR_BIT(GPIOC->ODR,GPIO_ODR_ODR3)
#define LED5_ON() SET_BIT(GPIOC->ODR,GPIO_ODR_ODR4)
#define LED5_OFF() CLEAR_BIT(GPIOC->ODR,GPIO_ODR_ODR4)
#define LED6_ON() SET_BIT(GPIOC->ODR,GPIO_ODR_ODR5)
#define LED6_OFF() CLEAR_BIT(GPIOC->ODR,GPIO_ODR_ODR5)
#define LED7_ON() SET_BIT(GPIOC->ODR,GPIO_ODR_ODR8)
#define LED7_OFF() CLEAR_BIT(GPIOC->ODR,GPIO_ODR_ODR8)
#define LED8_ON() SET_BIT(GPIOC->ODR,GPIO_ODR_ODR7)
#define LED8_OFF() CLEAR_BIT(GPIOC->ODR,GPIO_ODR_ODR7)

uint16_t time = 0;             		    //????????
int8_t button_click =0;           		//?????????? ??????? ??????
uint32_t bright_end =0;       		    //???????
uint32_t array = 0;
void modulation (void);              // ?????????
uint32_t code_read(void);						//??????? ???????? ???????? ? ??????
void interrupt_button(void);				  //?????????? ??????????
void delay (uint32_t ticks);					//??????? ???????? ?????????
uint32_t code_counterk(void);
uint32_t code_countertpwm(void);
uint32_t code_countertmax(void);
uint32_t code_counterper(void);
float per = 0;
float k = 0;
float tpwm = 0;
float tmax = 0;
_Bool run = 0;
uint32_t array1 = 0;
uint32_t array2 = 0;
uint32_t array3 = 0;
uint32_t array4 = 0;
	
int main (void)
{
	
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;   // ????????? ???????????? ??? ????? A
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;   // ????????? ???????????? ??? ????? C
	
	  /* ????????? ???????? CRL ????? MODE(0-7)[1:0] ?? ??? ?????? ????????? 50 MHz */
	  GPIOC->CRL = (GPIOC->CRL) | ( GPIO_CRL_MODE0_0 | GPIO_CRL_MODE0_1);
	 	GPIOC->CRL = (GPIOC->CRL) | ( GPIO_CRL_MODE1_0 | GPIO_CRL_MODE1_1);
 		GPIOC->CRL = (GPIOC->CRL) | ( GPIO_CRL_MODE2_0 | GPIO_CRL_MODE2_1);
		GPIOC->CRL = (GPIOC->CRL) | ( GPIO_CRL_MODE3_0 | GPIO_CRL_MODE3_1);
		GPIOC->CRL = (GPIOC->CRL) | ( GPIO_CRL_MODE4_0 | GPIO_CRL_MODE4_1);
		GPIOC->CRL = (GPIOC->CRL) | ( GPIO_CRL_MODE5_0 | GPIO_CRL_MODE5_1);
		GPIOC->CRL = (GPIOC->CRL) | ( GPIO_CRL_MODE6_0 | GPIO_CRL_MODE6_1);
		GPIOC->CRL = (GPIOC->CRL) | ( GPIO_CRL_MODE7_0 | GPIO_CRL_MODE7_1);
	  GPIOC->CRH = (GPIOC->CRH) | ( GPIO_CRH_MODE8_0 | GPIO_CRH_MODE8_1);
	
	  /* ????????? ???????? CRL ????? CNF5[1:0] ?? ??? ?????? push-pull */
	  GPIOC->CRL = (GPIOC->CRL) & ( ~( GPIO_CRL_CNF0_0 | GPIO_CRL_CNF0_1));
		GPIOC->CRL = (GPIOC->CRL) & ( ~( GPIO_CRL_CNF1_0 | GPIO_CRL_CNF1_1));
		GPIOC->CRL = (GPIOC->CRL) & ( ~( GPIO_CRL_CNF2_0 | GPIO_CRL_CNF2_1));
		GPIOC->CRL = (GPIOC->CRL) & ( ~( GPIO_CRL_CNF3_0 | GPIO_CRL_CNF3_1));
		GPIOC->CRL = (GPIOC->CRL) & ( ~( GPIO_CRL_CNF4_0 | GPIO_CRL_CNF4_1));
		GPIOC->CRL = (GPIOC->CRL) & ( ~( GPIO_CRL_CNF5_0 | GPIO_CRL_CNF5_1));
		GPIOC->CRL = (GPIOC->CRL) & ( ~( GPIO_CRL_CNF6_0 | GPIO_CRL_CNF6_1));
		GPIOC->CRL = (GPIOC->CRL) & ( ~( GPIO_CRL_CNF7_0 | GPIO_CRL_CNF7_1));
		GPIOC->CRH = (GPIOC->CRH) & ( ~( GPIO_CRH_CNF8_0 | GPIO_CRH_CNF8_1));

/* ????????? ???????? CRL ????? CNF [1:0] ?? ??? ?????  pull-up */
	
	GPIOA->CRL &= (~GPIO_CRL_CNF0_0);
  GPIOA->CRL |= GPIO_CRL_CNF0_1;
	
	GPIOA->CRL &= (~GPIO_CRL_CNF4_0);
  GPIOA->CRL |= GPIO_CRL_CNF4_1;
	
	GPIOA->CRL &= (~GPIO_CRL_CNF6_0);
  GPIOA->CRL |= GPIO_CRL_CNF6_1;
	
	GPIOA->CRL &= (~GPIO_CRL_CNF7_0);
  GPIOA->CRL |= GPIO_CRL_CNF7_1;
	
	GPIOA->CRH &= (~GPIO_CRH_CNF8_0);
  GPIOA->CRH |= GPIO_CRH_CNF8_1;
	
	GPIOA->CRH &= (~GPIO_CRH_CNF9_0);
  GPIOA->CRH |= GPIO_CRH_CNF9_1;
	
	GPIOA->CRH &= (~GPIO_CRH_CNF10_0);
  GPIOA->CRH |= GPIO_CRH_CNF10_1;
	
	GPIOA->CRH &= (~GPIO_CRH_CNF12_0);
  GPIOA->CRH |= GPIO_CRH_CNF12_1;

	GPIOC->CRH = (GPIOC->CRH) & ( ~( GPIO_CRH_MODE13_0 | GPIO_CRH_MODE13_1));
	GPIOC->CRH = ((GPIOC->CRH) | ( GPIO_CRH_CNF13_0 | GPIO_CRH_MODE13_1));
	GPIOC->ODR = ((GPIOC->ODR) | ( GPIO_PIN_13));
	interrupt_button();
while (1)
{
	 if (button_click ==1)
	 {
		 array = code_read();
		 array1=code_counterk();
		 array2=code_countertpwm();
		 array3=code_countertmax();
		 array4=code_counterper();
		 ////
		 GPIOC->BRR = GPIOC->BRR | 0x0000FF;//это прерывание 
     run = true;	 
		 modulation ();// сейчас работает,удалось увидеть влавногоснущий огонь?? нет, ничего не горит вообще. Тогда желаю вам удачи :)спасибо))
		 //
	 }
	 else if (button_click ==2)
	 {
	 run = false;
   button_click = 0;		 
   }
}
}
void modulation (void)
{
     if (run == true& time >= tmax)
		 {
    LED1_ON();LED2_ON();LED3_ON();LED4_ON();LED5_ON();LED6_ON();LED7_ON();LED8_ON();
			 delay(k*time);
		LED1_OFF();LED2_OFF();LED3_OFF();LED4_OFF();LED5_OFF();LED6_OFF();LED7_OFF();LED8_OFF();	 
			 delay(per-(k*time));			 
		 }
			if(time >= tmax)
		 {
			 time=0;
		 }
			else 
		 {
			 time=time+tpwm;
		 }
}
uint32_t code_counterk(void)
{
	k=20/(24*((float)bright_end+1));
	return k; 
}
uint32_t code_counterper(void)
{
	per=20/(10*((float)bright_end+1));
	return per; 
}
uint32_t code_countertpwm(void)
{
	tpwm=1/(24*((float)bright_end+1));
	return tpwm; 
}
uint32_t code_countertmax(void)
{
	tmax=20/(bright_end+1); //это расчет времени максимального до которого будет модуляция, я все параметры расчитывала при загрузке,потом перписала их, чтобы сбросить 
	return tmax; 
}
void interrupt_button(void)
{
	RCC->APB2ENR = RCC->APB2ENR | RCC_APB2ENR_AFIOEN;
	AFIO->EXTICR[3] = 0x0020;

	EXTI-> IMR|=(EXTI_IMR_MR13);
	EXTI-> EMR|=(EXTI_EMR_MR13);
	EXTI->RTSR|=(EXTI_RTSR_TR13);
	NVIC->ISER[1] = (uint32_t)(1UL << (((uint32_t)EXTI15_10_IRQn) & 0x1FUL));
}


void EXTI15_10_IRQHandler (void)		//?????????? ?????????? ?? ????? EXTI15_10
{
	
	EXTI->PR |= GPIO_PIN_13;
	button_click =button_click+1;
}

uint32_t code_read (void)
{
	
	bright_end |= PIN_CODE_0;
	bright_end |= PIN_CODE_1;
	bright_end |= PIN_CODE_2;
	bright_end |= PIN_CODE_3;
	bright_end |= PIN_CODE_4;
	bright_end |= PIN_CODE_5;
	bright_end |= PIN_CODE_6;
	bright_end |= PIN_CODE_7;
	return bright_end;
}

void delay (uint32_t ticks)
{
	for (uint32_t i = 0; i < ticks*100; i++) {}
}