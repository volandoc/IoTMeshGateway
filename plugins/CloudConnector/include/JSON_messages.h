#if !defined(JSON_MESSAGES_H)
#define JSON_MESSAGES_H

#define GW_STATUS_ONLINE                   "{\"status\":\"ONLINE\"}"                           // online status message
#define GW_STATUS_OFFLINE                  "{\"status\":\"OFFLINE\"}"                          // offline status message

char const* generate_onbording_msg();

char const* generate_response_msg();

#endif // JSON_MESSAGES_H