/*
 * hw_config.h
 *
 *  Created on: 20 Dec 2016
 *      Author: maksym
 */

#ifndef HW_CONFIG_H_
#define HW_CONFIG_H_

#define HW_DEV_COUNT 3
#define HW_PROP_COUNT 5

#define GPIO_LED 2
#define GPIO_PIR 5
#define GPIO_MIC 4

#define PIR_HANDLER gpio05_interrupt_handler
#define MIC_HANDLER gpio04_interrupt_handler
#define LED_ACTIVE (0)
#define LED_TIMEOUT (15000)

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

typedef struct
{
	char *name;
	char *type;
	char *descr;
	char *lim_type;
	char *lim_json;
	char *deflt;
	char *rw;
}property_t;

typedef struct
{
	char *name;
	property_t *properties;
	uint8_t prop_count;
}device_t;

irq_pin_t     PIR = {.last = 0, .prev = 0, .gpio = GPIO_PIR};
irq_pin_t     MIC = {.last = 0, .prev = 0, .gpio = GPIO_MIC};
control_pin_t LED = {.time_on = 0, .gpio = GPIO_LED};

const property_t hw_properties[HW_PROP_COUNT] = {
	{.name="SSID", .type="string", .descr="WiFi Name", .lim_type="null", .lim_json="null", .deflt="\"VCH-Simulator\"", .rw="\"rw\""},
	{.name="SSIDPassword", .type="string", .descr="WiFi password", .lim_type="null", .lim_json="null", .deflt="\"12345678\"", .rw="\"rw\""},
	{.name="GWID", .type="string", .descr="Gateway ID", .lim_type="null", .lim_json="null", .deflt="A000000000000777", .rw="r"},
	{.name="MQTTAddr", .type="string", .descr="MQTT Address", .lim_type="null", .lim_json="null", .deflt="192.168.1.1", .rw="r"},
	{.name="restart", .type="int", .descr="Restart", .lim_type="range", .lim_json="[0, 1]", .deflt="0", .rw="w"},
};

property_t led_properties[1] = {{.name="power", .type="string", .descr="Led indicator", .lim_type="enum", .lim_json="[\"on\",\"off\"]", .deflt="\"off\"", .rw="\"rw\""}};
property_t pir_properties[1] = {{.name="motion", .type="int", .descr="Motion sensor", .lim_type="", .lim_json="null", .deflt="0", .rw="\"r\""}};
property_t mic_properties[1] = {{.name="noise", .type="int", .descr="Noise detector", .lim_type="", .lim_json="null", .deflt="0", .rw="\"r\""}};


const device_t hw_devices[HW_DEV_COUNT] = {{
	.name="LED",
	.properties=led_properties,
	.prop_count = 1
},
{
	.name="PIR_SENSOR",
	.properties=pir_properties,
	.prop_count = 1
},
{
	.name="MIC_SENSOR",
	.properties=mic_properties,
	.prop_count = 1
}};

#endif /* EXAMPLES_EMBGW_HW_PLUGIN_INCLUDE_HW_CONFIG_H_ */
