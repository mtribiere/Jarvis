#include "MQTTClient.hpp"

MQTTClient::MQTTClient(){

		cout << "Initializing for server '" << DFLT_SERVER_ADDRESS << "'...";
		client = new mqtt::async_client(DFLT_SERVER_ADDRESS, CLIENT_ID, PERSIST_DIR);

		client->set_callback(cb);

		auto connOpts = mqtt::connect_options_builder()
			.clean_session()
			//.will(mqtt::message(TOPIC, LWT_PAYLOAD, QOS))
			.finalize();

		cout << "  ...OK" << endl;

		try{
			// Connect
			cout << "\nConnecting...";
			mqtt::token_ptr conntok = client->connect(connOpts);
            conntok->wait();
            
            //Subscribe to important topics
            this->subscribeToTopic(NODE_CONNECTION_TOPIC);
		}
		catch (const mqtt::exception& exc) {
			cerr << exc.what() << endl;
		}
}

/**
 * @brief Publish a MQTT message on a topic
 * 
 * @param msg The message
 * @param topic The topic
 */
void MQTTClient::publishMessage(char* msg, char* topic){

    //If not connected
    if(!client->is_connected())
        return;

    try {
        // Publish message with listenner
        cout << "\nSending message...";
        mqtt::message_ptr pubmsg = mqtt::make_message(topic, msg);
        client->publish(pubmsg, nullptr, listener);
        cout << "  ...OK" << endl;
    }
    catch (const mqtt::exception& exc) {
        cerr << exc.what() << endl;
    }

}

void MQTTClient::subscribeToTopic(string topic){

    if(!client->is_connected())
        return;
    
    client->subscribe(topic, QOS, nullptr, listener);
}


MQTTClient::~MQTTClient(){
    try{
        //  Check that there are no pending tokens
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