
#include "devices/Bcm2835.hpp"
#include "RamArea.hpp"
#include "Log.hpp"
#include "Memory.hpp"
#include "devices/MmioArea.hpp"
#include "UartRelay.hpp"

#include <cassert>
#include <ios>
#include <sstream>

#define MEMORY_SIZE (128 * 1024 * 1024)
#define MMIO_START 0x7e000000
#define MMIO_LENGTH 0x01000000
#define BOOTROM_START 0x60000000
#define BOOTROM_LENGTH 0x00010000
#define SRAM_START 0x60008000
#define SRAM_LENGTH 0x00008000

#include <iostream>

class RegisterBcm2835 {
public:
	RegisterBcm2835() {
		std::cout << "Registering BCM2835" << std::endl;
		static Bcm2835 device;
		Device::registerDeviceType(&device);
	}
};

static RegisterBcm2835 reg;

MmioRegister systemTimerRegs[] = {
	{ "CS", 0, 4 },
	{ "CLO", 4, 4 },
	{ "CHI", 8, 4 },
	{ "C0", 12, 4 },
	{ "C1", 16, 4 },
	{ "C2", 20, 4 },
	{ "C3", 24, 4 },
};

MmioRegister gpioRegs[] = {
	{ "GPFSEL0", 0x00, 4 },
	{ "GPFSEL1", 0x04, 4 },
	{ "GPFSEL2", 0x08, 4 },
	{ "GPFSEL3", 0x0c, 4 },
	{ "GPFSEL4", 0x10, 4 },
	{ "GPFSEL5", 0x14, 4 },
	{ "GPFSET0", 0x1c, 4 },
	{ "GPFSET1", 0x20, 4 },
	{ "GPFCLR1", 0x28, 4 },
	{ "GPFCLR1", 0x2c, 4 },
};


static MmioRegisterGroup groups[] = {
	{
		"VC_INT_CTRL",
		0x2000,
		0x1000,
		0,
		NULL
	},
	{
		"SYSTEM_TIMER",
		0x3000,
		0x1c,
		sizeof(systemTimerRegs) / sizeof(systemTimerRegs[0]),
		systemTimerRegs
	},
	{
		"DMA",
		0x7000,
		0x1000,
		0,
		NULL
	},
	{
		"INT_CTRL",
		0xB000,
		0x1000,
		0,
		NULL
	},
	{
		"CLK_GEN",
		0x101000,
		0x1000,
		0,
		NULL
	},
	{
		"GPIO",
		0x200000,
		0x1000,
		sizeof(gpioRegs) / sizeof(gpioRegs[0]),
		gpioRegs
	},
	{
		"UART0",
		0x201000,
		0x1000,
		0,
		NULL
	},
	{
		"MMC",
		0x202000,
		0x1000,
		0,
		NULL
	},
	{
		"PCM",
		0x203000,
		0x1000,
		0,
		NULL
	},
	{
		"SPI0",
		0x204000,
		0x1000,
		0,
		NULL
	},
	{
		"BSC0",
		0x205000,
		0x1000,
		0,
		NULL
	},
	{
		"AUX",
		0x215000,
		0x1000,
		0,
		NULL
	},
	{
		"EMMC",
		0x300000,
		0x1000,
		0,
		NULL
	},
	{
		"SMI",
		0x600000,
		0x1000,
		0,
		NULL
	},
	{
		"BSC1",
		0x804000,
		0x1000,
		0,
		NULL
	},
	{
		"BSC2",
		0x805000,
		0x1000,
		0,
		NULL
	},
	{
		"USB",
		0x980000,
		0x1000,
		0,
		NULL
	},
	{
		"DRAM0",
		0xE00000,
		0x1000,
		0,
		NULL
	},
	{
		"DMA15",
		0xE05000,
		0x1000,
		0,
		NULL
	},
	{
		"DRAM1",
		0xE06000,
		0x1000,
		0,
		NULL
	},
	{
		"DRAM2",
		0xE07000,
		0x1000,
		0,
		NULL
	},
};

static MmioDefinition registers = {
	sizeof(groups) / sizeof(groups[0]), groups
};

static MmioDefinition bootromRegs = {
	0, NULL
};

Bcm2835::Bcm2835() : data(NULL), mmio(NULL), sram(NULL), sramData(NULL), uartRelay(NULL) {
	for (unsigned int i = 0; i < 4; i++) {
		memory[i] = NULL;
	}
}
Bcm2835::Bcm2835(const Bcm2835 &other) : data(NULL), mmio(NULL), sram(NULL),
		sramData(NULL), uartRelay(NULL) {
	for (unsigned int i = 0; i < 4; i++) {
		memory[i] = NULL;
	}
}
Bcm2835::~Bcm2835() {
	for (unsigned int i = 0; i < 4; i++) {
		if (memory[i]) {
			delete memory[i];
		}
	}
	if (data) {
		delete[] data;
	}
	if (mmio) {
		delete[] mmio;
	}
	if (sram) {
		delete[] sram;
	}
	if (sramData) {
		delete[] sramData;
	}
	if (uartRelay) {
		delete uartRelay;
	}
}

void Bcm2835::initialize(Log *log, Memory *memory) {
	uartRelay = new UartRelay("192.168.0.100");

	if (data == NULL) {
		data = new uint8_t[MEMORY_SIZE];
	}
	for (unsigned int i = 0; i < 4; i++) {
		uint32_t start = i * 0x40000000;
		// TODO
		this->memory[i] = new RamArea(data, start, start + MEMORY_SIZE);
		memory->registerArea(this->memory[i]);
		log->info("bcm2835", "Added DRAM (0x%08x to 0x%08x).", start,
				start + MEMORY_SIZE);
	}

	mmio = new MmioArea(registers, MMIO_START, MMIO_START + MMIO_LENGTH, log);
	memory->registerArea(mmio);
	log->info("bcm2835", "Added MMIO (0x%08x to 0x%08x).", MMIO_START,
			MMIO_START + MMIO_LENGTH);

	mmio->setCallbacks(&UartRelay::executeWriteStatic,
	                   &UartRelay::executeReadStatic,
	                   uartRelay);

	/*bootrom = new MmioArea(bootromRegs, BOOTROM_START,
			BOOTROM_START + BOOTROM_LENGTH, log);
	memory->registerArea(bootrom);
	log->info("bcm2835", "Added MMIO (0x%08x to 0x%08x).", BOOTROM_START,
			BOOTROM_START + BOOTROM_LENGTH);

	bootrom->setCallbacks(&UartRelay::executeWriteStatic,
	                   &UartRelay::executeReadStatic,
	                   uartRelay);*/
	if (sramData == NULL) {
		sramData = new uint8_t[SRAM_LENGTH];
	}
	sram = new RamArea(sramData, SRAM_START, SRAM_START + SRAM_LENGTH);
	memory->registerArea(sram);
	log->info("bcm2835", "Added DRAM (0x%08x to 0x%08x).", SRAM_START,
			SRAM_START + SRAM_LENGTH);

}

Device *Bcm2835::clone() const {
	return new Bcm2835(*this);
}
