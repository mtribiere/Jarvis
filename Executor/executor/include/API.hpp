#ifndef API_HPP
#define API_HPP

#include <httpserver.hpp>
#include "../include/MQTTClient.hpp"

using namespace httpserver;

/**
 * @brief The handler used to set a parameter on a device
 * 
 */
class SetApiHandler : public http_resource {
public:
	SetApiHandler(MQTTClient* _client) : client(_client) {};
    const std::shared_ptr<http_response> render_POST(const http_request& req);

private:
	MQTTClient *client;
};


/**
 * @brief The handler used to get a parameter from a device
 * 
 */
class GetApiHandler : public http_resource {
public:
	GetApiHandler(MQTTClient* _client) : client(_client) {};
    const std::shared_ptr<http_response> render_POST(const http_request& req);

private:
	MQTTClient *client;

};

/**
 * @brief The general purpuse handler for the API
 * 
 */
class GPApiHandler : public http_resource {
public:
	GPApiHandler(MQTTClient* _client) : client(_client) {};
    const std::shared_ptr<http_response> render(const http_request& req);

private:
	MQTTClient *client;

};


#endif