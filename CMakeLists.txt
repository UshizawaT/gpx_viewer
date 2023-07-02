cmake_minimum_required(VERSION 3.1)
project(gpt2tgraph)

set(CMAKE_CXX_FLAGS "-Ofast -mtune=native -march=native -fdiagnostics-color=always")
 
find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
message("boost_include : ${Boost_INCLUDE_DIRS}")

find_package(XercesC REQUIRED)
include_directories(${XercesC_INCLUDE_DIRS})
message("XercesC_include : ${XercesC_INCLUDE_DIRS}")
message("XercesC_lib : ${XercesC_LIBRARIES}")
 
#find_package(yaml-cpp REQUIRED)
#set(YAML_CPP_INCLUDE_DIR "/opt/homebrew/opt/yaml-cpp/include")
#include_directories(${YAML_CPP_INCLUDE_DIR})
#find_library(YAML_CPP_LIBRARIES, NAMES yaml-cpp)
#set(YAML_CPP_LIBRARIES "/opt/homebrew/opt/yaml-cpp/lib/libyaml-cpp.dylib")
#message("yamlinclude : ${YAML_CPP_INCLUDE_DIR}")
#message("libyaml : ${YAML_CPP_LIBRARIES}")
 
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
 
include_directories(${PROJECT_SOURCE_DIR}/include)
 
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cpp)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hpp)
 
add_library(geolibs SHARED ${sources} ${headers})
#target_link_libraries(geolibs ${Boost_LIBRARIES} ${YAML_CPP_LIBRARIES} ${XercesC_LIBRARIES})
target_link_libraries(geolibs ${Boost_LIBRARIES} ${XercesC_LIBRARIES})
add_executable(gpt2tgraph main.cpp)
target_link_libraries(gpt2tgraph geolibs)