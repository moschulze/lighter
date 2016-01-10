cmake_minimum_required(VERSION 3.2)
project(lighter)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(
        lib/rapidjson/include
        lib/easyloggingpp/src
)

find_package(Threads)

set(SOURCE_FILES src/main.cpp src/configuration/Configuration.cpp src/configuration/Configuration.h src/configuration/ConfigurationLoader.cpp src/configuration/ConfigurationLoader.h src/Kernel.cpp src/Kernel.h src/http/HttpServer.cpp src/http/HttpServer.h src/http/ClientProcessor.cpp src/http/ClientProcessor.h src/http/HttpRequest.cpp src/http/HttpRequest.h src/device/DeviceType.cpp src/device/DeviceType.h src/device/DeviceTypeRepository.cpp src/device/DeviceTypeRepository.h src/device/Device.cpp src/device/Device.h src/device/DeviceRepository.cpp src/device/DeviceRepository.h src/interface/Interface.cpp src/interface/Interface.h src/dmx/DmxUniverse.cpp src/dmx/DmxUniverse.h)
add_executable(lighter ${SOURCE_FILES})
target_link_libraries(lighter ${CMAKE_THREAD_LIBS_INIT})