
#ifndef RESIM_BCM2835_HPP_INCLUDED
#define RESIM_BCM2835_HPP_INCLUDED

#include "../Device.hpp"

#include <stdint.h>

class RamArea;
class MmioArea;
class UartRelay;

class Bcm2835 : public Device {
public:
	Bcm2835();
	Bcm2835(const Bcm2835 &other);
	~Bcm2835();

	virtual void initialize(Log *log, Memory *memory);

	virtual const char *getName() const {
		return "BCM2835";
	};

	virtual Device *clone() const;
private:
	RamArea *memory[4];
	uint8_t *data;

	MmioArea *mmio;
	MmioArea *bootrom;
	RamArea *sram;
	uint8_t *sramData;

	UartRelay *uartRelay;
};

#endif
