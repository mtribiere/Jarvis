# CMakeLists.txt
#
# CMake file part of the Jarvis Home Automation Project
#######################################################


#Add the project source dir
add_subdirectory(src)


## link binaries
#target_link_libraries(jarvis_executor paho-mqttpp3 paho-mqtt3a pthread nlohmann_json::nlohmann_json)