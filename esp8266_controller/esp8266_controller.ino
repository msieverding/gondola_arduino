// Configuration
#include "Config.hpp"
// Own classes and libraries
#include "Gondola.hpp"
#include "HardwareAnchor.hpp"
#include "WiFiConnection.hpp"
#include "SerialConnection.hpp"
#include "APConnection.hpp"
#include "WebServer.hpp"
#include "WebServerMaster.hpp"
#include "WebServerSlave.hpp"
#include "CommandInterpreter.hpp"
#include "ConnectionMgr.hpp"
#include "Log.hpp"
#include <PubSubClient.h>

IConnection *serial;
ConnectionMgr *conMgr;
HardwareAnchor *anchor;

// Update these with values suitable for your network.
const char* mqtt_server = "broker.mqtt-dashboard.com";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void reconnect()
{
  // Loop until we're reconnected
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str()))
    {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("/test/temperature", "hello world");
      // ... and resubscribe
      client.subscribe("inTopic");
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup()
{
  // Wait one second for hardware and serial console
  delay(1000);
  serial = SerialConnection::create(115200);


  Config* config = Config::get();
  // config->readFromEEPROM();

  anchor = HardwareAnchor::create({0, 5, 4}, {0.0f, 0.0f, 0.0f}, 0.0f);

  conMgr = ConnectionMgr::get();

  client.setServer(IPAddress(192, 168, 5, 101), 1883);
  client.setCallback(callback);

  wdt_enable(1000);
  // ESP.wdtEnable(1000);
}

void loop()
{
  wdt_reset();
  // ESP.wdtFeed();
  if (serial)
    serial->loop();
  if (conMgr)
    conMgr->loop();
  if (anchor)
    anchor->move();

  // if (!client.connected())
  // {
  //   reconnect();
  // }
  // client.loop();
  //
  // long now = millis();
  // if (now - lastMsg > 2000)
  // {
  //   lastMsg = now;
  //   ++value;
  //   snprintf (msg, 75, "hello world #%ld", value);
  //   Serial.print("Publish message: ");
  //   Serial.println(msg);
  //   client.publish("/test/temperature", msg);
  // }
}


// /*
//  * Author: Francesco Barone
//  * MqTT Server simple sketch
//  * 18.07.2017 - release 1
//  */
//
// #include "lib/ME-MqTT-Server-for-ESP8266/src/FB_IoT-MqttServer/src/ESP8266MqTT.h"
// #include "ESP.h"
//
// #include <ESP8266WiFi.h>
// #include <ESP8266WiFiMulti.h>
// #include <PubSubClient.h> // https://github.com/knolleary/pubsubclient/releases/tag/v2.3
// #include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson/releases/tag/v5.0.7
//
// //-------- Customise these values -----------
// const char* ssid = "GondolaWiFi";
// const char* password = "TU_GRAZ_ITI";
// //-------- Customise the above values --------
//
// WiFiClient wifiClient;
//
// //--------------------------------------------
//
// ME me("MqTT");
//
//
// int STATE=0;
// const int _START=10;
// const int _WIFI_IP=20;
// const int _WIFI_CONNECT=30;
// const int _LOOP_APP=40;
// boolean WIFI_connected;
//
//
// boolean mqtt_callback (char* queue, byte* payload, unsigned int length)
// {
//   String data = String((char *)payload);
//   Serial.println("**********************"+String(queue)+"> ["+data+"]");
// }
//
// void setup()
// {
//
//   Serial.begin(115200);
//
//   //start
//   me.begin(&Serial);
//
//   //add callback
//   me.setCallback(mqtt_callback);
//   me.subscribe("/test/temperature",false);
//
//
//   //0=PROD
//   //me.setloglevel(PROD);
//
//   // Connect to Network
//   WiFi.begin(ssid, password);
//   // Set stationary mode
//   WiFi.mode(WIFI_STA);
//
//   Serial.print("Connect to WiFi: ");
//   Serial.print(ssid);
//
//   while(WiFi.status() != WL_CONNECTED)
//   {
//     delay(500);
//     Serial.print(".");
//   }
//   Serial.print(" Ready\n");
//
//   IPAddress ip = WiFi.localIP();
//   // Print the IP address
//   Serial.print("Local IP address: ");
//   Serial.print(ip.toString().c_str());
//   Serial.print("\n");
//
//   STATE=START;
// }
//
// void loop()
// {
//   //process mqtt
//   me.loop();
// }
