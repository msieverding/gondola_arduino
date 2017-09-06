#include "MQTTServer.hpp"
#include <functional>

MQTTServer::MQTTServer()
 : m_mqttServer("MqTT")
{
  // start
  m_mqttServer.begin(&Serial);

  // add callback
  m_mqttServer.setCallback(std::bind(&MQTTServer::mqttCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
  m_mqttServer.subscribe("/test/temperature", false);
}

MQTTServer::~MQTTServer()
{
  WiFi.softAPdisconnect(false);
  WiFi.disconnect(false);
}

void MQTTServer::loop()
{
  static uint32_t nextPublish = 0;

  if (millis() > nextPublish)
  {
    m_mqttServer.publish("test/temperature", "Server publish");
    nextPublish = millis() + 5000;
  }

  m_mqttServer.loop();
}


boolean MQTTServer::mqttCallback(char* queue, byte* payload, unsigned int length)
{
  String data = String((char *)payload);
  Serial.println("**********************"+String(queue)+"> ["+data+"]");
  return true;
}
