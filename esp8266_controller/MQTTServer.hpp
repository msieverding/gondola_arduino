#ifndef _MQTT_SERVER_HPP_
#define _MQTT_SERVER_HPP_

#include "IMQTTService.hpp"
#include "ESP8266MqTT.h"
#include "ESP.h"

#include <PubSubClient.h> // https://github.com/knolleary/pubsubclient/releases/tag/v2.3

// TODO DOCU

class MQTTServer : public IMQTTService
{
public:

  MQTTServer();
  virtual ~MQTTServer();

  virtual void loop();

private:

  boolean mqttCallback(char* queue, byte* payload, unsigned int length);

  ME      m_mqttServer;
};

#endif /* _MQTT_SERVER_HPP_ */
