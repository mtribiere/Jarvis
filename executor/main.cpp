#include <iostream>
#include <cstdlib>
#include <string>
#include <thread>
#include <atomic>
#include <chrono>
#include <cstring>
#include <stdio.h>
#include <nlohmann/json.hpp>
#include "mqtt/async_client.h"

#include "MQTTClient.hpp"
#include "Base64.hpp"
#define CROW_MAIN
#include "crow_all.h"


#define CONSOLE_BUILD 0

using json = nlohmann::json;

/////////////////////////////////////////////////////////////////////////////

int main(int argc, char* argv[])
{
	//Declare client
	MQTTClient *client = new MQTTClient();

	//Create the default packet
	json message;
	message["intent"] = "setProperty";
	message["property"] = "onStatus";
	message["value"] = 1;
	message["targetNode"] = "0";
	message["targetDevice"] = "0"; 

	#if CONSOLE_BUILD == 0

	//Create front-end Web serve
	crow::SimpleApp app;
	CROW_ROUTE(app, "/<string>")([client,&message](string parameter){

		//Perform complex analysis (One day ?)
		string decoded;
		macaron::Base64::Decode(parameter,decoded);

		//If christmas related
		if(decoded.find("christmas") != std::string::npos || decoded.find("Christmas") != std::string::npos){
			
			//If off command
			if(decoded.find("not") != std::string::npos){ 
				message["value"] = 0;
			}else{
				message["value"] = 1;
			}

			//Create buffer
			char buff[255];
			string strMsg = message.dump();
			strcpy(buff,strMsg.c_str());

			//If not quit commmand, publish the message
			client->publishMessage(buff,"/home/light");

			return (message["value"] == 1 ? "True, here we go!!":"That's kinda sad");
		}

		//If thanks
		if(decoded.find("Thank") != std::string::npos ){
			return "You welcome";
		}

		return "Bro I don't get it...Code me better";;

	});
	app.bindaddr("192.168.43.166").port(8080).multithreaded().run();

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

