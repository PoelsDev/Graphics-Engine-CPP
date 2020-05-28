cmake_minimum_required(VERSION 3.5)

project(engine)

############################################################
# Set compiler flags
############################################################
set(OWN_GXX_FLAGS "-Wall -Wextra -fstack-protector-all -std=c++14")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${OWN_GXX_FLAGS} -pg -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${OWN_GXX_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OWN_GXX_FLAGS}")

############################################################
# List all sources
############################################################
set(engine_sources
        src/easy_image.cc
        src/engine.cc
        src/ini_configuration.cc
        src/lineUtils.cpp
        src/utils.h
        src/utils.cpp
        src/lineUtils.h
        src/l_parser.cc
        src/l_parser.h
        src/3DLineUtils.cpp
        src/3DLineUtils.cpp
        src/vector3d.cc
        src/vector3d.h
        src/solids.cpp
        src/solids.h
        src/zBufferUtils.h
        src/zBufferUtils.cpp
        src/library.h
        src/Color.cpp
        src/Color.h
        src/Lighting.cpp
        src/Lighting.h
        )

############################################################
# Create an executable
############################################################
set(exe_name "engine")
add_executable(${exe_name} ${engine_sources})
install(TARGETS ${exe_name} DESTINATION ${PROJECT_SOURCE_DIR}/)
