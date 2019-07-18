cmake_minimum_required(VERSION 3.5)
project(toys C)

set(CMAKE_C_STANDARD 99)

add_subdirectory(elfparser)
add_subdirectory(shell)
add_subdirectory(fdviewer)
add_subdirectory(snippets)