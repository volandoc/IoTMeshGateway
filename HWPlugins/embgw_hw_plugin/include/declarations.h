#ifndef __GW_DECLARATIONS_H__
#define __GW_DECLARATIONS_H__

SemaphoreHandle_t wifi_alive, timer_start;
QueueHandle_t publish_queue;
QueueHandle_t gpio_queue;

char sub_topic[64];

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

const char *event_template = "%s;%ld";

typedef struct
{
    char *msg;
    char *topic;
}queue_buf_t;

queue_buf_t pub_msg;

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

static int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
    if (tok->type == JSMN_STRING && (int) strlen(s) == tok->end - tok->start &&
            strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
        return 0;
    }
    return -1;
}

#endif //__GW_DECLARATIONS_H__