
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
else(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wno-unused-parameter -Woverloaded-virtual -std=c++0x")
endif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")

find_package(Qt4 REQUIRED QtCore QtGui)
include(${QT_USE_FILE})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_BINARY_DIR} ${QT_QTOPENGL_INCLUDE_DIR})


set(SRC
	src/main.cpp
	src/MainWindow.cpp
)

set(MOC_H
	src/MainWindow.hpp
)
QT4_WRAP_CPP(MOC_SRC ${MOC_H})

set(QRC
	armsim.qrc
)
QT4_ADD_RESOURCES(QRC_SRC ${QRC})

set(UIS
	ui/MainWindow.ui
)
QT4_WRAP_UI(UIS_H ${UIS})

add_executable(armsim ${SRC} ${UIS_H} ${MOC_SRC} ${QRC_SRC})
target_link_libraries(armsim libarmsim ${QT_LIBRARIES})
