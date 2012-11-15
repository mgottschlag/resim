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

#include "Memory.hpp"
#include "MemoryAccessException.hpp"
#include "MemoryArea.hpp"

#include <stdexcept>

uint32_t Memory::readWord(uintptr_t address) {
	if ((address & 3) != 0) {
		throw std::runtime_error("readWord(): Invalid address alignment!");
	}
	MemoryArea *area = getArea(address, 4, MemoryAccessMode::Read);
	return area->readWord(address);
}
void Memory::writeWord(uintptr_t address, uint32_t value) {
	if ((address & 3) != 0) {
		throw std::runtime_error("writeWord(): Invalid address alignment!");
	}
	MemoryArea *area = getArea(address, 4, MemoryAccessMode::Write);
	area->writeWord(address, value);
}
uint16_t Memory::readHalfWord(uintptr_t address) {
	if ((address & 1) != 0) {
		throw std::runtime_error("readHalfWord(): Invalid address alignment!");
	}
	MemoryArea *area = getArea(address, 2, MemoryAccessMode::Read);
	return area->readHalfWord(address);
}
void Memory::writeHalfWord(uintptr_t address, uint16_t value) {
	if ((address & 1) != 0) {
		throw std::runtime_error("writeHalfWord(): Invalid address alignment!");
	}
	MemoryArea *area = getArea(address, 2, MemoryAccessMode::Write);
	area->writeHalfWord(address, value);
}
uint8_t Memory::readByte(uintptr_t address) {
	MemoryArea *area = getArea(address, 1, MemoryAccessMode::Read);
	return area->readByte(address);
}
void Memory::writeByte(uintptr_t address, uint8_t value) {
	MemoryArea *area = getArea(address, 1, MemoryAccessMode::Write);
	area->writeByte(address, value);
}

void Memory::registerArea(MemoryArea *area) {
	// Find the neighbour areas
	int previous = -1;
	for (unsigned int i = 0; i < areas.size(); i++) {
		if (areas[i]->getStart() > area->getStart()) {
			break;
		} else {
			previous = i;
		}
	}
	int next = -1;
	if (previous != (int)areas.size() - 1) {
		next = previous + 1;
	}
	// Check for overlaps
	if ((previous != -1 && (areas[previous]->getEnd() > area->getStart()))
			|| (next != -1 && (areas[next]->getStart() < area->getEnd()))) {
		throw std::runtime_error("Memory areas overlap!");
	}
	// Insert the area into the (sorted) list
	if (next != -1) {
		areas.insert(areas.begin() + next, area);
	} else {
		areas.push_back(area);
	}
}

MemoryArea *Memory::getArea(uintptr_t address,
                            unsigned int size,
                            MemoryAccessMode::List mode) {
	for (MemoryArea *area : areas) {
		if (area->getStart() <= address && area->getEnd() >= address + size) {
			return area;
		} else if (area->getStart() > address) {
			break;
		}
	}
	throw NonexistentMemoryException(address, size, mode);
}

void Memory::setCurrentInstr(uintptr_t pc) {
	Memory::pc = pc;
}
uintptr_t Memory::getCurrentInstr() {
	return pc;
}

uintptr_t Memory::pc = 0;
