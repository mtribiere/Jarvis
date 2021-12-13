#include <ArduinoJson.h>
#include <EspMQTTClient.h>

#define DEBUG_BUILD 0
#define RELAY_PIN 5
#define STATUS_LED_PIN 4
#define NODE_ID 0


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
  #if DEBUG_BUILD
   Serial.begin(115200);
  #endif

  //Enable LED as output
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(STATUS_LED_PIN, OUTPUT);

  digitalWrite(STATUS_LED_PIN,LOW);

  // Enable build option
  #if DEBUG_BUILD
    client.enableDebuggingMessages(); // Enable debugging messages sent to serial output
  #else
    client.enableOTA(); // Enable OTA (Over The Air) updates. Password defaults to MQTTPassword. Port is the default OTA port. Can be overridden with enableOTA("password", port).
  #endif

  //Construct the last will message
  DynamicJsonDocument doc(1024);
  doc["intent"] = "disconnectNode";
  doc["targetNode"] = NODE_ID;
  
  String lastWillMessage;
  serializeJson(doc,lastWillMessage);
  client.enableLastWillMessage("/home/device/status", lastWillMessage.c_str());  // You can activate the retain flag by setting the third parameter to true

  digitalWrite(STATUS_LED_PIN,HIGH);

}

void onConnectionEstablished()
{

  client.subscribe("/home/light", [](const String & payload) {

    DynamicJsonDocument doc(1024);
    deserializeJson(doc, payload);

    const char* intent = doc["intent"];
    
    if(strcmp(intent,"setProperty") == 0){
      
      const char* property = doc["property"];
      if(strcmp(property,"onStatus") == 0){
          
          int value = doc["value"];
          digitalWrite(RELAY_PIN,value);
        
      }
    }

    #if DEBUG_BUILD
      Serial.println(payload);
    #endif

  });
}

void loop()
{
  client.loop();
}
