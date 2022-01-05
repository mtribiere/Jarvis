#include <iostream>
#include <cstdlib>
#include <string>
#include <thread>
#include <atomic>
#include <chrono>
#include <cstring>
#include <stdio.h>
#include <signal.h>
#include <httpserver.hpp>
#include "yaml-cpp/yaml.h"
#include "mqtt/async_client.h"

#include "../include/MQTTClient.hpp"
#include "../include/API.hpp"
#include "../include/Device.hpp"

using namespace httpserver;

#define ENABLE_CONSOLE 0

/////////////////////////////////////////////////////////////////////////////

bool exitFlag = 0;
void signal_callback_handler(int signum);

int main(int argc, char* argv[])
{

	//If config file not specified
	if(argc < 2 ){
		cout << "Usage: " << argv[0] << " <config_file.yaml>" << endl;
		exit(EXIT_SUCCESS); 
	}

	cout << "Starting to load devices...";

	//Load devices configuration
	std::vector<Device::Device> devices;
	Device::LoadConfigFile(argv[1], devices);
	cout << "[OK]" << endl;

	//Display loaded devices
	cout << "Loaded devices (" << devices.size() << ")" << " :" << endl;
	for(int i = 0;i<devices.size();i++){
		cout << devices.at(i);
	}	

	//Declare client
	MQTTClient *client = new MQTTClient();

	//Substribe to topic
	client->subscribeToTopic("stat/+/#");

	webserver ws = create_webserver(8080);
    SetApiHandler setHandler(client);
	GetApiHandler getHandler(client);
    ws.register_resource("\/[a-z]+[0-9]*\/set[0-9]*", &setHandler);
	ws.register_resource("\/[a-z]+[0-9]*\/get[0-9]*", &getHandler);
    ws.start();

	//Register exit signal and loop
	signal(SIGINT, signal_callback_handler);
	while(!exitFlag);

	#if ENABLE_CONSOLE == 1
	
		//TODO: REMAKE THE CONSOLE//
	
	#endif

	//Stopping the server
	cout << endl <<"Stopping API...";
	ws.stop();
	cout << "[OK]" << endl;

	//Free client 
	delete client;

 	return 0;
}

void signal_callback_handler(int signum) {
   cout << "Receiving signal " << signum << endl;
   exitFlag = true;
}