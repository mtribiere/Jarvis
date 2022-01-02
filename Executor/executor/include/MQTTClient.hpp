#ifndef MQTTCLIENT_HPP
#define MQTTCLIENT_HPP

#include <iostream>
#include <cstdlib>
#include <string>
#include <thread>
#include <atomic>
#include <chrono>
#include <cstring>
#include <stdio.h>
#include "mqtt/async_client.h"
#include "Callback.hpp"

using namespace std;

#define NODE_CONNECTION_TOPIC "/home/nodes/connect"
#define CLIENT_ID "Jarvis_Executor"
#define QOS 1

class MQTTClient{

	
	const string DFLT_SERVER_ADDRESS	{ "tcp://localhost:1883" };
	const string PERSIST_DIR			{ "./persist" };

	const char* LWT_PAYLOAD = "Jarvis_Executor_disconnected";

	const std::chrono::duration<int64_t> TIMEOUT = std::chrono::seconds(10);
	mqtt::async_client *client;
	action_listener listener;
	callback cb;
	
	public: 
		MQTTClient();
		void publishMessage(char* msg, char* topic);
		void subscribeToTopic(string topic);
		~MQTTClient();


};

#endif