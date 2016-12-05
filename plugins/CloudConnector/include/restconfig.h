#ifndef REST_CONFIG_H
#define REST_CONFIG_H

#define REST_HOST                   "https://test.vzudm.com/gw2c-server/"

#define REST_RESOURCE_PROVISION             "provision/"
#define REST_RESOURCE_DATASYNC              "datasync/"
#define REST_RESOURCE_DISCOVERED_SENSORS    "discoveredsensors/"
#define REST_RESOURCE_CONNECTED_SENSORS     "connectedsensors/"

#define REST_CERTIFICATE            "../certs/gw.crt"
#define REST_PRIVATEKEY             "../certs/gw.key"
#define REST_OUTPUT_FILE            "temp.log"
#define REST_PROVISION_FILE         "provision.log"
#define REST_DATASYNC_FILE          "datasync.log"
#define REST_DISCOVER_SENSORS_FILE  "discoveredSensors.log"

#endif //REST_CONFIG_H