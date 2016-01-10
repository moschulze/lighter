#ifndef LIGHTER_DEVICETYPE_H
#define LIGHTER_DEVICETYPE_H


#include <string>
#include <map>

class DeviceType {
public:
    std::string name;
    std::string id;
    int getNumberOfSlots();
    void addSlot(std::string name, int number);

private:
    std::map<std::string, int> slots;
};


#endif //LIGHTER_DEVICETYPE_H
