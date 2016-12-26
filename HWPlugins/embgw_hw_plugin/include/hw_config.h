/*
 * hw_config.h
 *
 *  Created on: 20 Dec 2016
 *      Author: maksym
 */

#ifndef HW_CONFIG_H_
#define HW_CONFIG_H_

#define GPIO_LED 2
#define GPIO_PIR 5
#define GPIO_MIC 4

typedef struct
{
	uint32_t last;
	uint32_t prev;
	uint32_t gpio;
}irq_pin_t;

typedef struct
{
	uint32_t time_on;
	uint32_t gpio;
}control_pin_t;


#endif /* EXAMPLES_EMBGW_HW_PLUGIN_INCLUDE_HW_CONFIG_H_ */
