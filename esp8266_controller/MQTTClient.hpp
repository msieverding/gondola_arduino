#include "IConnection.hpp"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

class MQTTClient : public IConnection
{
public:
  MQTTClient();
  virtual ~MQTTClient();
  virtual void loop();

private:
  void reconnect();

  WiFiClient    m_espClient;
  PubSubClient  m_mqttClient;
};
