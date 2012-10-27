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

#include "RamArea.hpp"

#include <cassert>

RamArea::RamArea(uint8_t *data, uintptr_t start, uintptr_t end)
		: MemoryArea(start, end), data(data) {
	assert(end > start);
	//data = new uint8_t[end - start];
}
RamArea::~RamArea() {
	//delete[] data;
}

uint32_t RamArea::readWord(uintptr_t address) {
	assert(address >= getStart() && address + 4 <= getEnd());
	return *(uint32_t*)(data + (address - getStart()));
}
void RamArea::writeWord(uintptr_t address, uint32_t value) {
	assert(address >= getStart() && address + 4 <= getEnd());
	*(uint32_t*)(data + (address - getStart())) = value;
}
uint16_t RamArea::readHalfWord(uintptr_t address) {
	assert(address >= getStart() && address + 2 <= getEnd());
	return *(uint16_t*)(data + (address - getStart()));
}
void RamArea::writeHalfWord(uintptr_t address, uint16_t value) {
	assert(address >= getStart() && address + 2 <= getEnd());
	*(uint16_t*)(data + (address - getStart())) = value;
}
uint8_t RamArea::readByte(uintptr_t address) {
	assert(address >= getStart() && address + 1 <= getEnd());
	return *(data + (address - getStart()));
}
void RamArea::writeByte(uintptr_t address, uint8_t value) {
	assert(address >= getStart() && address + 1 <= getEnd());
	*(data + (address - getStart())) = value;
}
