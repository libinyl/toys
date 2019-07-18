cmake_minimum_required(VERSION 3.5)
project(toys C)

set(CMAKE_C_STANDARD 99)

add_subdirectory(elfparser)
add_subdirectory(osdetector)
add_subdirectory(clearn)
add_subdirectory(shell)
add_subdirectory(fdviewer)