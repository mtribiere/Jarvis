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

/**
 * A callback class for use with the main MQTT client.
 */
class callback : public virtual mqtt::callback,
					public virtual mqtt::iaction_listener
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

	// (Re)connection success
	void connected(const std::string& cause) override {
		std::cout << "\nConnection to Broker successfull" << std::endl;
	}

    void message_arrived(mqtt::const_message_ptr msg) override {
		std::cout << "Message arrived" << std::endl;
		std::cout << "\ttopic: '" << msg->get_topic() << "'" << std::endl;
		std::cout << "\tpayload: '" << msg->to_string() << "'\n" << std::endl;

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

		cout << "Warning: A none registered request as been pulled" << endl;
		return false;
	}

	string getMessageResponse(string _topic){
		
		for(int i = 0;i<waitList.size();i++){
			if(waitList.at(i).topic == _topic){
				return waitList.at(i).response;
			}
		}

		cout << "Warning: A none registered request as been pulled" << endl;
		return "";
	}

	void removeRequest(string _topic){

		for(int i = 0;i<waitList.size();i++){
			if(waitList.at(i).topic == _topic){
				waitList.erase(waitList.begin() + i);
			}
		}

	}

	vector<WaitRequest> waitList;
    
};


#endif