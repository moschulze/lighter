cmake_minimum_required(VERSION 3.2)
project(lighter)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(
        lib/rapidjson/include
        lib/easyloggingpp/src
)

set(SOURCE_FILES src/main.cpp src/configuration/Configuration.cpp src/configuration/Configuration.h src/configuration/ConfigurationLoader.cpp src/configuration/ConfigurationLoader.h)
add_executable(lighter ${SOURCE_FILES})