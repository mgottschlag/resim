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

#ifndef RESIM_PROCESSOR_HPP_INCLUDED
#define RESIM_PROCESSOR_HPP_INCLUDED

#include <string>
#include <cstdint>
#include <vector>

class Log;
class Memory;

class Processor {
public:
	Processor() : log(NULL), memory(NULL) {
	}
	virtual ~Processor() {
	}

	static void registerProcessorType(const Processor *processor);
	static const std::vector<const Processor*> &getProcessorTypes();

	void initialize(Log *log, Memory *memory) {
		this->log = log;
		this->memory = memory;
	}

	virtual void setRegister(const std::string &name, uint64_t value) = 0;
	virtual uint64_t getRegister(const std::string &name) = 0;
	virtual int getRegisterSize(const std::string &name) = 0;
	virtual const std::vector<std::string> &getRegisterList() = 0;
	virtual void run(unsigned int steps) = 0;

	virtual Processor *clone() const = 0;

	virtual std::string getName() const = 0;

	Log *getLog() {
		return log;
	}
	Memory *getMemory() {
		return memory;
	}
private:
	Log *log;
	Memory *memory;
};

#endif
