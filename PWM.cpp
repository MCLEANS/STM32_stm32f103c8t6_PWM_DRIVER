/*
 * PWM.cpp
 *
 *  Created on: Sep 26, 2019
 *      Author: MCLEANS
 */

#include "PWM.h"

namespace custom_drivers {

PWM::PWM(TIM_TypeDef * TIMER,
		uint8_t channel,
		GPIO_TypeDef *PORT,
		uint8_t PIN):TIMER(TIMER),
					channel(channel),
					PORT(PORT),
					PIN(PIN){

	enable_timer_RCC(this->TIMER);
	enable_port_RCC(this->PORT);
	enable_pin_alternate_output_function(this->PORT , this->PIN);
	enable_timer(this->TIMER);


}

void PWM::enable_timer_RCC(TIM_TypeDef * TIMER){
	if(TIMER == TIM1) RCC->APB2ENR |= (1<<11);
	if(TIMER == TIM2) RCC->APB1ENR |= (1<<0);
	if(TIMER == TIM3) RCC->APB1ENR |= (1<<1);
	if(TIMER == TIM4) RCC->APB1ENR |= (1<<2);
	if(TIMER == TIM5) RCC->APB1ENR |= (1<<3);
}

void PWM::enable_port_RCC(GPIO_TypeDef *PORT){
	if(PORT == GPIOA) RCC->APB2ENR |= (1<<2);
	if(PORT == GPIOB) RCC->APB2ENR |= (1<<3);
	if(PORT == GPIOC) RCC->APB2ENR |= (1<<4);
	if(PORT == GPIOD) RCC->APB2ENR |= (1<<5);
	if(PORT == GPIOE) RCC->APB2ENR |= (1<<6);

}

void PWM::enable_pin_alternate_output_function(GPIO_TypeDef *PORT , uint8_t PIN){
	if(PIN < 8){
		PORT->CRL |= (1<<(PIN*4))|(1<<((PIN*4)+1))|(1<<((PIN*4)+3));
		PORT->CRL &= ~(1<<((PIN*4)+2));
	}
	else{
		PIN = PIN-8;
		PORT->CRH |= (1<<(PIN*4))|(1<<((PIN*4)+1))|(1<<((PIN*4)+3));
		PORT->CRH &= ~(1<<((PIN*4)+2));
	}
}

void PWM::set_timer_direction(TIM_TypeDef * TIMER , uint8_t direction){
	if(direction == upcounter) TIMER->CR1 &= ~(1<<4);
	if(direction == downcounter) TIMER->CR1 |= (1<<4);
}

void PWM::enable_timer(TIM_TypeDef * TIMER){
	TIMER->CR1 |= (1<<0);
}

void PWM::set_alignment_mode(TIM_TypeDef * TIMER, uint8_t mode){
	if(mode == edge_aligned_mode ) TIMER->CR1 &= ~((1<<5)|(1<<6));
	if(mode == center_aligned_mode_1) {
		TIMER->CR1 |= (1<<5);
		TIMER->CR1 &= ~(1<<6);
		}
	if(mode == center_aligned_mode_2){
		TIMER->CR1 &= ~(1<<5);
		TIMER->CR1 |= (1<<6);
	}
	if(mode == center_aligned_mode_3) TIMER->CR1 |= (1<<5)|(1<<6);
}

void PWM::set_output_compare_mode(TIM_TypeDef * TIMER, uint8_t channel ,uint8_t mode){
	if(channel == 1 ){
		if(mode == 1) {
			TIMER->CCMR1 &= ~(1<<4);
			TIMER->CCMR1 |= (1<<5)|(1<<6);
		}
		if(mode == 2){
			TIMER->CCMR1 |= (1<<4)|(1<<5)|(1<<6);
		}

		//set preload enable
		TIMER->CCMR1 |= (1<<3);
	}

	if(channel == 2){
		if(mode == 1) {
			TIMER->CCMR1 &= ~(1<<12);
			TIMER->CCMR1 |= (1<<13)|(1<<14);
		}
		if(mode == 2){
			TIMER->CCMR1 |= (1<<12)|(1<<13)|(1<<14);
		}

		//set preload enable
		TIMER->CCMR1 |= (1<<11);

	}

	if(channel == 3 ){
		if(mode == 1) {
			TIMER->CCMR2 &= ~(1<<4);
			TIMER->CCMR2 |= (1<<5)|(1<<6);
		}
		if(mode == 2){
			TIMER->CCMR2 |= (1<<4)|(1<<5)|(1<<6);
		}

		//set preload enable
		TIMER->CCMR2 |= (1<<3);
	}

	if(channel == 4){
		if(mode == 1) {
			TIMER->CCMR2 &= ~(1<<12);
			TIMER->CCMR2 |= (1<<13)|(1<<14);
		}
		if(mode == 2){
			TIMER->CCMR2 |= (1<<12)|(1<<13)|(1<<14);
		}

		//set preload enable
		TIMER->CCMR2 |= (1<<11);

	}

}

void PWM::set_channel_polarity_and_enable(TIM_TypeDef * TIMER, uint8_t channel, timer_polarity polarity){
	channel = channel-1;
	//set_polarity
	if(polarity == active_high) TIMER->CCER &= ~(1<<((channel*4)+1));
	if(polarity == active_low) TIMER->CCER |= (1<<((channel*4)+1));
	//enable output
	TIMER->CCER |= (1<<(channel*4));
}

void PWM::set_prescaller_ARR_and_dutycycle(TIM_TypeDef * TIMER,
		uint16_t prescaller,
		uint16_t ARR,
		uint16_t duty_cycle){

		//set prescaler
		TIMER->PSC = prescaller;
		//set  auto-reload register
		TIMER->ARR = ARR;
		//set duty cycle
		TIMER->CCR1 = duty_cycle;

		//required to complete configuration
		//enable the main output
		TIM1->BDTR |= (1<<15);

		//set the UG register
		TIM1->EGR |= (1<<1);
}

void PWM::set_timer_modes_polarity_and_direction(counter_direction direction,
		alignment_mode mode,
		output_compare_mode output_mode,
		timer_polarity polarity){

	set_timer_direction(this->TIMER ,direction);
	set_alignment_mode(this->TIMER, mode);
	set_output_compare_mode(this->TIMER,this->channel,output_mode);
	set_channel_polarity_and_enable(this->TIMER , this->channel , polarity);
}

void PWM::set_prescaller_ARR_and_duty_cycle(uint16_t prescaller,
												uint16_t ARR,
												uint16_t duty_cycle){
	this->prescaller = prescaller;
	this->duty_cycle = duty_cycle;
	this->reload_register = ARR;


	set_prescaller_ARR_and_dutycycle(this->TIMER,
										this->prescaller,
										this->reload_register,
										this->duty_cycle);

}

uint16_t PWM::get_counter_value(){
	counter_value = this->TIMER->CNT;
	return counter_value;
}

void PWM::set_duty_cycle(uint16_t new_duty_cycle){

			this->duty_cycle = new_duty_cycle;

	}

PWM::~PWM() {

}

} /* namespace custom_drivers */
