#include "MQTTClient.hpp"

MQTTClient::MQTTClient(){

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


void MQTTClient::publishMessage(char* msg, char* topic){
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


MQTTClient::~MQTTClient(){
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