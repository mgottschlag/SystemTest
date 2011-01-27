
project(SystemTest)
cmake_minimum_required(VERSION 2.6.0)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wno-unused-parameter")

find_package(Qt4 REQUIRED)

set(QT_USE_GUI FALSE)
include(${QT_USE_FILE})

# Common shared library

set(SRC
	main.cpp
	Test.cpp
	TestSuite.cpp
)

set(MOC_H
	Test.hpp
	TestSuite.hpp
)

QT4_WRAP_CPP(MOC_SRC ${MOC_H})

set(QRC
)

QT4_ADD_RESOURCES(QRC_SRC ${QRC})

set(UIS
)

QT4_WRAP_UI(UIS_H ${UIS})

add_executable(SystemTest ${SRC} ${UIS_H} ${MOC_SRC} ${QRC_SRC})
target_link_libraries(SystemTest ${QT_LIBRARIES})
