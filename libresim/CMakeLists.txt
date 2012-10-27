
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
else(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wno-unused-parameter -Woverloaded-virtual -std=c++0x")
endif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")

set(SRC
	include/Device.hpp
	include/Log.hpp
	include/MemoryAccessException.hpp
	include/MemoryAccessMode.hpp
	include/Memory.hpp
	include/Processor.hpp
	include/RamArea.hpp
	include/Simulation.hpp
	include/devices/Bcm2835.hpp
	include/devices/MmioArea.hpp
	include/devices/RamDevice.hpp
	include/videocore/VideoCoreIVProcessor.hpp
	include/videocore/VideoCoreIVRegisterFile.hpp
	src/Device.cpp
	src/Log.cpp
	src/MemoryAccessException.cpp
	src/Memory.cpp
	src/Processor.cpp
	src/RamArea.cpp
	src/Simulation.cpp
	src/devices/Bcm2835.cpp
	src/devices/MmioArea.cpp
	src/devices/RamDevice.cpp
	src/devices/UartRelay.cpp
	src/devices/UartRelay.hpp
	src/videocore/VideoCoreIVDecode.hpp
	src/videocore/VideoCoreIVExecute.hpp
	src/videocore/VideoCoreIVProcessor.cpp
	src/videocore/VideoCoreIVRegisterFile.cpp
)

include_directories(include)

add_library(libresim SHARED ${SRC})
set_target_properties(libresim PROPERTIES OUTPUT_NAME "armsim")
