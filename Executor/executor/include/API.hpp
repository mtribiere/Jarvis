#ifndef API_HPP
#define API_HPP

#include "../include/MQTTClient.hpp"
#include <httpserver.hpp>

using namespace httpserver;

class SetApiHandler : public http_resource {
public:
	SetApiHandler(MQTTClient* _client) : client(_client) {};

    const std::shared_ptr<http_response> render_POST(const http_request& req) {
		//Get the URI
		string uri = req.get_path();
		uri = uri.substr(1,uri.size()-1);
		
		//Get the target device
		string device = uri.substr(0,uri.find('/'));
		std::cout << "Set request for " << device << " -> " << req.get_arg("state") <<std::endl;

		client->publishMessage(req.get_arg("state"),"cmnd/"+device+"/POWER");

		//Send response
		std::shared_ptr<http_response> rep(new string_response("OK",200));
		rep->with_header("Access-Control-Allow-Origin","*");
		
		return rep;
	}

	MQTTClient *client;
};

class GetApiHandler : public http_resource {
public:
	GetApiHandler(MQTTClient* _client) : client(_client) {};
    const std::shared_ptr<http_response> render_POST(const http_request& req) {
		//Get the URI
		string uri = req.get_path();
		uri = uri.substr(1,uri.size()-1);
		
		//Get the target device
		string device = uri.substr(0,uri.find('/'));
		std::cout << "Get request for " << device;

		client->publishMessage("","cmnd/"+device+"/POWER");
		

		//Send response
		std::shared_ptr<http_response> rep(new string_response("OK",200));
		rep->with_header("Access-Control-Allow-Origin","*");
		
		return rep;
	}

	MQTTClient *client;
};


#endif