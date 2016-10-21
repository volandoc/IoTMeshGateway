#ifndef COMMANDS_H
#define COMMANDS_H

#include <string>

int cmd_count = 7;
enum cmd_values {GET,SET,RESET,UPDATE,REBOOT,CONNECT,DISCONNECT};
char *cmd_string_values[cmd_count] = {"GET","SET","RESET","UPDATE","REBOOT","CONNECT","DISCONNECT"};

class Command {
private:
    cmd_values value;
    std::string id;
public:
    std::string getCommandID() { return id; }
    int getCommandValue() { return value; }
    std::string getCommandStrValue() { return cmd_string_values[cmd_values]; }

};

class CommandSet {
private:

public:
    int getCommandByName(std::string command);
};



#endif // COMMANDS_H
