#include <iostream>
#include <cstdlib>
#include <string>
#include <thread>
#include <atomic>
#include <chrono>
#include <cstring>
#include <stdio.h>
#include "mqtt/async_client.h"

#include "../include/MQTTClient.hpp"


#define CONSOLE_BUILD 0

/////////////////////////////////////////////////////////////////////////////

int main(int argc, char* argv[])
{
	//Declare client
	MQTTClient *client = new MQTTClient();

	#if CONSOLE_BUILD == 0



	#else
	//Create input buffer
	char buff[255];
	
	//While not quit command
	while(true){
		
		//Print the bash line and ask for command
		cout<<"JV> ";
		scanf("%254s",buff);

		//If quit command
		if(strcmp(buff,"/quit") == 0){
			break;
		}

		//If on command
		if(strcmp(buff,"on") == 0){
			message["value"] = 1;

		}else if(strcmp(buff,"off") == 0){ //If off command
			message["value"] = 0;
		}else continue;
		

		string strMsg = message.dump();
		strcpy(buff,strMsg.c_str());

		//If not quit commmand, publish the message
		client->publishMessage(buff,"/home/light");

	}
	#endif

	//Free client 
	delete client;

 	return 0;
}

