#ifndef __EMBGW_CONFIG_H__
#define __EMBGW_CONFIG_H__

#define MQTT_HOST ("172.24.254.20") //DragonBoard
#define MQTT_PORT 1883

#define MQTT_PUB_TOPIC "A000000000000777/Esp8266Plugin/%s/%s/event"
#define MQTT_SUB_TOPIC "A000000000000777/Esp8266Plugin/%s/command"
#define MQTT_WILL_TOPIC "A000000000000777/Esp8266Plugin/%s/status"
#define MQTT_WILL_MSG "{\"status\":\"offline\", \"data\":null, \"time\":null, \"err\":\"Driver not running\"}"

#define MQTT_USER NULL
#define MQTT_PASS NULL
#define PUB_MSG_LEN 512

#endif // __EMBGW_CONFIG_H__
