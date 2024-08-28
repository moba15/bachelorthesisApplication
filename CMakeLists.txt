cmake_minimum_required(VERSION 3.30)
project(bachelorThesisAplication)
find_package(doctest REQUIRED)
include(doctest)
set(CMAKE_CXX_STANDARD 26)

add_executable(bachelorThesisAplication main.cpp
        server/hashmap/hashmap.h
        server/hashmap/hashmap.cpp
        server/server.h
        server/server.cpp

        shared/shared.h
        client/client.h
        client/client.cpp)


add_executable(testHashMap test.cpp
        server/hashmap/hashmap.h

        server/server.h
        server/server.cpp

        shared/shared.h
        client/client.h
        client/client.cpp)
target_link_libraries(testHashMap PRIVATE doctest::doctest pthread)