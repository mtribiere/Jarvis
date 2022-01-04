#include <iostream>
#include <cstdlib>
#include <string>
#include <thread>
#include <atomic>
#include <chrono>
#include <cstring>
#include <stdio.h>
#include <httpserver.hpp>
#include "mqtt/async_client.h"

#include "../include/MQTTClient.hpp"
#include "../include/API.hpp"

using namespace httpserver;

#define CONSOLE_BUILD 0


/////////////////////////////////////////////////////////////////////////////

int main(int argc, char* argv[])
{
	//Declare client
	MQTTClient *client = new MQTTClient();

	#if CONSOLE_BUILD == 0

	webserver ws = create_webserver(8080);
    SetApiHandler setHandler(client);
	GetApiHandler getHandler(client);
    ws.register_resource("\/[a-z]+[0-9]*\/set[0-9]*", &setHandler);
	ws.register_resource("\/[a-z]+[0-9]*\/get[0-9]*", &getHandler);
    ws.start(true);

	#else
	
		//TODO: REMAKE THE CONSOLE//
	
	#endif

	//Free client 
	delete client;

 	return 0;
}

