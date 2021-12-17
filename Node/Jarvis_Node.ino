#include <ArduinoJson.h>
#include <EspMQTTClient.h>

#define DEBUG_BUILD 0
#define RELAY_PIN 12
#define STATUS_LED_PIN 13

#define NODE_ID 0
int devicesID[] = {0,4,5};


EspMQTTClient *client = nullptr;
DynamicJsonDocument deviceConfig(1024);
String tmp;

void setup()
{
  #if DEBUG_BUILD
   Serial.begin(115200);
  #endif

  //Enable LED as output
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(STATUS_LED_PIN, OUTPUT);

  //Reset output status
  digitalWrite(STATUS_LED_PIN,LOW);
  digitalWrite(RELAY_PIN,LOW);

  //Connect client
  client = new EspMQTTClient(
  "AndroidAP_4153",
  "a2a2wPRb34gJS468",
  "192.168.43.166",  // MQTT Broker server ip
  "",  // Broker username
  "",  // Broker password
  "Switch1",     // Client name (UNIQUE)
  1883              // Broker port
  );

  // Enable debug message
  #if DEBUG_BUILD
    client->enableDebuggingMessages(); // Enable debugging messages sent to serial output
  #endif
  
  client->enableOTA(); // Enable OTA (Over The Air) updates. Password defaults to MQTTPassword. Port is the default OTA port. Can be overridden with enableOTA("password", port).
  
  //Construct device configuration
  deviceConfig["intent"] = "connectNode";
  deviceConfig["targetNode"] = NODE_ID;

  for(int i = 0;i<sizeof(devicesID)/sizeof(int);i++)
    deviceConfig["targetDevices"][i] = devicesID[i];

  //When going down after a update, turn off status led
  ArduinoOTA.onEnd([]() {
    digitalWrite(STATUS_LED_PIN,LOW);
  });

}

void onConnectionEstablished()
{
  //Turn on status LED
  digitalWrite(STATUS_LED_PIN,HIGH);

  //Send the device configuration to the broker
  deviceConfig["ip"] = WiFi.localIP();
  serializeJson(deviceConfig,tmp);
  client->publish("/home/nodes/connect",tmp.c_str());

  //Enable last will message
  deviceConfig["intent"] = "disconnectNode";
  serializeJson(deviceConfig,tmp);
  client->enableLastWillMessage("/home/nodes/disconnect", tmp.c_str()); 


  //Subscribe to the order topic
  client->subscribe("/home/light", [](const String & payload) {

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
  client->loop();
}
