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

#include "lib/ME-MqTT-Server-for-ESP8266/src/FB_IoT-MqttServer/src/ESP8266MqTT.h"
#include "ESP.h"

#include <ESP8266WiFi.h>
#include <PubSubClient.h> // https://github.com/knolleary/pubsubclient/releases/tag/v2.3
#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson/releases/tag/v5.0.7


IConnection *serial;
ConnectionMgr *conMgr;
HardwareAnchor *anchor;

WiFiClient wifiClient;

//--------------------------------------------

ME me("MqTT");


int STATE=0;
const int _START=10;
const int _WIFI_IP=20;
const int _WIFI_CONNECT=30;
const int _LOOP_APP=40;
boolean WIFI_connected;


boolean mqtt_callback(char* queue, byte* payload, unsigned int length)
{
  String data = String((char *)payload);
  Serial.println("**********************"+String(queue)+"> ["+data+"]");
}

void setup()
{
  // Wait one second for hardware and serial console
  delay(1000);
  serial = SerialConnection::create(115200);


  Config* config = Config::get();
  // config->readFromEEPROM();

  // anchor = HardwareAnchor::create({0, 5, 4}, {0.0f, 0.0f, 0.0f}, 0.0f);

  // conMgr = ConnectionMgr::get();

  //start
  me.begin(&Serial);

  //add callback
  me.setCallback(mqtt_callback);
  me.subscribe("/test/temperature",false);

  wdt_enable(1000);
  // ESP.wdtEnable(1000);
}

void loop()
{
  wdt_reset();
  // ESP.wdtFeed();
  // if (serial)
  //   serial->loop();
  // if (conMgr)
  //   conMgr->loop();
  // if (anchor)
  //   anchor->move();

  //process mqtt
  me.loop();
}
