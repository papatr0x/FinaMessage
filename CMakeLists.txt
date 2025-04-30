# (c) 2025 Patricio Palma <ppalma.dev@protonmail.com>

cmake_minimum_required(VERSION 3.31)
project(FinaMessage)

set(CMAKE_CXX_STANDARD 23)

set(LIB_SOURCES
    src/Format.cpp
    src/MessageWriter.cpp
    src/MessageReader.cpp
    src/DebugTools.cpp
)

set(LIB_HEADERS
    src/Format.h
    src/Field.h
    src/CommonDefinitions.h
    src/Message.h
    src/MessageWriter.h
    src/MessageReader.h
    src/BufferWriter.h
    src/BufferReader.h
    src/DebugTools.h
)

add_library(FinaLib STATIC ${LIB_SOURCES} ${LIB_HEADERS})
target_compile_options(FinaLib PRIVATE -Wall -Wextra -pedantic -Werror)

add_executable(pos_example test/POS_example.cpp)
target_link_libraries(pos_example PRIVATE FinaLib)
target_include_directories(pos_example PUBLIC src)
