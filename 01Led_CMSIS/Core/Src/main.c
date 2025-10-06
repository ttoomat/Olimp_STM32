// 7 October 2025
#include <inttypes.h>
#include "stm32g030xx.h"

GPIO_TypeDef * led_port = GPIOA;
uint32_t led_pin = 2;

int main() {
	RCC->IOPENR |= RCC_IOPENR_GPIOAEN;
	led_port->MODER &= ~GPIO_MODER_MODE2_Msk; // ~(1 << 5);
	led_port->MODER |= GPIO_MODER_MODE2_0; // (1 << 4);
	while (1) {
		led_port->ODR |= (1 << led_pin);
		for (uint32_t i = 0; i < 1000000; i++);
		led_port->ODR &= ~(1 << led_pin);
		for (uint32_t i = 0; i < 1000000; i++);
	}
}
