cmake_minimum_required(VERSION 3.17)

project(Weather)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND SOURCES
        src/sources/main.cpp
        src/sources/application.cpp
        src/headers/application.hpp
        src/sources/client.cpp
        src/headers/client.hpp
        src/sources/network.cpp
        src/headers/network.hpp
        )

find_package(Boost 1.66 COMPONENTS program_options nowide)

IF (Boost_FOUND)
    add_executable(${PROJECT_NAME} ${SOURCES})
    target_include_directories(${PROJECT_NAME} PRIVATE src/headers ${BOOST_INCLUDEDIR})
    IF (WIN32)
        target_link_libraries(${PROJECT_NAME} Boost::program_options Boost::nowide ws2_32)
    ELSE ()
        target_link_libraries(${PROJECT_NAME} Boost::program_options Boost::nowide)
    ENDIF ()
ELSE ()
    message(FATAL_ERROR "Boost Not Found")
ENDIF ()
