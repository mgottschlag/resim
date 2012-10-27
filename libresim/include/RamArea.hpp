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

#ifndef RESIM_RAMAREA_HPP_INCLUDED
#define RESIM_RAMAREA_HPP_INCLUDED

#include "MemoryArea.hpp"

class RamArea : public MemoryArea {
public:
	RamArea(uint8_t *data, uintptr_t start, uintptr_t end);
	virtual ~RamArea();

	virtual uint32_t readWord(uintptr_t address);
	virtual void writeWord(uintptr_t address, uint32_t value);
	virtual uint16_t readHalfWord(uintptr_t address);
	virtual void writeHalfWord(uintptr_t address, uint16_t value);
	virtual uint8_t readByte(uintptr_t address);
	virtual void writeByte(uintptr_t address, uint8_t value);
private:
	uint8_t *data;
};

#endif
