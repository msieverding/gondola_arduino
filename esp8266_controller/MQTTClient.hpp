#ifndef _MQTT_CLIENT_HPP_
#define _MQTT_CLIENT_HPP_

#include <ESP8266WiFi.h>
#include "IMQTTService.hpp"
#include <PubSubClient.h>
#include "Anchor.hpp"

class MQTTClient : public IMQTTService
{
public:
  MQTTClient();
  virtual ~MQTTClient();
  virtual void loop();

private:
  void reconnect();
  void callback(char* topic, byte* payload, unsigned int length);
  void callbackGondolaMove(std::string &msg);
  std::string responseToString(int response);

  WiFiClient    m_WiFiClient;
  PubSubClient  m_MqTTClient;
  Anchor       *m_Anchor;
};

#endif /* _MQTT_CLIENT_HPP_ */
