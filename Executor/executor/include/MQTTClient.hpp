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

using namespace std;

/////////////////////////////////////////////////////////////////////////////

/**
 * A callback class for use with the main MQTT client.
 */
class callback : public virtual mqtt::callback
{
public:
	void connection_lost(const string& cause) override {
		cout << "\nConnection lost" << endl;
		if (!cause.empty())
			cout << "\tcause: " << cause << endl;
	}

	void delivery_complete(mqtt::delivery_token_ptr tok) override {
		cout << "\tDelivery complete for token: "
			<< (tok ? tok->get_message_id() : -1) << endl;
	}
};

/////////////////////////////////////////////////////////////////////////////

/**
 * A base action listener.
 */
class action_listener : public virtual mqtt::iaction_listener
{
protected:
	void on_failure(const mqtt::token& tok) override {
		cout << "\tListener failure for token: "
			<< tok.get_message_id() << endl;
	}

	void on_success(const mqtt::token& tok) override {
		cout << "\tListener success for token: "
			<< tok.get_message_id() << endl;
	}
};

/////////////////////////////////////////////////////////////////////////////

/**
 * A derived action listener for publish events.
 */
class delivery_action_listener : public action_listener
{
	atomic<bool> done_;

	void on_failure(const mqtt::token& tok) override {
		action_listener::on_failure(tok);
		done_ = true;
	}

	void on_success(const mqtt::token& tok) override {
		action_listener::on_success(tok);
		done_ = true;
	}

public:
	delivery_action_listener() : done_(false) {}
	bool is_done() const { return done_; }
};

class MQTTClient{

	
	const string DFLT_SERVER_ADDRESS	{ "tcp://localhost:1883" };
	const string CLIENT_ID				{ "paho_cpp_async_publish" };
	const string PERSIST_DIR			{ "./persist" };

	const string TOPIC { "/home/light" };
	const char* LWT_PAYLOAD = "Jarvis_Executor_disconnected";

	const int  QOS = 1;

	const std::chrono::duration<int64_t> TIMEOUT = std::chrono::seconds(10);
	mqtt::async_client *client;
	
	public: 
		MQTTClient();
		void publishMessage(char* msg, char* topic);
		~MQTTClient();


};

#endif