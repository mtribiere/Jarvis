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
#include <pthread.h>
#include <sys/time.h>

#include "../include/Device.hpp"

using namespace std;

#define NODE_TELE_TOPIC "tele/#"
#define CLIENT_ID "Jarvis_Executor"
#define QOS 1

class MQTTClient{

	private:
		const string DFLT_SERVER_ADDRESS	{ "tcp://localhost:1883" };
		const string PERSIST_DIR			{ "./persist" };

		const std::chrono::duration<int64_t> TIMEOUT = std::chrono::seconds(10);
		mqtt::async_client *client;
		callback cb;

		std::vector<Device::Device> devices;
	
	public: 
		MQTTClient(std::vector<Device::Device> _devices);
		void publishMessage(string msg, string topic);
		void subscribeToTopic(string topic);
		string getInfoFromDevice(string device, string info, string id);
		
		~MQTTClient();


};

#endif