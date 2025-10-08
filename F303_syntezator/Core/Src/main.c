/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */



void TIMER2_setup(){
	// HSI -> AHB -> APB1 presc-> TIM2EN
	RCC->APB1ENR|= RCC_APB1ENR_TIM2EN;
	/*%%%%%%%%% TIMER SETTINGS  %%%%%%%%%*/
	// Prescaler and Auto-reload reg. Settings
	TIM2->PSC=7999;
	//TIM2->ARR=1000-1;
	int freq = 110;
	TIM2->ARR=(1000-1)/freq;
	//TIM2->EGR|=TIM_EGR_UG;

	// Update Interrupt
	TIM2->DIER |= TIM_DIER_UIE;

	/*%%%%%%%%% TIMER ENABLE  %%%%%%%%%*/
	TIM2->CR1 |= TIM_CR1_CEN;
	TIM2->CR1 |= TIM_CR1_ARPE;

	NVIC_EnableIRQ(TIM2_IRQn);
}

void pin_output_mode(GPIO_TypeDef *GPIO, int n){
	int mask = ~((0b11)<<n*2);
	GPIO->MODER &= mask;
	GPIO->MODER |= (1<<n*2);
}

void pin_input_mode(GPIO_TypeDef *GPIO, int n){
	int mask = ~((0b11)<<n*2);
	GPIO->MODER &= mask;
}

void  turn_on_off(GPIO_TypeDef *GPIO, int n, int state){
	if (state == 1){
		GPIO->ODR|= (1<<n);
	} else if (state == 0){
		GPIO->ODR&=~(1<<n);
	}
	return;
}

int get_input(GPIO_TypeDef *gpio,int n){
	return (gpio -> IDR & (1<<n))>>n;
}

int i = 0;
void TIM2_IRQHandler(void){
	TIM2->SR &= ~(TIM_SR_UIF_Msk);
	//if (get_input(GPIOA, 0)){
		if (i%2==0){
			turn_on_off(GPIOE, 8, 1);
			turn_on_off(GPIOA, 1, 1);
		} else {
			turn_on_off(GPIOE, 8, 0);
			turn_on_off(GPIOA, 1, 0);
		}
	//}
	i+=1;
}

/**
 * Надо не через прерывания процессора для каждого канала, а связать ножку с таймером.
 */


uint8_t button_state = 0;
uint32_t prev_button = 0;
int main(void)
{

	RCC->AHBENR|=RCC_AHBENR_GPIOAEN;
	RCC->AHBENR|=RCC_AHBENR_GPIOEEN;
	pin_input_mode(GPIOA, 0);
	// кнопка
	pin_input_mode(GPIOA, 0);
	// другое
	pin_output_mode(GPIOE, 8);
	pin_output_mode(GPIOA, 1);

	turn_on_off(GPIOE, 8, 1);
	turn_on_off(GPIOA, 1, 1);

	TIMER2_setup();
	//turn_on_off(GPIOA, 2, 1);

	// отключили прерывания
	TIM2->DIER &= ~TIM_DIER_UIE;
  while (1)
  {
	  if (get_input(GPIOA, 0) == 1) {
		  TIM2->DIER |= TIM_DIER_UIE;
	  } else {
		  TIM2->DIER &= ~TIM_DIER_UIE;
	  }
	  /*
	  // Надо менять состояние кнопки только при отпускании
	  if (get_input(GPIOA, 0) == 0 && prev_button == 1) {
		  if (button_state) {
			  TIM2->DIER |= TIM_DIER_UIE;
			  button_state = (button_state + 1) % 2;
		  } else {
			  TIM2->DIER &= ~TIM_DIER_UIE;
		  }
		  prev_button = 0;
	  }
	  */
  }
}










