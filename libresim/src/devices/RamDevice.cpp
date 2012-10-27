
#include "devices/RamDevice.hpp"
#include "RamArea.hpp"
#include "Log.hpp"
#include "Memory.hpp"

#include <cassert>
#include <ios>
#include <sstream>

class RegisterRamDevice {
public:
	RegisterRamDevice() {
		static RamDevice device;
		Device::registerDeviceType(&device);
	}
};

static RegisterRamDevice reg;

RamDevice::RamDevice()
		: start(0), size(0x8000000), memoryArea(NULL), data(NULL) {
}
RamDevice::RamDevice(const RamDevice &other)
		: start(other.start), size(other.size), memoryArea(NULL), data(NULL) {
}
RamDevice::~RamDevice() {
	if (memoryArea) {
		delete memoryArea;
	}
	if (data) {
		delete[] data;
	}
}

void RamDevice::initialize(Log *log, Memory *memory) {
	if (data == NULL) {
		data = new uint8_t[size];
	}
	memoryArea = new RamArea(data, start, start + size);
	memory->registerArea(memoryArea);
	log->info("ram", "Added DRAM (0x%08lx to 0x%08lx).", start,
			start + size);
}

Device *RamDevice::clone() const {
	return new RamDevice(*this);
}
