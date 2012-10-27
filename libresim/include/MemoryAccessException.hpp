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

#ifndef RESIM_MEMORYACCESSEXCEPTION_HPP_INCLUDED
#define RESIM_MEMORYACCESSEXCEPTION_HPP_INCLUDED

#include "MemoryAccessMode.hpp"

#include <stdexcept>
#include <string>

class MemoryAccessException : public std::runtime_error {
public:
	MemoryAccessException(uintptr_t address,
	                      unsigned int size,
	                      MemoryAccessMode::List mode,
	                      const std::string &description);
	~MemoryAccessException() throw() {
	}

	uintptr_t getAddress() const {
		return address;
	}
	unsigned int getSize() const {
		return size;
	}
	MemoryAccessMode::List getMode() const {
		return mode;
	}
private:
	uintptr_t address;
	unsigned int size;
	MemoryAccessMode::List mode;
};

class NonexistentMemoryException : public MemoryAccessException {
public:
	NonexistentMemoryException(uintptr_t address,
	                           unsigned int size,
	                           MemoryAccessMode::List mode)
			: MemoryAccessException(address, size, mode,
			                        "No memory at the specified address.") {
	}
};

#endif
