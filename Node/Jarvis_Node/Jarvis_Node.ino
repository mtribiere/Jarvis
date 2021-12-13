#include "EspMQTTClient.h"
#include <ArduinoJson.h>


#define LED_PIN 5

EspMQTTClient client(
  "AndroidAP_4153",
  "a2a2wPRb34gJS468",
  "192.168.43.166",  // MQTT Broker server ip
  "",   // Can be omitted if not needed
  "",   // Can be omitted if not needed
  "LightClient1",     // Client name that uniquely identify your device
  1883              // The MQTT port, default to 1883. this line can be omitted
);

void setup()
{
  Serial.begin(115200);

  // Optional functionalities of EspMQTTClient
  client.enableDebuggingMessages(); // Enable debugging messages sent to serial output
  client.enableHTTPWebUpdater("admin","admin"); // Enable the web updater. User and password default to values of MQTTUsername and MQTTPassword. These can be overridded with enableHTTPWebUpdater("user", "password").
  client.enableOTA(); // Enable OTA (Over The Air) updates. Password defaults to MQTTPassword. Port is the default OTA port. Can be overridden with enableOTA("password", port).
  client.enableLastWillMessage("TestClient/lastwill", "I am going offline");  // You can activate the retain flag by setting the third parameter to true

  //Enable LED as output
  pinMode(LED_PIN, OUTPUT);
}

void onConnectionEstablished()
{

  client.subscribe("/home/light", [](const String & payload) {

    DynamicJsonDocument doc(1024);
    deserializeJson(doc, payload);

    const char* action = doc["action"];
    
    if(strcmp(action,"TurnOn") == 0){
      digitalWrite(LED_PIN,HIGH);
    }
    if(strcmp(action,"TurnOff") == 0){
      digitalWrite(LED_PIN,LOW);
    }
    
    Serial.println(payload);
  });
}

void loop()
{
  client.loop();
}
