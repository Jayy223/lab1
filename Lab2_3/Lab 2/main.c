#include "main.h"
void init_tim_gpio(void){
RCC -> AHBENR |= RCC_AHBENR_GPIOEEN;

GPIOE -> MODER |= GPIO_MODER_MODER9_1;
GPIOE -> MODER &=~ GPIO_MODER_MODER9_0;
GPIOE -> AFR[1] |=(1<<5);
GPIOE -> OSPEEDR |=GPIO_OSPEEDER_OSPEEDR9_0;
GPIOE -> OSPEEDR |=GPIO_OSPEEDER_OSPEEDR9_1;
}
void init_tim(int duty){
	RCC -> APB2ENR |= RCC_APB2ENR_TIM1EN;
	TIM1 -> CCMR1 |= TIM_CCMR1_OC1PE;
	TIM1-> PSC = 0x00F;
	TIM1 -> CCMR1 &=~ TIM_CCMR1_OC1M_0;
	TIM1 -> CCMR1 |= TIM_CCMR1_OC1M_1;
	TIM1 -> CCMR1 |= TIM_CCMR1_OC1M_2;
	TIM1 -> CCMR1 &=~ TIM_CCMR1_OC1M_3;
	TIM1 ->ARR =1000;
	TIM1 -> CCR1=duty;
	TIM1->BDTR |=TIM_BDTR_MOE;
	TIM1->CCER |=TIM_CCER_CC1E;
	TIM1->CR1 |= TIM_CR1_CEN;
}
void EXTI0_config(void){
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	SYSCFG -> EXTICR[0] &=~ SYSCFG_EXTICR1_EXTI0;
	SYSCFG ->EXTICR[0] |= SYSCFG_EXTICR1_EXTI0_PA;
	EXTI -> RTSR &=~ EXTI_RTSR_TR0;
	EXTI -> FTSR |=EXTI_FTSR_TR0;
	EXTI -> IMR |=EXTI_IMR_MR0;
	NVIC_EnableIRQ(EXTI0_IRQn);
}
void EXTI0_IRQHandler(void){
	if(EXTI -> PR & EXTI_PR_PR0){
		EXTI -> PR |= EXTI_PR_PR0;
		TIM1 -> CCR1 += TIM1 -> ARR/10;
	}
}
int main(void){
	init_tim_gpio();
	init_tim(100);
	EXTI0_config();

while(1){
	int b = 0;
	while(b<1000){
		init_tim(b);
		b++;
		for(int i=0; i<500;i++);
	}
if(b>=100){
	while(b>0){
		init_tim(b);
		b--;
		for(int i=0; i<500;i++);
	}
}
}}
