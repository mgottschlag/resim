
#include "videocore/VideoCoreIVProcessor.hpp"
#include "VideoCoreIVDecode.hpp"
#include "VideoCoreIVExecute.hpp"

#include <cassert>

class RegisterVideoCoreIVProcessor {
public:
	RegisterVideoCoreIVProcessor() {
		static VideoCoreIVProcessor processor;
		Processor::registerProcessorType(&processor);
	}
};

static RegisterVideoCoreIVProcessor reg;

VideoCoreIVProcessor::VideoCoreIVProcessor() {
}
VideoCoreIVProcessor::VideoCoreIVProcessor(const VideoCoreIVProcessor &other) {
	registers = other.registers;
}

void VideoCoreIVProcessor::setRegister(const std::string &name,
                                       uint64_t value) {
	unsigned int index = registers.getRegisterIndex(name);
	registers.setRegister(index, value);
}
uint64_t VideoCoreIVProcessor::getRegister(const std::string &name) {
	unsigned int index = registers.getRegisterIndex(name);
	return registers.getRegister(index);
}
int VideoCoreIVProcessor::getRegisterSize(const std::string &name) {
	return 4;
}
const std::vector<std::string> &VideoCoreIVProcessor::getRegisterList() {
	return registers.getRegisterNames();
}
void VideoCoreIVProcessor::run(unsigned int steps) {
	VideoCoreIVDecode decode(getMemory(), registers, getLog());
	for (unsigned int i = 0; i < steps; i++) {
		decode.step();
	}
}

Processor *VideoCoreIVProcessor::clone() const {
	return new VideoCoreIVProcessor(*this);
}
