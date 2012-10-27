
#include "Processor.hpp"

static std::vector<const Processor*> processorTypes;

void Processor::registerProcessorType(const Processor *processor) {
	processorTypes.push_back(processor);
}
const std::vector<const Processor*> &Processor::getProcessorTypes() {
	return processorTypes;
}
