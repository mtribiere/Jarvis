include_directories(${Jarvis_executor_SOURCE_DIR}/executor/include/)
set (CMAKE_CXX_STANDARD 14)

file(GLOB SRCS
    "${Jarvis_executor_SOURCE_DIR}/executor/src/*.cpp")

add_executable(jarvis_executor ${SRCS})

target_link_libraries(jarvis_executor paho-mqttpp3 paho-mqtt3a pthread yaml-cpp httpserver)

#Install instruction
install(TARGETS jarvis_executor DESTINATION bin)