#include "JSON_messages.h"
/*#include "cJSON.h"

#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
*/

#define ONBORDING_MSG   "{\n"\
                        "	\"unitName\":	\"VZW_LH_UNIT_%02X%02X%02X%02X%02X34567\",\n"\
                        "	\"unitMacId\":	\"0800272F0000\",\n"\
                        "	\"unitSerialNo\":	\"34567\",\n"\
                        "	\"sensor\":	{\n"\
                        "		\"name\":	\"BULB\",\n"\
                        "		\"id\":	\"no_id_0\",\n"\
                        "		\"characteristics\":	[{\n"\
                        "				\"characteristicsName\":	\"UNITState\",\n"\
                        "				\"parameterType\":	\"Boolean\",\n"\
                        "				\"readLevel\":	\"RW\",\n"\
                        "				\"providedValues\":	[\"true\", \"false\"],\n"\
                        "				\"currentValue\":	\"false\",\n"\
                        "				\"measurementUnit\":	\"NULL\",\n"\
                        "				\"additionalInfo\":	\"NULL\"\n"\
                        "			}, {\n"\
                        "				\"characteristicsName\":	\"DimmerValue\",\n"\
                        "				\"parameterType\":	\"NumberRange\",\n"\
                        "				\"readLevel\":	\"RW\",\n"\
                        "				\"providedValues\":	[\"MIN=0\", \"MAX=100\", \"STEP=1\"],\n"\
                        "				\"currentValue\":	\"0\",\n"\
                        "				\"measurementUnit\":	\"Percent\",\n"\
                        "				\"additionalInfo\":	\"NULL\"\n"\
                        "			}]\n"\
                        "	},\n"\
                        "	\"availableUnits\":	[{\n"\
                        "			\"name\":	\"BULB\",\n"\
                        "			\"id\":	\"no_id_0\"\n"\
                        "		}, {\n"\
                        "			\"name\":	\"SECURITY\",\n"\
                        "			\"id\":	\"no_id_1\"\n"\
                        "		}]\n"\
                        "}"


#define RESPONSE_MSG    "{\n"\
                        "	\"unitCommand\":	\"NO_CMD\",\n"\
                        "	\"commandUUID\":	\"NO_UUID_DATA\",\n"\
                        "	\"statusMsg\":	\"BAD_REQUEST\",\n"\
                        "	\"statusCode\":	400\n"\
                        "}"


/*
static  bool get_mac_addr(unsigned char* mac_add)
{
    ifreq ifr;
    ifconf ifc;
    char ifname_buf[1024];
    int success = 0;

    int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
    if (sock == -1)
    {
        return false
    };

    ifc.ifc_len = sizeof(ifname_buf);
    ifc.ifc_buf = ifname_buf;

    if (ioctl(sock, SIOCGIFCONF, &ifc) == -1)
    {
        return false;
    }

    ifreq* iterator = ifc.ifc_req;
    const ifreq* const end = iterator + (ifc.ifc_len / sizeof(ifreq));

    for (; iterator != end; ++iterator)
    {
        strcpy(ifr.ifr_name, iterator->ifr_name);
        if (ioctl(sock, SIOCGIFFLAGS, &ifr) == 0)
        {
            if (! (ifr.ifr_flags & IFF_LOOPBACK)) /* skip loopback */
/*            {
                if (ioctl(sock, SIOCGIFHWADDR, &ifr) == 0)
                {
                    memcpy(mac_add, ifr.ifr_hwaddr.sa_data, 6);
                    return true;
                }
            }
        }
        else
        { 
            return false;
        }
    }

    return false;
}
*/
char const* generate_onbording_msg()
{
    return ONBORDING_MSG;
}


char const* generate_response_msg()
{
    return RESPONSE_MSG;
}




