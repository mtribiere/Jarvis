include_directories(${Jarvis_executor_SOURCE_DIR}/executor/include/)
include_directories(${Jarvis_executor_SOURCE_DIR}/nlohmann_json/include/)
link_directories(${Jarvis_executor_SOURCE_DIR}/nlohmann_json/src)

file(GLOB SRCS
    "${Jarvis_executor_SOURCE_DIR}/executor/src/*.cpp")

add_executable(jarvis_executor ${SRCS})

target_link_libraries(jarvis_executor paho-mqttpp3 paho-mqtt3a pthread nlohmann_json::nlohmann_json)

#Install instruction
install(TARGETS jarvis_executor DESTINATION bin)