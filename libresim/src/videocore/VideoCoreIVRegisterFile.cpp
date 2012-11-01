
#include "videocore/VideoCoreIVRegisterFile.hpp"

#include <iterator>
#include <stdexcept>

static struct {
	const char *name;
	unsigned int index;
} registerInfo[] = {
	{"r0", 0},
	{"r1", 1},
	{"r2", 2},
	{"r3", 3},
	{"r4", 4},
	{"r5", 5},
	{"r6", 6},
	{"r7", 7},
	{"r8", 8},
	{"r9", 9},
	{"r10", 10},
	{"r11", 11},
	{"r12", 12},
	{"r13", 13},
	{"r14", 14},
	{"r15", 15},
	{"r16", 16},
	{"r17", 17},
	{"r18", 18},
	{"r19", 19},
	{"r20", 20},
	{"r21", 21},
	{"r22", 22},
	{"r23", 23},
	{"r24", 24},
	{"sp", 25},
	{"lr", 26},
	{"r27", 27},
	{"r28", 28},
	{"r29", 29},
	{"sr", 30},
	{"pc", 31},
};

VideoCoreIVRegisterFile::VideoCoreIVRegisterFile() {
	for (unsigned int i = 0; i < 32; i++) {
		registers[i] = 0;
	}

	const unsigned int registerCount =
			sizeof(registerInfo) / sizeof(registerInfo[0]);
	registerNames.reserve(registerCount);
	for (unsigned int i = 0; i < registerCount; i++) {
		registerNames.push_back(registerInfo[i].name);
	}
}

unsigned int VideoCoreIVRegisterFile::getRegisterIndex(const std::string &name) {
	for (unsigned int i = 0; i < registerNames.size(); i++) {
		if (registerNames[i] == name) {
			return registerInfo[i].index;
		}
	}
	throw std::runtime_error("Invalid register name.");
}
