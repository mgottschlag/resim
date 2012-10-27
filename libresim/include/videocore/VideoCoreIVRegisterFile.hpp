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

#ifndef RESIM_VIDEOCOREIVREGISTERFILE_HPP_INCLUDED
#define RESIM_VIDEOCOREIVREGISTERFILE_HPP_INCLUDED

#include <cstdint>
#include <vector>
#include <string>

enum VideoCoreIVRegister {
	VC_R0,
	VC_R1,
	VC_R2,
	VC_SP = 25,
	VC_LR = 26,
	VC_SR = 30,
	VC_PC = 31
};

class VideoCoreIVRegisterFile {
public:
	VideoCoreIVRegisterFile();

	const std::vector<std::string> &getRegisterNames() {
		return registerNames;
	}

	unsigned int getRegisterIndex(const std::string &name);

	uint32_t getRegister(unsigned int regIndex) {
		// TODO: Range check
		return registers[regIndex];
	}
	void setRegister(unsigned int regIndex, uint32_t value) {
		// TODO: Range check
		registers[regIndex] = value;
	}

	void setStatus(uint32_t mask, uint32_t value) {
		registers[VC_SR] = (registers[VC_SR] & ~mask) | (mask & value);
	}

private:
	uint32_t registers[32];

	std::vector<std::string> registerNames;
};

#endif
