cmake_minimum_required(VERSION 3.15)

project(ModernLog VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_compile_options(-Wno-error=deprecated-declarations)
add_compile_options(-Wno-deprecated-declarations)

include(ExternalProject)

set(EXTERNAL_INSTALL_LOCATION ${CMAKE_BINARY_DIR}/external)

ExternalProject_Add(fmtlib
    GIT_REPOSITORY https://github.com/fmtlib/fmt
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION}
    UPDATE_DISCONNECTED TRUE
)

ExternalProject_Add(plog
    GIT_REPOSITORY https://github.com/SergiusTheBest/plog
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION}
    UPDATE_DISCONNECTED TRUE
)

include_directories(${EXTERNAL_INSTALL_LOCATION}/include)
link_directories(${EXTERNAL_INSTALL_LOCATION}/lib)

add_executable(modernLog MyPreciousContainer.cpp modernLog.cpp)
add_dependencies(modernLog fmtlib plog)
target_link_libraries(modernLog fmt)
