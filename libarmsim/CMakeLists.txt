
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
	include/Simulation.hpp
	src/Log.cpp
	src/MemoryAccessException.cpp
	src/Memory.cpp
	src/Processor.cpp
	src/Simulation.cpp
)

include_directories(include)

add_library(libarmsim SHARED ${SRC})
set_target_properties(libarmsim PROPERTIES OUTPUT_NAME "armsim")
