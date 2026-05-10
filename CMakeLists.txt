cmake_minimum_required(VERSION 4.1)
project(C_TCP_SERVER_CLIENT C)

set(CMAKE_C_STANDARD 11)

add_executable(C_TCP_SERVER_CLIENT server.c
        client.c)
