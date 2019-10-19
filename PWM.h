/*
 * PWM.h
 *
 *  Created on: Sep 26, 2019
 *      Author: MCLEANS
 */

#ifndef PWM_H_
#define PWM_H_

#include "stm32f10x.h"
#include "stdlib.h"
#include <string.h>

namespace custom_drivers {

enum  counter_direction{
	upcounter ,
	downcounter
};

enum alignment_mode{
	edge_aligned_mode,
	center_aligned_mode_1,
	center_aligned_mode_2,
	center_aligned_mode_3
};

enum output_compare_mode{
	mode_1 = 1,
	mode_2 = 2
};

enum timer_polarity{
	active_high,
	active_low
};

class PWM {
private:
	TIM_TypeDef * TIMER ;
	uint8_t channel ;
	GPIO_TypeDef *PORT;
	uint8_t PIN;
	uint16_t prescaller = 0;
	uint16_t duty_cycle = 0;
	uint16_t reload_register = 0;
	uint16_t counter_value = 0;


private:
		void enable_timer_RCC(TIM_TypeDef * TIMER);
		void enable_port_RCC(GPIO_TypeDef *PORT);
		void enable_pin_alternate_output_function(GPIO_TypeDef *PORT , uint8_t PIN);
		void set_timer_direction(TIM_TypeDef * TIMER , uint8_t direction);
		void enable_timer(TIM_TypeDef * TIMER);
		void set_alignment_mode(TIM_TypeDef * TIMER, uint8_t mode);
		void set_output_compare_mode(TIM_TypeDef * TIMER, uint8_t channel ,uint8_t mode);
		void set_channel_polarity_and_enable(TIM_TypeDef * TIMER, uint8_t channel , timer_polarity polarity);
		void set_prescaller_ARR_and_dutycycle(TIM_TypeDef * TIMER,
												uint16_t prescaller,
												uint16_t ARR,
												uint16_t duty_cycle);

public:
public:
	PWM(TIM_TypeDef * TIMER,
			uint8_t channel,
			GPIO_TypeDef *PORT,
			uint8_t PIN);

				//APPLICATION FUNCTIONS//
	void set_timer_modes_polarity_and_direction(counter_direction direction,
										alignment_mode mode,
										output_compare_mode output_mode,
										timer_polarity polarity);

	void set_prescaller_ARR_and_duty_cycle(uint16_t prescaller,
											uint16_t ARR,
											uint16_t duty_cycle);

	void set_duty_cycle(uint16_t new_duty_cycle);
	uint16_t get_counter_value();

	~PWM();
};

} /* namespace custom_drivers */

#endif /* PWM_H_ */
