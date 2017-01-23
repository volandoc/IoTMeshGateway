#include "espressif/esp_common.h"
#include "esp/uart.h"

#include <string.h>

#include <FreeRTOS.h>
#include <task.h>
#include <ssid_config.h>

#include <espressif/esp_sta.h>
#include <espressif/esp_wifi.h>

#include <paho_mqtt_c/MQTTESP8266.h>
#include <paho_mqtt_c/MQTTClient.h>

#include <semphr.h>
#include "jsmn.h"

#include "embgw_config.h"
#include "hw_config.h"

/* You can use http://test.mosquitto.org/ to test mqtt_client instead
 * of setting up your own MQTT server */

SemaphoreHandle_t wifi_alive;
QueueHandle_t publish_queue;
QueueHandle_t gpio_queue;

char sub_topic[64];

const property_t led_property = {.name="LED", .type="string", .descr="Led indicator", .lim_type="enum", .lim_json="[\"on\",\"off\"]", .deflt="\"off\"", .rw="rw"};
const property_t pir_property = {.name="PIR", .type="int", .descr="Motion sensor", .lim_type="", .lim_json="null", .deflt="0", .rw="r"};
const property_t mic_property = {.name="MIC", .type="int", .descr="Noise detector", .lim_type="", .lim_json="null", .deflt="0", .rw="r"};


irq_pin_t     PIR = {.last = 0, .prev = 0, .gpio = GPIO_PIR};
irq_pin_t     MIC = {.last = 0, .prev = 0, .gpio = GPIO_MIC};
control_pin_t LED = {.time_on = 0, .gpio = GPIO_LED};

#define PIR_HANDLER gpio05_interrupt_handler
#define MIC_HANDLER gpio04_interrupt_handler
#define LED_ACTIVE (0)
#define LED_TIMEOUT (15000)

const char *property_template = "{\"name\":\"%s\","\
"\"type\":\"%s\","\
"\"descr\":\"%s\","\
"\"lim_type\":\"%s\","\
"\"lim_json\":%s,"\
"\"default\":%s,"\
"\"rw\":\"%s\""\
"}";

const char *status_template = "{\"status\":\"%s\","\
"\"data\":%s,"\
"\"time\":%ld,"\
"\"err\":\"%s\""\
"}";

const char *event_template = "%s:%ld";


typedef struct
{
    char *msg;
    char *topic;
}queue_buf_t;

queue_buf_t pub_msg;

void PIR_HANDLER(uint8_t gpio_num)
{

    printf("interrupt gpio05 occurred\n");
    if(xQueueSendToBackFromISR(gpio_queue, &PIR, NULL)== pdFALSE)
        printf("Publish queue overflow.\r\n");

}

void MIC_HANDLER(uint8_t gpio_num)
{
    uint32_t delta;
    MIC.last = xTaskGetTickCountFromISR();
    delta = MIC.last - MIC.prev;
    if(delta > 100)
    {
        printf("interrupt gpio04 occurred\n");
        MIC.prev = MIC.last;
        if(xQueueSendToBackFromISR(gpio_queue, &MIC, NULL)== pdFALSE)
            printf("Publish queue overflow.\r\n");
    }
    else
    {
        printf("Jitter gpio04 detect exit\n");
    }

}

static int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
    if (tok->type == JSMN_STRING && (int) strlen(s) == tok->end - tok->start &&
            strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
        return 0;
    }
    return -1;
}

static void execute_command(char * cmd) {
    if (!strncmp(cmd, "on", 2)) {
        printf("Turning on LED\r\n");
        gpio_write(LED.gpio, 0);
        LED.time_on = xTaskGetTickCount();
        printf("led on time %d\n", LED.time_on);
    } else if (!strncmp(cmd, "off", 3)) {
        printf("Turning off LED\r\n");
        gpio_write(LED.gpio, 1);
    }
}

static void parse_command(char *command, size_t cmdsize) {
    int i;
    int res;
    jsmn_parser p;
    jsmntok_t tokens[128]; /* We expect no more than 128 tokens */
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
        } else if (jsoneq(command, &tokens[i], "PIR") == 0) {
            /* We may use strndup() to fetch string value */
            printf("- PIR: %.*s\n", tokens[i+1].end-tokens[i+1].start,
                    command + tokens[i+1].start);
            i++;
        } else if (jsoneq(command, &tokens[i], "MIC") == 0) {
            /* We may use strndup() to fetch string value */
            printf("- PIR: %.*s\n", tokens[i+1].end-tokens[i+1].start,
                    command + tokens[i+1].start);
            i++;
        } else if ((jsoneq(command, &tokens[i], "LED") == 0) && ((jsoneq(command, &tokens[i+1], "on") == 0) || (jsoneq(command, &tokens[i+1], "off") == 0))) {
            /* We may use strndup() to fetch string value */
            printf("- value: %.*s\n", tokens[i+1].end-tokens[i+1].start,
                    command + tokens[i+1].start);
            snprintf(cmd, 4, "%.*s", tokens[i+1].end-tokens[i+1].start,
                    command + tokens[i+1].start);
            execute_command(cmd);
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
            printf("Unexpected key: %.*s\n", tokens[i].end-tokens[i].start,
                    command + tokens[i].start);
        }
    }
}


static void topic_received(mqtt_message_data_t *md) {
    int i;
    mqtt_message_t *message = md->message;
    printf("Received: ");
    for( i = 0; i < md->topic->lenstring.len; ++i)
        printf("%c", md->topic->lenstring.data[ i ]);

    printf(" = ");
    for( i = 0; i < (int)message->payloadlen; ++i)
        printf("%c", ((char *)(message->payload))[i]);
    printf("\r\n");

    parse_command((char *)(message->payload), (int)message->payloadlen);
}

static const char * get_my_id(void) {
    // Use MAC address for Station as unique ID
    static char my_id[13];
    static bool my_id_done = false;
    int8_t i;
    uint8_t x;
    if (my_id_done)
        return my_id;
    if (!sdk_wifi_get_macaddr(STATION_IF, (uint8_t *)my_id))
        return NULL;
    for (i = 5; i >= 0; --i) {
        x = my_id[i] & 0x0F;
        if (x > 9) x += 7;
        my_id[i * 2 + 1] = x + '0';
        x = my_id[i] >> 4;
        if (x > 9) x += 7;
        my_id[i * 2] = x + '0';
    }
    my_id[12] = '\0';
    my_id_done = true;
    return my_id;
}

static void  mqtt_task(void *pvParameters) {
    int ret = 0;
    struct mqtt_network network;
    mqtt_client_t client = mqtt_client_default;
    char mqtt_client_id[20];
    uint8_t mqtt_buf[511];
    uint8_t mqtt_readbuf[511];
    mqtt_packet_will_options_t will = mqtt_packet_will_options_initializer;
    mqtt_packet_connect_data_t data = mqtt_packet_connect_data_initializer;
    queue_buf_t pub_msg_loc;
    TickType_t xLastWakeTime;

    char will_topic[64];
    sprintf(will_topic, MQTT_WILL_TOPIC, get_my_id());
    
    will.topicName.cstring = will_topic;
    will.message.cstring = MQTT_WILL_MSG;
    will.retained = 1;
    will.qos = MQTT_QOS1;

    mqtt_network_new( &network );
    memset(mqtt_client_id, 0, sizeof(mqtt_client_id));
    strcpy(mqtt_client_id, "ESP-");
    strcat(mqtt_client_id, get_my_id());

    data.willFlag = 1;
    data.will = will;
    data.MQTTVersion = 3;
    data.clientID.cstring = mqtt_client_id;
    data.username.cstring = MQTT_USER;
    data.password.cstring = MQTT_PASS;
    data.keepAliveInterval = 10;
    data.cleansession = 0;

    while(1) {
        vTaskDelay( 1000 / portTICK_PERIOD_MS );
        xSemaphoreTake(wifi_alive, portMAX_DELAY);
        printf("%s: started\n\r", __func__);
        printf("%s: (Re)connecting to MQTT server %s ... ",__func__,
               MQTT_HOST);
        ret = mqtt_network_connect(&network, MQTT_HOST, MQTT_PORT);
        if( ret ) {
            printf("error: %d\n\r", ret);
            taskYIELD();
            continue;
        }
        printf("done\n\r");
        mqtt_client_new(&client, &network, 5000, mqtt_buf, 511,
                      mqtt_readbuf, 511);
        printf("Send MQTT connect ... ");
        ret = mqtt_connect(&client, &data);
        if(ret){
            printf("error: %d\n\r", ret);
            mqtt_network_disconnect(&network);
            taskYIELD();
            continue;
        }

        char tmp_property[PUB_MSG_LEN - 1] = "\0";
        char properties[PUB_MSG_LEN - 1] = "\0";
        strcat(properties,"[");
        sprintf(tmp_property, property_template, led_property.name, led_property.type, led_property.descr, led_property.lim_type, led_property.lim_json, led_property.deflt, led_property.rw);
        strcat(properties,tmp_property);
        strcat(properties,",");
        sprintf(tmp_property, property_template, pir_property.name, pir_property.type, pir_property.descr, pir_property.lim_type, pir_property.lim_json, pir_property.deflt, pir_property.rw);
        strcat(properties,tmp_property);
        strcat(properties,",");
        sprintf(tmp_property, property_template, mic_property.name, mic_property.type, mic_property.descr, mic_property.lim_type, mic_property.lim_json, mic_property.deflt, mic_property.rw);
        strcat(properties,tmp_property);
        strcat(properties,"]");

        char msg[PUB_MSG_LEN - 1] = "\0";
        xLastWakeTime = xTaskGetTickCount();
        sprintf(msg, status_template, "available", properties, xLastWakeTime,"null");
        printf("%s: connection message is %s \n with length %d ",__func__, msg, strlen(msg));
        
        mqtt_message_t init_message;
        init_message.payload = msg;
        init_message.payloadlen = strlen(msg);
        init_message.dup = 0;
        init_message.qos = MQTT_QOS1;
        init_message.retained = 1;
        ret = mqtt_publish(&client, will_topic, &init_message);
        if (ret != MQTT_SUCCESS ){
            printf("error while send status message: %d\n", ret );
        }

        printf("done\r\n");
        sprintf(sub_topic, MQTT_SUB_TOPIC, get_my_id());
        mqtt_subscribe(&client, sub_topic, MQTT_QOS1, topic_received);

        xQueueReset(publish_queue);

        while(1){
            while(xQueueReceive(publish_queue, &pub_msg_loc, 0) == pdTRUE){
                printf("got message to publish\r\n");
                mqtt_message_t message;
                message.payload = pub_msg_loc.msg;
                message.payloadlen = strlen(pub_msg_loc.msg);
                message.dup = 0;
                message.qos = MQTT_QOS1;
                message.retained = 0;
                ret = mqtt_publish(&client, pub_msg_loc.topic, &message);
                free(pub_msg_loc.msg);
                free(pub_msg_loc.topic);
                if (ret != MQTT_SUCCESS ){
                    printf("error while publishing message: %d\n", ret );
                    break;
                }
            }

            if(gpio_read(LED.gpio) == LED_ACTIVE)
            {
                if((xTaskGetTickCount() - LED.time_on) > LED_TIMEOUT)
                {
                    gpio_write(LED.gpio, 1);
                }
            }


            ret = mqtt_yield(&client, 1000);
            if (ret == MQTT_DISCONNECTED)
                break;
        }
        printf("Connection dropped, request restart\n\r");
        mqtt_network_disconnect(&network);
        taskYIELD();
    }
}

static void  wifi_task(void *pvParameters)
{
    uint8_t status = 0;
    uint8_t retries = 30;
    struct sdk_station_config config = {
        .ssid = WIFI_SSID,
        .password = WIFI_PASS,
    };

    printf("WiFi: connecting to WiFi\n\r");
    sdk_wifi_set_opmode(STATION_MODE);
    sdk_wifi_station_set_config(&config);

    while(1) {
        while ((status != STATION_GOT_IP) && (retries)) {
            status = sdk_wifi_station_get_connect_status();
            printf("%s: status = %d\n\r", __func__, status );
            if( status == STATION_WRONG_PASSWORD ) {
                printf("WiFi: wrong password\n\r");
                break;
            } else if( status == STATION_NO_AP_FOUND ) {
                printf("WiFi: AP not found\n\r");
                break;
            } else if( status == STATION_CONNECT_FAIL ) {
                printf("WiFi: connection failed\r\n");
                break;
            }
            vTaskDelay( 1000 / portTICK_PERIOD_MS );
            --retries;
        }
        if (status == STATION_GOT_IP) {
            printf("WiFi: Connected\n\r");
            xSemaphoreGive( wifi_alive );
            taskYIELD();
        }

        while ((status = sdk_wifi_station_get_connect_status()) == STATION_GOT_IP) {
            xSemaphoreGive( wifi_alive );
            taskYIELD();
        }
        printf("WiFi: disconnected\n\r");
        sdk_wifi_station_disconnect();
        vTaskDelay( 1000 / portTICK_PERIOD_MS );
    }
}

void gpio_task(void *pvParameters)
{
    gpio_set_interrupt(GPIO_PIR, GPIO_INTTYPE_EDGE_POS, PIR_HANDLER);
    gpio_set_interrupt(GPIO_MIC, GPIO_INTTYPE_EDGE_POS, MIC_HANDLER);

    TickType_t xLastWakeTime = xTaskGetTickCount();

    for(;;)
    {
        irq_pin_t gpio_pin;
        xQueueReceive(gpio_queue, &gpio_pin, portMAX_DELAY);
        xLastWakeTime = xTaskGetTickCount();
        char *msg = malloc(256);
        char *topic = malloc(64);

        if(msg == NULL)
        {
            printf("[ERROR allocate memory\n]");
            return;
        }

        if(gpio_pin.gpio == GPIO_PIR)
        {
            sprintf(msg, event_template, "1", xLastWakeTime);
            sprintf(topic, MQTT_PUB_TOPIC, get_my_id(), "PIR");

        }
        else if (gpio_pin.gpio == GPIO_MIC)
        {
            sprintf(msg, event_template, "1", xLastWakeTime);
            sprintf(topic, MQTT_PUB_TOPIC, get_my_id(), "MIC");
        }
        else
        {
            printf("unidentified sensor\n");
            free(msg);
            free(topic);
        }

        pub_msg.msg = msg;
        pub_msg.topic = topic;

        if (xQueueSend(publish_queue, &pub_msg, 0) == pdFALSE)
            printf("Publish queue overflow.\r\n");

    }
}


void user_init(void)
{
    uart_set_baud(0, 115200);
    printf("SDK version:%s\n", sdk_system_get_sdk_version());

    gpio_enable(GPIO_LED, GPIO_OUTPUT);
    gpio_enable(GPIO_PIR, GPIO_INPUT);
    gpio_enable(GPIO_MIC, GPIO_INPUT);
    gpio_write(GPIO_LED, 1);

    vSemaphoreCreateBinary(wifi_alive);

    publish_queue = xQueueCreate(3, sizeof(queue_buf_t));
    gpio_queue = xQueueCreate(3, sizeof(irq_pin_t));

    xTaskCreate(&wifi_task, "wifi_task",  256, NULL, 2, NULL);
    xTaskCreate(&gpio_task, "gpio_task", 256, NULL, 3, NULL);
    xTaskCreate(&mqtt_task, "mqtt_task", 1024, NULL, 4, NULL);
}
