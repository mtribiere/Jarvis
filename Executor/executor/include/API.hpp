#ifndef API_HPP
#define API_HPP

#include <httpserver.hpp>
#include "../include/MQTTClient.hpp"

using namespace httpserver;

class SetApiHandler : public http_resource {
public:
	SetApiHandler(MQTTClient* _client) : client(_client) {};
    const std::shared_ptr<http_response> render_POST(const http_request& req);

private:
	MQTTClient *client;
};

class GetApiHandler : public http_resource {
public:
	GetApiHandler(MQTTClient* _client) : client(_client) {};
    const std::shared_ptr<http_response> render_POST(const http_request& req);

private:
	MQTTClient *client;

};


#endif