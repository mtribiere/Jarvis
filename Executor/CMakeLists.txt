project(Jarvis_executor)

# Project setup
cmake_minimum_required(VERSION 3.18)
project(executor)

## --- Library dependencies ---
set (THREADS_PREFER_PTHREAD_FLAG ON)
set(JSON_BuildTests OFF CACHE INTERNAL "")

find_package(Threads REQUIRED)

#Set install dir
set(CMAKE_INSTALL_PREFIX .)

#Compile lib and code
add_subdirectory(executor)
add_subdirectory(nlohmann_json)