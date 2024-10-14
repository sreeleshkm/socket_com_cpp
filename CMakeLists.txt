cmake_minimum_required(VERSION 3.0)

project(SOCKET_COMMUNICATION_CPP)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# set(CMAKE_CXX_FLAGS_DEBUG -g)
set(CMAKE_CXX_COMPILER g++)

set (SOC_CLIENT 0)
set (SOC_SERVER 1)

set (SOCKET_CO ${SOC_CLIENT})
set (SOURCE_CLIENT main.cpp client.cpp socket_common.cpp)
add_executable(client ${SOURCE_CLIENT})
target_compile_definitions(client PRIVATE -DSOCKET_COM=${SOCKET_CO})

set (SOCKET_CO ${SOC_SERVER})
set (SOURCE_SERVER main.cpp server.cpp socket_common.cpp)
add_executable(server ${SOURCE_SERVER})
target_compile_definitions(server PRIVATE -DSOCKET_COM=${SOCKET_CO})

