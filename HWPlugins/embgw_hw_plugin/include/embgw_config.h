#ifndef __EMBGW_CONFIG_H__
#define __EMBGW_CONFIG_H__

#define MQTT_HOST ("54.173.78.1") //DragonBoard
#define MQTT_PORT 1883

#define MQTT_PUB_TOPIC "A000000000000777/Esp8266_%s/%s"
#define MQTT_SUB_TOPIC "A000000000000777/Esp8266_%s/command"
#define MQTT_WILL_TOPIC "A000000000000777/Esp8266_%s/status"
#define MQTT_DEV_PUB_TOPIC "A000000000000777/Esp8266_%s/%s/%s"
#define MQTT_DEV_SUB_TOPIC "A000000000000777/Esp8266_%s/%s/command"
#define MQTT_DEV_WILL_TOPIC "A000000000000777/Esp8266_%s/%s/status"

#define MQTT_WILL_MSG "{\"status\":\"offline\", \"data\":null, \"time\":null, \"err\":\"Driver not running\"}"

#define MQTT_USER NULL
#define MQTT_PASS NULL
#define PUB_MSG_LEN 1024

#endif // __EMBGW_CONFIG_H__
