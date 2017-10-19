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

#define HW_DEV_LED 0
#define HW_DEV_PIR 1
#define HW_DEV_MIC 2

#define GPIO_LED 2
#define GPIO_PIR 5
#define GPIO_MIC 4

#define PIR_HANDLER pir_irq_handler
#define MIC_HANDLER mic_irq_handler
#define LED_HANDLER led_irq_handler

#define LED_ACTIVE (0)
#define LED_TIMEOUT (15000)

typedef struct
{
    char name[10];
    uint32_t last;
    uint32_t prev;
    uint8_t gpio;
    uint8_t value;
}irq_pin_t;

typedef struct
{
    char name[10];
    uint32_t time_on;
    uint8_t gpio;
    uint8_t value;
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

void properties_to_str(char * prop_buf, int buf_size, const property_t * properties, uint8_t prop_count){
    char tmp_property[192] = "\0";
    memset(prop_buf,0,buf_size);
    strcpy(prop_buf,"[");
    for (int i=0; i<prop_count; i++) {
        sprintf(tmp_property, property_template,\
                properties[i].name,\
                properties[i].type,\
                properties[i].descr,\
                properties[i].lim_type,\
                properties[i].lim_json,\
                properties[i].deflt,\
                properties[i].rw);
        strcat(prop_buf,tmp_property);
        if (i<(prop_count-1)) strcat(prop_buf,",");
    }
    strcat(prop_buf,"]");
}

typedef struct {
    uint8_t ssid[32];     /* Null terminated string */
    uint8_t password[64]; /* Null terminated string */
    uint8_t gwid[17];     /* Null terminated string */
    char mqttadr[16];  /* Null terminated string */
    uint16_t mqttport;
}plugin_properties_t;

typedef struct
{
    char *name;
    property_t *properties;
    uint8_t prop_count;
    char cmd_topic[64];
    mqtt_message_handler_t cmd_hndlr;
    irq_pin_t irq_pin;
    control_pin_t ctrl_pin;
    gpio_interrupt_handler_t irq_hndl;
}device_t;

/**
 * Capabilities available for ESP8266 Plugin
**/
const property_t hw_properties[HW_PROP_COUNT] = {
    {.name="SSID", .type="string", .descr="WiFi Name", .lim_type="null", .lim_json="null", .deflt="\"VCH-Simulator\"", .rw="\"rw\""},
    {.name="SSIDPassword", .type="string", .descr="WiFi password", .lim_type="null", .lim_json="null", .deflt="\"12345678\"", .rw="\"rw\""},
    {.name="GWID", .type="string", .descr="Gateway ID", .lim_type="null", .lim_json="null", .deflt="A000000000000777", .rw="r"},
    {.name="MQTTAddr", .type="string", .descr="MQTT Address", .lim_type="null", .lim_json="null", .deflt="192.168.1.1", .rw="rw"},
    {.name="restart", .type="int", .descr="Restart", .lim_type="range", .lim_json="[0, 1]", .deflt="0", .rw="w"},
};

plugin_properties_t plugin_prop={ .ssid = WIFI_SSID, .password = WIFI_PASS, .gwid = "A000000000000777", .mqttadr = MQTT_HOST, .mqttport = MQTT_PORT };

/**
 * Variables for devices initialization
**/
irq_pin_t     PIR = {.last = 0, .prev = 0, .gpio = GPIO_PIR};
irq_pin_t     MIC = {.last = 0, .prev = 0, .gpio = GPIO_MIC};
control_pin_t LED = {.time_on = 0, .gpio = GPIO_LED};

property_t led_properties[1] = {{.name="power", .type="int", .descr="Led indicator", .lim_type="enum", .lim_json="[\"on\",\"off\"]", .deflt="\"off\"", .rw="\"rw\""}};
property_t pir_properties[1] = {{.name="motion", .type="int", .descr="Motion sensor", .lim_type="null", .lim_json="null", .deflt="0", .rw="\"r\""}};
property_t mic_properties[1] = {{.name="noise", .type="int", .descr="Noise detector", .lim_type="null", .lim_json="null", .deflt="0", .rw="\"r\""}};

/**
 * definitions of handlers for mqtt messages 
 */
void led_cmd_hndlr(mqtt_message_data_t *md);
void pir_cmd_hndlr(mqtt_message_data_t *md);
void mic_cmd_hndlr(mqtt_message_data_t *md);

/**
 * definitions of  handlers for GPIO irq
**/
void LED_HANDLER(uint8_t gpio_num);
void PIR_HANDLER(uint8_t gpio_num);
void MIC_HANDLER(uint8_t gpio_num);

/**
 * All devices available in ESP8266
**/
device_t hw_devices[HW_DEV_COUNT] = {
/* Led attached to GPIO 2 and placed on ESP8266 PCB */
/* Has not irq pin                                  */
{
    .name="LED",
    .properties=led_properties,
    .prop_count = 1,
    .cmd_topic={0},
    .cmd_hndlr = led_cmd_hndlr,
    .irq_pin = {.name={0}, .last = 0, .prev = 0, .gpio = 0, .value=0},
    .ctrl_pin = {.name="power", .time_on = 0, .gpio = GPIO_LED, .value=1},
    .irq_hndl = LED_HANDLER
},
/* PIR SR501 Motion sensor attached to GPIO 5 */
/* has not control pin, but can be developed  */
{
    .name="PIR_SENSOR",
    .properties=pir_properties,
    .prop_count = 1,
    .cmd_topic={0},
    .cmd_hndlr = pir_cmd_hndlr,
    .irq_pin = {.name="motion", .last = 0, .prev = 0, .gpio = GPIO_PIR, .value=0},
    .ctrl_pin = {.name={0}, .time_on = 0, .gpio = 0, .value=0},
    .irq_hndl = PIR_HANDLER
},
/* Noise sensor based on LM393 amp.          */
/* has not control pin, but can be developed */
{
    .name="MIC_SENSOR",
    .properties=mic_properties,
    .prop_count = 1,
    .cmd_topic={0},
    .cmd_hndlr = mic_cmd_hndlr,
    .irq_pin = {.name="noise", .last = 0, .prev = 0, .gpio = GPIO_MIC, .value=0},
    .ctrl_pin = {.name={0}, .time_on = 0, .gpio = 0, .value=0},
    .irq_hndl = MIC_HANDLER
}
};

void publish_dev_event_or_state(char *cont, char * dev_name, char *prop_name){
    TickType_t xHndlrStartTime = xTaskGetTickCount();

    char *msg = malloc(256);
    char *topic = malloc(64);
    if(msg == NULL || topic == NULL)
    {
        printf("[ERROR allocate memory\n]");
        free(msg);
        free(topic);
    } else {
        sprintf(msg, event_template, cont, xHndlrStartTime);
        sprintf(topic, MQTT_DEV_PUB_TOPIC, get_my_id(), dev_name, prop_name);
        pub_msg.msg = msg;
        pub_msg.topic = topic;
        if (xQueueSend(publish_queue, &pub_msg, 0) == pdFALSE)
            printf("Publish queue overflow.\r\n");
    }
}

/**
 * In this section placed handlers for mqtt messages
 * All devices subscribed to its own topic and handle
 * messages arrived in this topic by own handler
**/
void led_cmd_hndlr(mqtt_message_data_t *md) {
    printf("Handle command for LED\n");
    bool success = false;
    publish_dev_event_or_state("ack", hw_devices[HW_DEV_LED].name, "event");

    int i;
    char * command = (char *)(md->message->payload);
    int cmdsize = (int)(md->message->payloadlen);

    int res;
    jsmn_parser p;
    jsmntok_t tokens[16]; /* We expect no more than 16 tokens */
    char cmd[4];
    jsmn_init(&p);
    res = jsmn_parse(&p, command, cmdsize, tokens, sizeof(tokens)/sizeof(tokens[0]));
    if (res < 0) {
        printf("Failed to parse JSON: %d\n", res);
        return;
    }
    /* Assume the top-level element is an object */
    if(res < 1 || tokens[0].type != JSMN_OBJECT) {
        printf("Object expected\n");
        return;
    }
    
    /* Loop over all keys of the root object */
    for (i = 1; i < res; i++) {
        if (jsoneq(command, &tokens[i], "data") == 0) {
            /* We may use strndup() to fetch string value */
            printf("- data: %.*s\n", tokens[i+1].end-tokens[i+1].start,
                    command + tokens[i+1].start);
            i++;
        } else if ((jsoneq(command, &tokens[i], "power") == 0) && ((jsoneq(command, &tokens[i+1], "on") == 0) || (jsoneq(command, &tokens[i+1], "off") == 0))) {
            /* We may use strndup() to fetch string value */
            printf("    {power: %.*s}\n", tokens[i+1].end-tokens[i+1].start,
                    command + tokens[i+1].start);
            snprintf(cmd, 4, "%.*s", tokens[i+1].end-tokens[i+1].start,
                    command + tokens[i+1].start);
            i++;
        } else if (jsoneq(command, &tokens[i], "time") == 0) {
            /* We may want to do strtol() here to get numeric value */
            printf("- time: %.*s\n", tokens[i+1].end-tokens[i+1].start,
                    command + tokens[i+1].start);
            i++;
        } else if (jsoneq(command, &tokens[i], "id") == 0) {
            /* We may want to do strtol() here to get numeric value */
            printf("- id: %.*s\n", tokens[i+1].end-tokens[i+1].start,
                    command + tokens[i+1].start);
            i++;
        } else if (jsoneq(command, &tokens[i], "ref") == 0) {
            /* We may want to do strtol() here to get numeric value */
            printf("- ref: %.*s\n", tokens[i+1].end-tokens[i+1].start,
                    command + tokens[i+1].start);
            i++;
        } else {
            printf("Unexpected command: %.*s\n", tokens[i].end-tokens[i].start,
                    command + tokens[i].start);
        }
    }

    if (!strncmp(cmd, "on", 2))
    {
        printf("Turning on LED\r\n");
        gpio_write(hw_devices[HW_DEV_LED].ctrl_pin.gpio, 0);
        hw_devices[HW_DEV_LED].ctrl_pin.time_on = xTaskGetTickCount();
        printf("led on time %d\n", hw_devices[HW_DEV_LED].ctrl_pin.time_on);
        hw_devices[HW_DEV_LED].ctrl_pin.value = 0;
        success = true;
        xSemaphoreGive(timer_start);
        publish_dev_event_or_state("on", hw_devices[HW_DEV_LED].name, hw_devices[HW_DEV_LED].ctrl_pin.name);
    }
    else if (!strncmp(cmd, "off", 3))
    {
        printf("Turning off LED\r\n");
        gpio_write(hw_devices[HW_DEV_LED].ctrl_pin.gpio, 1);
        hw_devices[HW_DEV_LED].ctrl_pin.value = 1;
        success = true;
        publish_dev_event_or_state("off", hw_devices[HW_DEV_LED].name, hw_devices[HW_DEV_LED].ctrl_pin.name);
    }

    if (success) {
        publish_dev_event_or_state("suc", hw_devices[HW_DEV_LED].name, "event");
    } else {
        publish_dev_event_or_state("err", hw_devices[HW_DEV_LED].name, "event");
    }
}

void pir_cmd_hndlr(mqtt_message_data_t *md) {
    printf("Handle command for PIR\n");
    publish_dev_event_or_state("ack", hw_devices[HW_DEV_PIR].name, "event");
    publish_dev_event_or_state("err: readonly", hw_devices[HW_DEV_PIR].name, "event");
    publish_dev_event_or_state(hw_devices[HW_DEV_PIR].irq_pin.value?"1":"0", hw_devices[HW_DEV_PIR].name, hw_devices[HW_DEV_PIR].irq_pin.name);
}

void mic_cmd_hndlr(mqtt_message_data_t *md) {
    printf("Handle command for MIC\n");
    publish_dev_event_or_state("ack", hw_devices[HW_DEV_MIC].name, "event");
    publish_dev_event_or_state("err: readonly", hw_devices[HW_DEV_MIC].name, "event");
    publish_dev_event_or_state(hw_devices[HW_DEV_MIC].irq_pin.value?"1":"0", hw_devices[HW_DEV_MIC].name, hw_devices[HW_DEV_MIC].irq_pin.name);
}

/**
 * In this section placed handlers for GPIO irq
 * All devices has GPIO pin
 * Changes in pin state (Rais, Fall, High, Low) handled
 * by this function.
 * Handler registered if .irq_pin greater than zero
**/

void LED_HANDLER(uint8_t gpio_num) {
    printf("Something went wrong. There is no spoon!!!\n");
}

void PIR_HANDLER(uint8_t gpio_num) {
    printf("interrupt PIR occurred\n");
    hw_devices[HW_DEV_PIR].irq_pin.value = gpio_read(hw_devices[HW_DEV_PIR].irq_pin.gpio);
    if(xQueueSendToBackFromISR(gpio_queue, &(hw_devices[HW_DEV_PIR].irq_pin), NULL)== pdFALSE)
        printf("Publish queue overflow.\r\n");

}

void MIC_HANDLER(uint8_t gpio_num) {
    uint32_t delta;
    hw_devices[HW_DEV_MIC].irq_pin.last = xTaskGetTickCountFromISR();
    delta = hw_devices[HW_DEV_MIC].irq_pin.last - hw_devices[HW_DEV_MIC].irq_pin.prev;
    if(delta > 100)
    {
        printf("interrupt MIC occurred\n");
        hw_devices[HW_DEV_MIC].irq_pin.prev = hw_devices[HW_DEV_MIC].irq_pin.last;
        hw_devices[HW_DEV_MIC].irq_pin.value = gpio_read(hw_devices[HW_DEV_MIC].irq_pin.gpio);
        if(xQueueSendToBackFromISR(gpio_queue, &(hw_devices[HW_DEV_MIC].irq_pin), NULL)== pdFALSE)
            printf("Publish queue overflow.\r\n");
    }
    else
    {
        printf("Jitter noise detect exit\n");
    }

}

#endif /* EXAMPLES_EMBGW_HW_PLUGIN_INCLUDE_HW_CONFIG_H_ */