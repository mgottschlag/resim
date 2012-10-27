/*
Copyright (C) 2012, Mathias Gottschlag

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "SimulationThread.hpp"
#include "Processor.hpp"
#include "Device.hpp"
#include "Memory.hpp"

#include <cassert>

SimulationThread::SimulationThread()
		: simulation(NULL), processorPrototype(NULL), paused(true),
		stopThread(false), resetted(false), resetProcessor(NULL),
		running(false) {
	this->moveToThread(&thread);
	connect(&thread, SIGNAL(started()), this, SLOT(threadEntry()));
	thread.start();
}
SimulationThread::~SimulationThread() {
	// Stop the thread
	pausedMutex.lock();
	stopThread = true;
	paused = false;
	pausedMutex.unlock();
	wait();
	// Clean up
	if (processorPrototype != NULL) {
		delete processorPrototype;
	}
	for (auto it = devicePrototypes.begin(); it != devicePrototypes.end();
			it++) {
		delete *it;
	}
	for (auto it = memoryContent.begin(); it != memoryContent.end(); it++) {
		delete *it;
	}
}

void SimulationThread::setProcessor(Processor *processor) {
	if (processorPrototype != NULL) {
		delete processorPrototype;
	}
	processorPrototype = processor;
}

void SimulationThread::addDevice(Device *device) {
	devicePrototypes.push_back(device);
}
void SimulationThread::removeDevice(Device *device) {
	for (unsigned int i = 0; i < devicePrototypes.size(); i++) {
		if (devicePrototypes[i] == device) {
			delete devicePrototypes[i];
			devicePrototypes.erase(devicePrototypes.begin() + i);
			return;
		}
	}
	assert(false && "Invalid device.");
}

void SimulationThread::addMemoryContent(MemoryContent *data) {
	memoryContent.push_back(data);
	// TODO: Check validity
}
void SimulationThread::removeMemoryContent(unsigned int index) {
	assert(index < memoryContent.size());
	delete memoryContent[index];
	memoryContent.erase(memoryContent.begin() + index);
}

void SimulationThread::start() {
	pausedMutex.lock();
	paused = false;
	pausedCondition.wakeAll();
	pausedMutex.unlock();
}
void SimulationThread::stop() {
	pausedMutex.lock();
	paused = true;
	pausedMutex.unlock();
}
void SimulationThread::reset() {
	pausedMutex.lock();
	if (resetted) {
		delete resetProcessor;
		for (auto it = resetDevices.begin(); it != resetDevices.end(); it++) {
			delete *it;
		}
		for (auto it = resetMemory.begin(); it != resetMemory.end(); it++) {
			delete *it;
		}
	}
	resetProcessor = processorPrototype->clone();
	resetDevices = devicePrototypes;
	for (unsigned int i = 0; i < resetDevices.size(); i++) {
		resetDevices[i] = resetDevices[i]->clone();
	}
	resetMemory = memoryContent;
	for (unsigned int i = 0; i < resetMemory.size(); i++) {
		resetMemory[i] = new MemoryContent(*resetMemory[i]);
	}
	resetted = true;
	pausedMutex.unlock();
}

bool SimulationThread::isRunning() {
	bool running;
	pausedMutex.lock();
	running = this->running;
	pausedMutex.unlock();
	return running;
}

void SimulationThread::threadEntry() {
	bool prevPaused = true;
	bool resetted = false;
	Memory *memory = NULL;
	Processor *processor = NULL;
	std::vector<Device*> devices;
	std::vector<MemoryContent*> memoryContent;
	while (true) {
		// Wait for the thread to be started
		pausedMutex.lock();
		while (paused == true) {
			if (!prevPaused) {
				prevPaused = true;
				running = false;
				emit runningChanged();
			}
			pausedCondition.wait(&pausedMutex);
		}
		if (stopThread) {
			pausedMutex.unlock();
			break;
		}
		if (prevPaused) {
			prevPaused = false;
			running = true;
			emit runningChanged();
		}
		if (this->resetted) {
			resetted = true;
			this->resetted = false;
			if (processor != NULL) {
				delete processor;
			}
			processor = resetProcessor;
			for (unsigned int i = 0; i < devices.size(); i++) {
				delete devices[i];
			}
			devices = resetDevices;
			resetDevices.clear();
			memoryContent = resetMemory;
			for (unsigned int i = 0; i < memoryContent.size(); i++) {
				delete memoryContent[i];
			}
			resetMemory.clear();
		}
		pausedMutex.unlock();
		// Initialize all devices if the system was resetted
		if (resetted) {
			resetted = false;
			if (memory != NULL) {
				delete memory;
			}
			memory = new Memory();
			log.info("", "Initializing all devices...");
			processor->initialize(&log, memory);
			for (unsigned int i = 0; i < devices.size(); i++) {
				devices[i]->initialize(&log, memory);
			}
			log.info("", "Preparing the initial memory content...");
			insertMemoryContent(memory, memoryContent);
			log.info("", "Initialization done.");
		}
		// Execute some steps of the simulation
		assert(processor != NULL);
		processor->run(1000);
	}
}

void SimulationThread::insertMemoryContent(Memory *memory,
		const std::vector<MemoryContent*> &content) {
	for (unsigned int i = 0; i < memoryContent.size(); i++) {
		const QByteArray &data = memoryContent[i]->data;
		uintptr_t base = memoryContent[i]->baseAddress;
		for (int offset = 0; offset < data.length(); offset++) {
			memory->writeByte(base + offset, data.at(offset));
		}
	}
}

