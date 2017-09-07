#ifndef _MQTT_SERVER_HPP_
#define _MQTT_SERVER_HPP_

#include "IMQTTService.hpp"
#include "ESP8266MqTT.h"
#include "ESP.h"
#include "IGondola.hpp"

#include <PubSubClient.h> // https://github.com/knolleary/pubsubclient/releases/tag/v2.3

// TODO DOCU

class MQTTServer : public IMQTTService, public IGondola
{
public:

  MQTTServer();
  virtual ~MQTTServer();

  // implement IMQTTService
  virtual void loop();

  // implement IGondola
  virtual void setTargetPosition(Coordinate &targetPos, float &speed);

private:

  boolean mqttCallback(char* queue, byte* payload, unsigned int length);

  ME          m_mqttServer;
};

#endif /* _MQTT_SERVER_HPP_ */
