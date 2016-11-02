#ifndef DEVICE_H
#define DEVICE_H

#include <string>
#include <map>

typedef std::map<long, std::string> Properties;
typedef std::map<long, std::string>::iterator PropertiesIterator;

/**
 * Device interface that represents physical device.
 */
class Device {
public:
    Device(){}
    virtual ~Device(){}

    /**
     * Returns device name.
     *
     * @return device name
     */
    virtual std::string getName() = 0;

    /**
     * Returns device type.
     *
     * @return device type
     */
    virtual std::string getType() = 0;

    /**
     * Returns device serial
     *
     * @return device serial
     */
    virtual std::string getSerial() = 0;

    /**
     * Returns device description
     *
     * @return device description
     */
    virtual std::string getDescription() = 0;


    /**
     * Returns device specific connection information
     *
     * @return connection information
     */
    virtual std::string getConnectionInfo() = 0;

    /**
     * Processes incoming device command.
     *
     * @param command sensor command
     */
    virtual void processDeviceCommand(std::string command) = 0;

    /**
     * Returns current device state.
     *
     * @return device state
     */
    virtual Properties getProperties() = 0;

    /**
     * Performs connection procedure.
     */
    virtual void connect() = 0;
};

#endif // DEVICE_H
