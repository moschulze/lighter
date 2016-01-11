#include "SceneStep.h"

void SceneStep::addDeviceData(Device *device, uint8_t *data) {
    this->data[device] = data;
}
