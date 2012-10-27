
#ifndef RESIM_VIDEOCOREIVEXECUTE_HPP_INCLUDED
#define RESIM_VIDEOCOREIVEXECUTE_HPP_INCLUDED

#include "Log.hpp"
#include "Memory.hpp"
#include "videocore/VideoCoreIVRegisterFile.hpp"

#include <stdexcept>

static const uint32_t VIDEOCORE4_CPUID = 0x04000104;

enum BinaryOp {
	OP_MOV,
	OP_CMN,
	OP_ADD,
	OP_BIC,
	OP_MUL,
	OP_EOR,
	OP_SUB,
	OP_AND,
	OP_MVN,
	OP_ROR,
	OP_CMP,
	OP_RSB,
	OP_BTST,
	OP_OR,
	OP_EXTU,
	OP_MAX,
	OP_BSET,
	OP_MIN,
	OP_BCLR,
	OP_ADDS2,
	OP_BCHG,
	OP_ADDS4,
	OP_ADDS8,
	OP_ADDS16,
	OP_EXTS,
	OP_NEG,
	OP_LSR,
	OP_CLZ,
	OP_LSL,
	OP_BREV,
	OP_ASR,
	OP_ABS
};

enum LoadStoreWidth {
	WIDTH_U32,
	WIDTH_U16,
	WIDTH_U8,
	WIDTH_S16
};

class VideoCoreIVExecute {
public:
	VideoCoreIVExecute(Memory *memory,
	                   VideoCoreIVRegisterFile &registers,
	                   Log *log)
			: memory(memory), registers(registers), log(log) {
	}

	void rti() {
		// TODO
		throw std::runtime_error("Unsupported instruction.");
	}

	void b(unsigned int reg) {
		registers.setRegister(VC_PC, registers.getRegister(reg));
	}

	void bl(unsigned int reg) {
		// TODO
		throw std::runtime_error("Unsupported instruction.");
	}

	void tbb(unsigned int reg) {
		// TODO
		throw std::runtime_error("Unsupported instruction.");
	}

	void tbh(unsigned int reg) {
		// TODO
		throw std::runtime_error("Unsupported instruction.");
	}

	void cpuid(unsigned int reg) {
		registers.setRegister(reg, VIDEOCORE4_CPUID);
	}

	void add(unsigned int rd, unsigned int rs, unsigned int shift) {
		// TODO
		throw std::runtime_error("Unsupported instruction.");
	}

	void binaryOp(unsigned int op, unsigned int rd, unsigned int rs) {
		uint32_t a = registers.getRegister(rd);
		uint32_t b = registers.getRegister(rs);
		doBinaryOp(op, rd, a, b);
	}

	void binaryOpImm(unsigned int op, unsigned int rd, int32_t imm) {
		uint32_t a = registers.getRegister(rd);
		doBinaryOp(op, rd, a, imm);
	}

	void binaryOp(unsigned int cond, unsigned int op, int rd, int ra, int rb) {
		if (!checkCond(cond)) {
			return;
		}
		uint32_t a = registers.getRegister(ra);
		uint32_t b = registers.getRegister(rb);
		doBinaryOp(op, rd, a, b);
	}

	void binaryOpImm(int cond, int op, int rd, int ra, int imm) {
		if (!checkCond(cond)) {
			return;
		}
		uint32_t a = registers.getRegister(ra);
		doBinaryOp(op, rd, a, imm);
	}

	bool bcc(unsigned int cond, int32_t offset) {
		if (!checkCond(cond)) {
			return false;
		}
		registers.setRegister(VC_PC, registers.getRegister(VC_PC) + offset);
		return true;
	}

	bool bccCmp(unsigned int cond, int rd, int rs, int32_t offset) {
		uint32_t b = registers.getRegister(rd);
		return bccCmpImm(cond, rd, b, offset);
	}

	bool bccCmpImm(unsigned int cond, int rd, uint32_t imm, int32_t offset) {
		uint32_t a = registers.getRegister(rd);
		log->debug("vc4exec", "bccCmpImm: %08x %08x", a, imm);
		unsigned int status = compare(a, imm);
		if (!checkCond(cond, status)) {
			return false;
		}
		registers.setRegister(VC_PC, registers.getRegister(VC_PC) + offset);
		return true;
	}

	void blImm(int32_t offset, uint32_t instSize) {
		uint32_t pc = registers.getRegister(VC_PC);
		registers.setRegister(VC_LR, pc + instSize);
		registers.setRegister(VC_PC, pc + offset);
	}

	void pushPop(bool push, bool lrpc, unsigned int start, unsigned int count) {
		if (push) {
			for (unsigned int i = 0; i < count; i++) {
				this->push((start + i) % 32);
			}
			if (lrpc) {
				this->push(VC_LR);
			}
		} else {
			if (lrpc) {
				this->pop(VC_PC);
			}
			for (int i = count - 1; i >= 0; i--) {
				this->pop((start + i) % 32);
			}
		}
	}

	void push(unsigned int reg) {
		uint32_t sp = registers.getRegister(VC_SP);
		sp -= 4;
		uint32_t value = registers.getRegister(reg);
		log->debug("vc4exec", "pushing %d to %08x: %08x", reg, sp, value);
		memory->writeWord(sp, registers.getRegister(reg));
		registers.setRegister(VC_SP, sp);
	}

	void pop(unsigned int reg) {
		uint32_t sp = registers.getRegister(VC_SP);
		log->debug("vc4exec", "popping %d to %08x: %08x", reg, sp, memory->readWord(sp));
		registers.setRegister(reg, memory->readWord(sp));
		sp += 4;
		registers.setRegister(VC_SP, sp);
	}

	void loadStoreOffset(bool store, int format, int rd, int rb, int offset) {
		uint32_t address = registers.getRegister(rb) + offset;
		if (store) {
			uint32_t value = registers.getRegister(rd);
			switch (format) {
				case WIDTH_U32:
					memory->writeWord(address, value);
					break;
				case WIDTH_S16:
				case WIDTH_U16:
					memory->writeHalfWord(address, value);
					break;
				case WIDTH_U8:
					memory->writeByte(address, value);
					break;
				default:
					throw std::runtime_error("Invalid load/store format.");
			}
		} else {
			uint32_t value;
			switch (format) {
				case WIDTH_U32:
					value = memory->readWord(address);
					break;
				case WIDTH_U16:
					value = memory->readHalfWord(address);
					break;
				case WIDTH_U8:
					value = memory->readByte(address);
					break;
				case WIDTH_S16:
					value = (int32_t)(int16_t)memory->readHalfWord(address);
					break;
				default:
					throw std::runtime_error("Invalid load/store format.");
			}
			registers.setRegister(rd, value);
		}
	}

	void leaSp(unsigned int rd, int32_t offset) {
		uint32_t address = registers.getRegister(VC_SP) + offset;
		registers.setRegister(rd, address);
	}

private:
	void doBinaryOp(unsigned int op, unsigned int rd, uint32_t a, uint32_t b) {
		if (op >= 0x20) {
			throw std::runtime_error("Invalid operation.");
		}
		// TODO: What about status bits after arith. operations?
		log->debug("vc4exec", "Op: %d %08x %08x", op, a, b);
		switch (op) {
			case OP_MOV:
				registers.setRegister(rd, b);
				break;
			case OP_ADD:
				registers.setRegister(rd, a + b);
				break;
			case OP_BIC:
				registers.setRegister(rd, a & ~b);
				break;
			case OP_MUL:
				registers.setRegister(rd, a * b);
				break;
			case OP_SUB:
				registers.setRegister(rd, a - b);
				break;
			case OP_AND:
				registers.setRegister(rd, a & b);
				break;
			case OP_MVN:
				registers.setRegister(rd, ~b);
				break;
			case OP_BTST:
				registers.setStatus(0x8, (a & (1 << b)) != 0 ? 0x8 : 0x0);
				break;
			case OP_OR:
				registers.setRegister(rd, a | b);
				break;
			case OP_EXTU:
				registers.setRegister(rd, a & ((1 << b) - 1));
				break;
			case OP_LSL:
				registers.setRegister(rd, a << b);
				break;
			default:
				throw std::runtime_error("Unimplemented operation.");
		}
		log->debug("vc4exec", "Result: %08x", registers.getRegister(rd));
	}

	bool checkCond(unsigned int cond) {
		return checkCond(cond, registers.getRegister(VC_SR));
	}

	bool checkCond(unsigned int cond, unsigned int status) {
		if (cond == 0x0) {
			return (status & 0x8) != 0;
		} else if (cond == 0x1) {
			return (status & 0x8) == 0;
		} else if (cond == 0x2) {
			return (status & 0x2) != 0;
		} else if (cond == 0x3) {
			return (status & 0x2) == 0;
		} else if (cond == 0xd) {
			return ((status & 0x8) != 0) || ((status & 0x4) != 0);
		} else if (cond == 0xe) {
			return true;
		} else if (cond == 0xf) {
			return false;
		} else {
			throw std::runtime_error("Unimplemented condition.");
		}
	}

	unsigned int compare(uint32_t a, uint32_t b) {
		unsigned int status = 0;
		if (a == b) {
			status |= 8;
		}
		if ((a - b) & 0x80000000) {
			status |= 4;
		}
		if (a < b) {
			status |= 2;
		}
		// TODO
		return status;
	}

	Memory *memory;
	VideoCoreIVRegisterFile &registers;
	Log *log;
};

#endif
