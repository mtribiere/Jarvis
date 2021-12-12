#include <iostream>
#include <cstdlib>
#include <string>
#include <thread>
#include <atomic>
#include <chrono>
#include <cstring>
#include <stdio.h>
#include "mqtt/async_client.h"

#include "MQTTClient.hpp"

/////////////////////////////////////////////////////////////////////////////

int main(int argc, char* argv[])
{

	bool needExit = false;
	char buff[255];

	//Declare client
	MQTTClient *client = new MQTTClient();

	//While not quit command
	while(!needExit){
		
		//Print the bash line and ask for command
		cout<<"JV> ";
		scanf("%254s",buff);

		//If quit command
		if(strcmp(buff,"/quit") == 0){
			needExit = true;
			break;
		}

		//If not quit commmand, publish the message
		client->publishMessage(buff,"/home/light");

	}

	//Free client 
	delete client;

 	return 0;
}

