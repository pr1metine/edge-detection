cmake_minimum_required(VERSION 3.27.7)
project("edge-detection")

set (CMAKE_PREFIX_PATH /opt/homebrew/Cellar/systemc/2.3.4)
set (CMAKE_EXPORT_COMPILE_COMMANDS ON)
find_package(SystemCLanguage CONFIG REQUIRED)
find_package(Boost 1.83 REQUIRED COMPONENTS ALL)
find_package(PNG REQUIRED)

set (CMAKE_CXX_STANDARD ${SystemC_CXX_STANDARD} CACHE STRING
     "C++ standard to build all targets. Supported values are 98, 11, 14, and 17.")
set (CMAKE_CXX_STANDARD_REQUIRED ${SystemC_CXX_STANDARD_REQUIRED} CACHE BOOL
     "The with CMAKE_CXX_STANDARD selected C++ standard is a requirement.")

add_library(edge-detection-lib
    src/convolution/Filter.cpp 
    )
target_link_libraries(edge-detection-lib SystemC::systemc ${PNG_LIBRARIES} Boost::boost)
target_include_directories(edge-detection-lib PUBLIC "include" PRIVATE ${PNG_INCLUDE_DIR})

add_executable(edge-detection src/bin/edge-detection.cpp)
target_link_libraries(edge-detection edge-detection-lib)