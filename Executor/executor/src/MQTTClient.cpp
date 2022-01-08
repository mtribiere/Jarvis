#include "MQTTClient.hpp"

/**
 * @brief Construct a new MQTTClient::MQTTClient object
 * 
 * @param _devices The known devices
 */
MQTTClient::MQTTClient(std::vector<Device::Device> *_devices){

        //Create the callbak structure
		cb = new callback();

        //Create the Async client
        cout << "Initializing for server '" << DFLT_SERVER_ADDRESS << "'...";
		client = new mqtt::async_client(DFLT_SERVER_ADDRESS, CLIENT_ID, PERSIST_DIR);
        
		client->set_callback(*cb);

		auto connOpts = mqtt::connect_options_builder()
			.clean_session()
			.finalize();

		cout << "[OK]" << endl;

		try{
			// Connect
			cout << "\nConnecting...";
			mqtt::token_ptr conntok = client->connect(connOpts);
            conntok->wait();
            cout << "[OK]" << endl;

            //Set device list
            this->devices = _devices;

            //Subscibe to device topics
            for(int i = 0;i<(*devices).size();i++)
                subscribeToDeviceTopic((*devices)[i]);
            
		}
		catch (const mqtt::exception& exc) {
			cerr << exc.what() << endl;
		}

        //Spawm the consumer thread
        newMessageSignal = new pthread_cond_t();
        messageConsumerMutex = new pthread_mutex_t();

        pthread_mutex_init(messageConsumerMutex, NULL);
        pthread_cond_init(newMessageSignal, NULL);

        threadExitFlag = new bool(false);
        messageConsumerThread = new std::thread(&messageConsumerFunc, threadExitFlag,devices,cb,messageConsumerMutex,newMessageSignal);
}

void MQTTClient::subscribeToDeviceTopic(Device::Device device){
    this->subscribeToTopic("stat/"+device.realName+"/#");
    this->subscribeToTopic("tele/"+device.realName+"/#");

}

/**
 * @brief Publish a MQTT message on a topic
 * 
 * @param msg The message
 * @param topic The topic
 */
void MQTTClient::publishMessage(string msg, string topic){

    //If not connected
    if(!client->is_connected())
        return;

    try {
        cout << "\nSending message...";
        mqtt::message_ptr pubmsg = mqtt::make_message(topic, msg);
        client->publish(pubmsg, nullptr, *cb);
        cout << "[OK]" << endl;
    }
    catch (const mqtt::exception& exc) {
        cerr << exc.what() << endl;
    }

}

/**
 * @brief Subscribe to a topic
 * 
 * @param topic 
 */
void MQTTClient::subscribeToTopic(string topic){

    if(!client->is_connected())
        return;
    
    client->subscribe(topic, QOS, nullptr, *cb);
}

/**
 * @brief Get an info from a device
 * 
 * @param device The device name
 * @param info The desired info
 * @param id The ID of the target on the device
 * @param response The response of the request. "" if error.
 * @return Device::REQUEST_RESPONSE The response code of the request
 */
Device::REQUEST_RESPONSE MQTTClient::getInfoFromDevice(string device, string info, string id, string& response){

    //If not connected
    if(!client->is_connected())
        return Device::REQUEST_RESPONSE::INTERNAL_ERROR;

    ///////TODO: Add return if device is offline

    //Add the message to the wait list
    string replyTopic = "stat/"+device+"/"+info;
    pthread_mutex_t mutex;
    pthread_cond_t receivedSignal;

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&receivedSignal, NULL);

    //Get the lock and the reauest to the list
    pthread_mutex_lock(&mutex);
    cb->addTopicToWaitList(replyTopic,&mutex,&receivedSignal);


    try {
        // Publish message with listenner
        cout << "\nSending message...";
        mqtt::message_ptr pubmsg = mqtt::make_message("cmnd/"+device+"/"+info, "");
        mqtt::delivery_token_ptr token = client->publish(pubmsg, nullptr, *cb);
        
        //If no time out
        if(token->wait_for(TIMEOUT)){
            
            //Prepare to wait
            struct timespec ts;
            timespec_get(&ts, TIME_UTC);
            ts.tv_sec += 1;

            //Wait until response or timeout
            int rc = 0;
            while(!(cb->getMessageFlag(replyTopic)) && rc == 0){
                rc = pthread_cond_timedwait(&receivedSignal,&mutex,&ts);        
            }   

            //Get the response
            string _response = cb->getMessageResponse(replyTopic);
            cb->removeRequest(replyTopic);

            //If error during the request
            if(rc != 0){
                cout << "[DEVICE TIMEOUT]" << endl;
                response = "";
                return Device::REQUEST_RESPONSE::TIMED_OUT;
            }
            
            response = _response;
            return Device::REQUEST_RESPONSE::OK;

        }else{ //If broker timeout
            cout << "[BROKER TIMEOUT]" << endl;
            response = "";
            return Device::REQUEST_RESPONSE::TIMED_OUT;
        }
    }
    catch (const mqtt::exception& exc) {
        cerr << exc.what() << endl;
    }

    cout << "[INTERNAL ERROR]" << endl;
    response = "";

    return Device::REQUEST_RESPONSE::INTERNAL_ERROR;
}


/**
 * @brief This function is used by the message consumer thread
 * 
 */
void MQTTClient::messageConsumerFunc(bool *threadExitFlag,std::vector<Device::Device> *devices, callback *cb, pthread_mutex_t* messageConsumerMutex, pthread_cond_t* newMessageSignal){
    
    //Lock ressources
    pthread_mutex_lock(messageConsumerMutex);

    //Setup the delivery system
    cb->setupMessageDelivery(messageConsumerMutex,newMessageSignal);

    //While not exit
    while(!(*threadExitFlag)){

        //Prepare to wait
        struct timespec ts;
        timespec_get(&ts, TIME_UTC);
        ts.tv_sec += 1;

        while((cb->getMessageQueueSize() == 0) && !(*threadExitFlag))
            pthread_cond_timedwait(newMessageSignal,messageConsumerMutex,&ts);

        MQTTMessage msg;
        if(!cb->consumeMessage(&msg)){
            cout << "(" << msg.topic << ") -> " << msg.payload << endl;
        }
            
    }

    pthread_mutex_unlock(messageConsumerMutex);
}

/**
 * @brief Destroy the MQTTClient::MQTTClient object
 * 
 */
MQTTClient::~MQTTClient(){

    //Stop the consumer thread
    cout << "Stopping consumer thread (5s max)....";
    cout.flush();
    *threadExitFlag = true;

    //Take the lock
    pthread_mutex_lock(messageConsumerMutex);
    pthread_cond_signal(newMessageSignal);
    pthread_mutex_unlock(messageConsumerMutex);

    messageConsumerThread->join();

    cout << "[OK]" << endl;

    //Disconnect from the brocker
    try{
        //  Check that there are no pending tokens
        auto toks = client->get_pending_delivery_tokens();
        if (!toks.empty())
            cout << "Error: There are pending delivery tokens!" << endl;

        // Disconnect
        cout << "Disconnecting from MQTT Broker...";
        client->disconnect()->wait();
        cout << "[OK]" << endl;
    }
    catch (const mqtt::exception& exc) {
        cerr << exc.what() << endl;
    }

    //Free the memory
    delete messageConsumerThread;
    delete threadExitFlag;
    delete newMessageSignal;
    delete messageConsumerMutex;
    delete client;
    delete cb;

}