# CMakeLists.txt
#
# CMake file for the Paho C++ sample applications.
#
#*******************************************************************************
# This is part of the Paho MQTT C++ client library.
#
# Copyright (c) 2016-2019
# 
# All rights reserved. This program and the accompanying materials
# are made available under the terms of the Eclipse Public License v1.0
# and Eclipse Distribution License v1.0 which accompany this distribution.
# 
# The Eclipse Public License is available at
#   http://www.eclipse.org/legal/epl-v10.html
# and the Eclipse Distribution License is available at
#   http://www.eclipse.org/org/documents/edl-v10.php.
# 
# Contributors:
#   Guilherme Maciel Ferreira - initial version
#   Frank Pagliughi - Updates for new samples
#*******************************************************************************/

## Note: on OS X you should install XCode and the associated command-line tools

# Project setup
project(Jarvis)

## --- Library dependencies ---
set (THREADS_PREFER_PTHREAD_FLAG ON)
cmake_minimum_required(VERSION 3.18)
set(JSON_BuildTests OFF CACHE INTERNAL "")

find_package(Threads REQUIRED)

#Compile the JSON lib
add_subdirectory(nlohmann_json)

## binary files
add_executable(jarvis_executor main.cpp MQTTClient.cpp)

## link binaries
target_link_libraries(jarvis_executor paho-mqttpp3 paho-mqtt3a pthread nlohmann_json::nlohmann_json)

