#ifndef _MQTT_CLIENT_HPP_
#define _MQTT_CLIENT_HPP_

#include "IMQTTService.hpp"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

class MQTTClient : public IMQTTService
{
public:
  MQTTClient();
  virtual ~MQTTClient();
  virtual void loop();

private:
  void reconnect();
  void callback(char* topic, byte* payload, unsigned int length);

  WiFiClient    m_espClient;
  PubSubClient  m_mqttClient;
};

#endif /* _MQTT_CLIENT_HPP_ */
