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

	
	const string DFLT_SERVER_ADDRESS	{ "tcp://192.168.43.166:1883" };
	const string CLIENT_ID				{ "paho_cpp_async_publish" };
	const string PERSIST_DIR			{ "./persist" };

	const string TOPIC { "/home/light" };

	const char* PAYLOAD3 = "Is anyone listening?";
	const char* LWT_PAYLOAD = "Last will and testament.";

	const int  QOS = 1;

	const std::chrono::duration<int64_t> TIMEOUT = std::chrono::seconds(10);
	mqtt::async_client *client;
	
	public: 
	MQTTClient(){

		cout << "Initializing for server '" << DFLT_SERVER_ADDRESS << "'...";
		client = new mqtt::async_client(DFLT_SERVER_ADDRESS, CLIENT_ID, PERSIST_DIR);


		callback cb;
		client->set_callback(cb);

		auto connOpts = mqtt::connect_options_builder()
			.clean_session()
			.will(mqtt::message(TOPIC, LWT_PAYLOAD, QOS))
			.finalize();

		cout << "  ...OK" << endl;

		try{
			// Connect
			cout << "\nConnecting...";
			mqtt::token_ptr conntok = client->connect(connOpts);
			cout << "Waiting for the connection...";
			conntok->wait();
			cout << "  ...OK" << endl;

		}
		catch (const mqtt::exception& exc) {
			cerr << exc.what() << endl;
		}

	}

	void publishMessage(char* msg, char* topic){
		try {
			// Publish message with listenner
			cout << "\nSending message..." << endl;
			action_listener listener;
			mqtt::delivery_token_ptr pubtok;
			mqtt::message_ptr pubmsg = mqtt::make_message(topic, msg);
			pubtok = client->publish(pubmsg, nullptr, listener);
			pubtok->wait();
			cout << "  ...OK" << endl;

		}
		catch (const mqtt::exception& exc) {
			cerr << exc.what() << endl;
		}

	}

	~MQTTClient(){
		try{
			// Double check that there are no pending tokens
			auto toks = client->get_pending_delivery_tokens();
			if (!toks.empty())
				cout << "Error: There are pending delivery tokens!" << endl;

			// Disconnect
			cout << "\nDisconnecting...";
			client->disconnect()->wait();
			cout << "  ...OK" << endl;
		}
		catch (const mqtt::exception& exc) {
			cerr << exc.what() << endl;
		}
	}
};