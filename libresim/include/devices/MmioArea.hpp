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

#ifndef RESIM_MMIOAREA_HPP_INCLUDED
#define RESIM_MMIOAREA_HPP_INCLUDED

#include "MemoryArea.hpp"

#include <string>
#include <vector>

class Log;

struct MmioRegister {
	std::string name;
	uintptr_t offset;
	uint32_t size;
};

struct MmioRegisterGroup {
	std::string name;
	uintptr_t offset;
	uintptr_t size;
	unsigned int registerCount;
	MmioRegister *registers;
};

struct MmioDefinition {
	unsigned int registerGroupCount;
	MmioRegisterGroup *registerGroups;
};

typedef void (*MMIO_WRITE_CB)(void *context,
                              uintptr_t address,
                              uint64_t value,
                              unsigned int size);
typedef uint64_t (*MMIO_READ_CB)(void *context,
                                 uintptr_t address,
                                 unsigned int size);

class MmioArea : public MemoryArea {
public:
	MmioArea(MmioDefinition registers, uintptr_t start, uintptr_t end,
			Log *log);
	virtual ~MmioArea();

	void setCallbacks(MMIO_WRITE_CB writeImpl,
	                  MMIO_READ_CB readImpl,
	                  void *context) {
		this->writeImpl = writeImpl;
		this->readImpl = readImpl;
		this->cbContext = context;
	}

	virtual uint32_t readWord(uintptr_t address);
	virtual void writeWord(uintptr_t address, uint32_t value);
	virtual uint16_t readHalfWord(uintptr_t address);
	virtual void writeHalfWord(uintptr_t address, uint16_t value);
	virtual uint8_t readByte(uintptr_t address);
	virtual void writeByte(uintptr_t address, uint8_t value);
private:
	std::string decode(uintptr_t address, unsigned int size);

	MmioDefinition registers;

	Log *log;

	MMIO_WRITE_CB writeImpl;
	MMIO_READ_CB readImpl;
	void *cbContext;
};

#endif
