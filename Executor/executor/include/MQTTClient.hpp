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
#include <pthread.h>
#include <sys/time.h>
#include <thread> 
#include <future>

#include "../include/Device.hpp"
#include "Callback.hpp"

using namespace std;

#define CLIENT_ID "Jarvis_Executor"
#define QOS 1

class MQTTClient{

	private:
		const string DFLT_SERVER_ADDRESS	{ "tcp://localhost:1883" };
		const string PERSIST_DIR			{ "./persist" };

		const std::chrono::duration<int64_t> TIMEOUT = std::chrono::seconds(10);
		mqtt::async_client *client = nullptr;
		callback *cb;

		std::vector<Device::Device> *devices = nullptr;

		bool *threadExitFlag = nullptr;
		std::thread *messageConsumerThread = nullptr;
		pthread_cond_t *newMessageSignal = nullptr;
		pthread_mutex_t *messageConsumerMutex = nullptr;


	
	public: 
		MQTTClient(std::vector<Device::Device> * _devices);
		void publishMessage(string msg, string topic);
		void subscribeToTopic(string topic); 
		void subscribeToDeviceTopic(Device::Device device);
		Device::REQUEST_RESPONSE getInfoFromDevice(string device, string info, string id, string& response);
		static void messageConsumerFunc(bool *threadExitFlag, std::vector<Device::Device> *devices, callback *cb, pthread_mutex_t* messageConsumerMutex,pthread_cond_t* newMessageSignal);

		~MQTTClient();


};

#endif