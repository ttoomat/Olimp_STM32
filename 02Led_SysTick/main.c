#include <inttypes.h>
#include "stm32g030xx.h"

GPIO_TypeDef* led_port = GPIOA;
uint32_t led_pin = 2;

void GPIO_Init() {
  RCC->IOPENR |= RCC_IOPENR_GPIOAEN;
  led_port->MODER &= ~GPIO_MODER_MODE2_1; // ~(1 << 5);
  led_port->MODER |= GPIO_MODER_MODE2_0; // (1 << 4);
}

uint8_t timerFlag = 0;
void SysTick_Handler() {
  if (SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) {
    timerFlag = (timerFlag + 1) % 2;
  }
}

void SysTick_Init() {
  // до скольки он считает
  SysTick->LOAD = (uint32_t)(16000000-1);
  // internal clock
  SysTick->CTRL |= (1U << 2);
  // exception when counts down zero
  SysTick->CTRL |= (1U << 1);
  // Enable
  SysTick->CTRL |= (1U << 0);
  NVIC_EnableIRQ(SysTick_IRQn);
}

int main() {
  GPIO_Init();
  SysTick_Init();
  while (1) {
    if (timerFlag)
      led_port->ODR |= (1 << led_pin);
    else
      led_port->ODR &= ~(1 << led_pin);
  }
}
