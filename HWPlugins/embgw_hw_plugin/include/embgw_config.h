#ifndef __EMBGW_CONFIG_H__
#define __EMBGW_CONFIG_H__

#define MQTT_HOST ("192.168.1.1")
#define MQTT_PORT 1883

#define MQTT_PUB_TOPIC "gw/Esp8266Plugin/event"
#define MQTT_SUB_TOPIC "gw/Esp8266Plugin/command"
#define MQTT_WILL_TOPIC "gw/Esp8266Plugin/status"
#define MQTT_WILL_MSG "disconected"

#define GPIO_LED 2

#define MQTT_USER NULL
#define MQTT_PASS NULL
#define PUB_MSG_LEN 32

#endif // __EMBGW_CONFIG_H__