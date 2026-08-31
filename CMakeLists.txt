cmake_minimum_required(VERSION 3.16)
project(qt-echo-server LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_PREFIX_PATH "$ENV{HOME}/Qt/6.10.1/gcc_64")

find_package(Qt6 COMPONENTS Core Network REQUIRED)

qt_standard_project_setup()


# server
add_executable(qt-echo-server
	${CMAKE_CURRENT_SOURCE_DIR}/src/qt_echo_server.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/echo_server.cpp

	# for MOC
	${CMAKE_CURRENT_SOURCE_DIR}/include/echo_server.hpp
)

target_include_directories(qt-echo-server
	PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(qt-echo-server PRIVATE Qt6::Core Qt6::Network)

set_target_properties(qt-echo-server PROPERTIES
	AUTOMOC ON
)

# client
add_executable(qt-echo-client
	${CMAKE_CURRENT_SOURCE_DIR}/src/echo_client.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/qt_echo_client.cpp

	# for MOC
	${CMAKE_CURRENT_SOURCE_DIR}/include/echo_client.hpp
)

target_include_directories(qt-echo-client
	PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(qt-echo-client PRIVATE Qt6::Core Qt6::Network)

set_target_properties(qt-echo-client PROPERTIES
	AUTOMOC ON
)
