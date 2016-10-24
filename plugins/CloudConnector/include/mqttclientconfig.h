#ifndef MQTTCLIENT_CONGIG_H
#define MQTTCLIENT_CONGIG_H

#define MQTT_HOST                               "A24HWB7QHR7PGW.iot.us-east-1.amazonaws.com" /*Customer specific MQTT HOST*/

#define MQTT_ID                                 "embgw123"
#define MQTT_PORT                               (8883)                                            /*default port for MQTT*/
#define MQTT_KEEPALIVE                          (60)                                              /*keep alive for MQTT*/

#define MQTT_ROOT_CA_FILENAME                   "../certs/test.rootCA.crt.pem"                                  /*Root CA file name*/
#define MQTT_CERTIFICATE_FILENAME               "../certs/test.certificate.crt.pem"                             /*device signed certificate file name*/
#define MQTT_PRIVATE_KEY_FILENAME               "../certs/test.private.key.pem"                                 /*Device private key filename*/
#define MQTT_CERT_DIR                           "../certs/"

#define MQTT_TLS_VER                            "tlsv1.2"

#define MQTT_TOPIC_SERV_PUBL_COMMAND            "ThingspaceSDK/0800272F0000/TSServerPublishCommand"
#define MQTT_TOIPIC_UNIT_PUBL_UNIT_ON_BOARD     "ThingspaceSDK/0800272F0000/UNITOnBoard"
#define MQTT_TOIPIC_UNIT_PUBL_CMD_RESPONSE      "ThingspaceSDK/0800272F0000/UNITCmdResponse"


#define MQTT_TOPIC_SUB_SENSOR_ACTUATOR_CMD      "SensorActuator/Command/VerizonGateway/%s/%s/"      //SensorActuator/Command/VerizonGateway/{homeId}/{gatewayId}
#define MQTT_TOPIC_SUB_SCENARIO_EXEC_CMD        "ScenarioExecCommand/"
#define MQTT_TOPIC_SUB_GW_CMD                   "GW/Command/%s"                                     //"GW/Command/${gateway_serial}"

#define MQTT_TOPIC_PUB_SENSOR_ACTUATOR_OCCUR    "SensorActuator/Occurrence/VerizonGateway/%s/%s/"   //SensorActuator/Occurrence/VerizonGateway/{homeId}/{gateway_serial}/
#define MQTT_TOPIC_PUB_SENSOR_ACTUATOR_ERROR    "SensorActuator/Error/VerizonGateway/%s/%s/"        // SensorActuator/Error/VerizonGateway/{homeId}/{gateway_serial}/


#endif // MQTTCLIENT_CONGIG_H
