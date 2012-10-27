
#include "UartRelay.hpp"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <netinet/tcp.h>
#include <cstring>
#include <stdexcept>
#include <sstream>
#include <unistd.h>
#include <cassert>
#include <cstdlib>

UartRelay::UartRelay(const std::string &address) {
	// Resolve the address
	struct addrinfo hints, *result;
	std::memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	int ret = getaddrinfo(address.c_str(), "12365", &hints, &result);
	if (ret < 0) {
		std::ostringstream msg;
		msg << "getaddrinfo failed: " << gai_strerror(ret);
		throw std::runtime_error(msg.str());
	}
	if (result == NULL) {
		throw std::runtime_error("Could not resolve the address.");
	}
	// Create the socket and connect
	sock = socket(result->ai_family, result->ai_socktype,
			result->ai_protocol);
	if (sock < 0) {
		throw std::runtime_error("socket failed.");
	}
	int on = 1;
	if (setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (char*)&on,
			sizeof(on)) < 0) {
		throw std::runtime_error("setsockopt failed.");
	}
	if (connect(sock, result->ai_addr, result->ai_addrlen) < 0) {
		throw std::runtime_error("connect failed.");
	}
	freeaddrinfo(result);
}
UartRelay::~UartRelay() {
	close(sock);
}

void UartRelay::executeWrite(uintptr_t address,
                             uint64_t value,
                             unsigned int size) {
	if (size != 4) {
		throw std::runtime_error("Unimplemented memory write operand size.");
	}
	sendCommand('w', address, value);
}
uint64_t UartRelay::executeRead(uintptr_t address,
                                unsigned int size) {
	if (size != 4) {
		throw std::runtime_error("Unimplemented memory read operand size.");
	}
	return sendCommand('r', address, 0x00000000);
}

uint32_t UartRelay::sendCommand(char type, uint32_t address, uint32_t value) {
	std::ostringstream stream;
	stream << type;
	stream.width(8);
	stream.fill('0');
	stream << std::hex << address;
	if (type != 'r') {
		stream.width(8);
		stream.fill('0');
		stream << std::hex << value;
	}
	std::string msg = stream.str();
	// Send the command
	unsigned int sent = 0;
	while (sent != msg.length()) {
		int ret = send(sock, msg.c_str() + sent, msg.length() - sent, 0);
		if (ret < 0) {
			throw std::runtime_error("send failed.");
		}
		sent += ret;
	}
	// Wait for a response
	if (type != 'r') {
		return 0;
	}
	int received = 0;
	char response[9];
	while (received != 8) {
		int ret = recv(sock, response + received, 8 - received, 0);
		if (ret < 0) {
			throw std::runtime_error("recv failed.");
		}
		if (ret == 0) {
			throw std::runtime_error("Relay disconnected.");
		}
		received += ret;
	}
	response[8] = 0;
	return std::strtoul(response, NULL, 16);
}
