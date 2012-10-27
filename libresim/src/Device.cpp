
#include "Device.hpp"

#include <iostream>

static std::vector<const Device*> deviceTypes;

void Device::registerDeviceType(const Device *device) {
	std::cout << "Registgered " << device->getName() << std::endl;
	deviceTypes.push_back(device);
}
const std::vector<const Device*> &Device::getDeviceTypes() {
	return deviceTypes;
}
