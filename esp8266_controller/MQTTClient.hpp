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
  void callback(char* topic, byte* payload, unsigned int length);
  
  WiFiClient    m_espClient;
  PubSubClient  m_mqttClient;
};
