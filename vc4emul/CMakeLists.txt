
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
else(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wno-unused-parameter -Woverloaded-virtual -std=c++0x")
endif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")

find_package(Qt4 REQUIRED QtCore QtGui)
include(${QT_USE_FILE})

include_directories(${CMAKE_CURRENT_BINARY_DIR})


set(SRC
	main.cpp
)

set(MOC_H
)
QT4_WRAP_CPP(MOC_SRC ${MOC_H})

set(QRC
)
QT4_ADD_RESOURCES(QRC_SRC ${QRC})

set(UIS
)
QT4_WRAP_UI(UIS_H ${UIS})

include_directories(../libresim/include)

add_executable(vc4emul ${SRC} ${UIS_H} ${MOC_SRC} ${QRC_SRC})
target_link_libraries(vc4emul libresim ${QT_LIBRARIES})

