
#include "devices/MmioArea.hpp"
#include "Log.hpp"
#include "Memory.hpp"

#include <stdexcept>
#include <sstream>

MmioArea::MmioArea(MmioDefinition registers, uintptr_t start, uintptr_t end,
		Log *log)
		: MemoryArea(start, end), registers(registers), log(log),
		writeImpl(NULL), readImpl(NULL), cbContext(NULL) {
}
MmioArea::~MmioArea() {
}

uint32_t MmioArea::readWord(uintptr_t address) {
	uint32_t value = 0x00000000;
	if (readImpl) {
		value = readImpl(cbContext, address, 4);
	}
	log->info("mmio", "MMIO(R, 4, 0x%08x): 0x%08x => 0x%08x",
	          (uint32_t)Memory::getCurrentInstr(),
	          (uint32_t)address,
	          value);
	return value;
}
void MmioArea::writeWord(uintptr_t address, uint32_t value) {
	log->info("mmio", "MMIO(W, 4, 0x%08x): 0x%08x <= 0x%08x",
	          (uint32_t)Memory::getCurrentInstr(),
	          (uint32_t)address,
	          value);
	if (writeImpl) {
		writeImpl(cbContext, address, value, 4);
	}
}
uint16_t MmioArea::readHalfWord(uintptr_t address) {
	// TODO
	throw std::runtime_error("Unimplemented MMIO read.");
}
void MmioArea::writeHalfWord(uintptr_t address, uint16_t value) {
	// TODO
	throw std::runtime_error("Unimplemented MMIO write.");
}
uint8_t MmioArea::readByte(uintptr_t address) {
	// TODO
	throw std::runtime_error("Unimplemented MMIO read.");
}
void MmioArea::writeByte(uintptr_t address, uint8_t value) {
	// TODO
	throw std::runtime_error("Unimplemented MMIO write.");
}

std::string MmioArea::decode(uintptr_t address, unsigned int size) {
	address -= getStart();
	std::ostringstream decoded;
	MmioRegisterGroup *group = NULL;
	for (unsigned int i = 0; i < registers.registerGroupCount; i++) {
		uintptr_t start = registers.registerGroups[i].offset;
		uintptr_t end = start + registers.registerGroups[i].size;
		if (address >= start && address + size <= end) {
			group = &registers.registerGroups[i];
			break;
		}
	}
	if (group != NULL) {
		address -= group->offset;
		MmioRegister *reg = NULL;
		for (unsigned int i = 0; i < group->registerCount; i++) {
			if (group->registers[i].offset == address
					&& group->registers[i].size == size) {
				reg = &group->registers[i];
				break;
			}
		}
		if (reg != NULL) {
			decoded << group->name << "." << reg->name;
		} else {
			decoded << group->name << " + 0x" << std::hex << address;
		}
	} else {
		decoded << "0x";
		decoded.fill('0');
		decoded.width(8);
		decoded << std::hex << address;
	}
	return decoded.str();
}
