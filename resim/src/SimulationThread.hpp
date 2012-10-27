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

#ifndef ARMSIM_SIMULATIONTHREAD_HPP_INCLUDED
#define ARMSIM_SIMULATIONTHREAD_HPP_INCLUDED

#include "Log.hpp"

#include <QThread>
#include <QObject>
#include <QSemaphore>
#include <vector>
#include <QWaitCondition>
#include <QMutex>

class Processor;
class Device;
class Simulation;
class Memory;

struct MemoryContent {
	QString name;
	QByteArray data;
	uintptr_t baseAddress;
};

class SimulationThread : public QObject {
	Q_OBJECT
public:
	SimulationThread();
	~SimulationThread();

	void setProcessor(Processor *processor);
	Processor *getProcessor() const {
		return processorPrototype;
	}
	void addDevice(Device *device);
	void removeDevice(Device *device);
	const std::vector<Device*> &getDevices() {
		return devicePrototypes;
	}

	void addMemoryContent(MemoryContent *data);
	void removeMemoryContent(unsigned int index);
	const std::vector<MemoryContent*> &getMemoryContent() {
		return memoryContent;
	}

	void start();
	void stop();
	void reset();

	bool isRunning();

	Log &getLog() {
		return log;
	}
signals:
	void runningChanged();
	void forcedPaused(const QString &message);
private slots:
	void threadEntry();
private:
	void insertMemoryContent(Memory *memory,
	                         const std::vector<MemoryContent*> &content);

	Simulation *simulation;

	Processor *processorPrototype;
	std::vector<Device*> devicePrototypes;
	std::vector<MemoryContent*> memoryContent;

	QThread thread;

	QSemaphore startSignal;

	QWaitCondition pausedCondition;
	QMutex pausedMutex;
	bool paused;
	bool stopThread;

	bool resetted;
	Processor *resetProcessor;
	std::vector<Device*> resetDevices;
	std::vector<MemoryContent*> resetMemory;

	QMutex runningMutex;
	bool running;

	Log log;
};

#endif
