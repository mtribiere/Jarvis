#ifndef CALLBACK_HPP
#define CALLBACK_HPP

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

typedef struct WaitRequest{
	
	string topic = "";
	bool flag = 0;
	string response;

	pthread_mutex_t *mutex;
	pthread_cond_t *receiveSignal;

} WaitRequest;

typedef struct MQTTMessage{

	string topic;
	string payload;

} MQTTMessage;

/**
 * A callback class for use with the main MQTT client.
 */
class callback : public virtual mqtt::callback,
					public virtual mqtt::iaction_listener
{
public:
	callback(){
		pthread_mutex_init(&messageQueueMutex, NULL);
	}

	void connection_lost(const string& cause) override {
		cout << "\nConnection lost" << endl;
		if (!cause.empty())
			cout << "\tcause: " << cause << endl;
	}

	void delivery_complete(mqtt::delivery_token_ptr tok) override {
		cout << "\tDelivery complete for token: "
			<< (tok ? tok->get_message_id() : -1) << endl;
	}

	// (Re)connection success
	void connected(const std::string& cause) override {
		std::cout << "\nConnection to Broker successfull" << std::endl;
	}

    void message_arrived(mqtt::const_message_ptr msg) override {

		//Check if the topic is in the wait list
		for(int i = 0;i<waitList.size();i++){
			if(waitList.at(i).topic ==  msg->get_topic()){
				
				//Wait for the lock
				pthread_mutex_lock(waitList.at(i).mutex);

				//Fill up the response
				waitList.at(i).flag = 1;
				waitList.at(i).response = msg->to_string();
				pthread_cond_signal(waitList.at(i).receiveSignal);

				//Free the lock
				pthread_mutex_unlock(waitList.at(i).mutex);
			}
		}

		//Add this message to the queue
		pthread_mutex_lock(&messageQueueMutex);
		messageQueue.push({msg->get_topic(),msg->to_string()});	
		pthread_mutex_unlock(&messageQueueMutex);

		//Signal the new message
		if(messageDeliveryMutex != nullptr){
			pthread_mutex_lock(messageDeliveryMutex);
			pthread_cond_signal(newMessageSignal);
			pthread_mutex_unlock(messageDeliveryMutex);
		}
	}

	void on_failure(const mqtt::token& tok) override {
		cout << "Listener failure for token: "
			<< tok.get_message_id() << endl;
	}

	void on_success(const mqtt::token& tok) override {
		cout << "Listener success for token: "
			<< tok.get_message_id() << endl;
	}

	void addTopicToWaitList(string topic,pthread_mutex_t *mutex ,pthread_cond_t *receiveSignal){
		waitList.push_back((WaitRequest){topic,0,"",mutex ,receiveSignal});
	}

	bool getMessageFlag(string _topic){
		for(int i = 0;i<waitList.size();i++){
			if(waitList.at(i).topic == _topic){
				return waitList.at(i).flag;
			}
		}

		cout << "Warning: A not registered request as been pulled" << endl;
		return false;
	}

	string getMessageResponse(string _topic){
		
		for(int i = 0;i<waitList.size();i++){
			if(waitList.at(i).topic == _topic){
				return waitList.at(i).response;
			}
		}

		cout << "Warning: A not registered request as been pulled" << endl;
		return "";
	}

	void removeRequest(string _topic){

		for(int i = 0;i<waitList.size();i++){
			if(waitList.at(i).topic == _topic){
				waitList.erase(waitList.begin() + i);
			}
		}

	}

	int consumeMessage(MQTTMessage *msg){

		bool rc = 1;		
		pthread_mutex_lock(&messageQueueMutex);
		
		if(messageQueue.size() > 0){
			msg->topic = messageQueue.front().topic;
			msg->payload = messageQueue.front().payload;
			messageQueue.pop();
			rc = 0;
		}
		
		pthread_mutex_unlock(&messageQueueMutex);

		return rc;
	}

	void setupMessageDelivery(pthread_mutex_t *_messageDeliveryMutex, pthread_cond_t *_newMessageSignal){
		messageDeliveryMutex = _messageDeliveryMutex;
		newMessageSignal = _newMessageSignal;
	}

	int getMessageQueueSize(){
		
		pthread_mutex_lock(&messageQueueMutex);
		int size = messageQueue.size();
		pthread_mutex_unlock(&messageQueueMutex);

		return size; 
	}
	

	private:
		vector<WaitRequest> waitList;
		
		pthread_mutex_t messageQueueMutex;
		queue<MQTTMessage> messageQueue;

		pthread_mutex_t *messageDeliveryMutex = nullptr;
		pthread_cond_t *newMessageSignal = nullptr;
};


#endif