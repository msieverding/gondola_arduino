#include "IConnection.hpp"
#include "lib/ME-MqTT-Server-for-ESP8266/src/FB_IoT-MqttServer/src/ESP8266MqTT.h"
#include "ESP.h"

#include <PubSubClient.h> // https://github.com/knolleary/pubsubclient/releases/tag/v2.3
#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson/releases/tag/v5.0.7

class MQTTServer : public IConnection
{
public:

  MQTTServer();
  virtual ~MQTTServer();

  virtual void loop();

private:

  boolean mqttCallback(char* queue, byte* payload, unsigned int length);

  ME      m_MQTTServer;
};
