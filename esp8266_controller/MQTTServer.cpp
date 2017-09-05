#include "MQTTServer.hpp"
#include <functional>

MQTTServer::MQTTServer()
 : m_MQTTServer("MqTT")
{
  // start
  m_MQTTServer.begin(&Serial);

  // add callback
  m_MQTTServer.setCallback(std::bind(&MQTTServer::mqttCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
  m_MQTTServer.subscribe("/test/temperature", false);
}

MQTTServer::~MQTTServer()
{
  WiFi.softAPdisconnect(false);
  WiFi.disconnect(false);
}

void MQTTServer::loop()
{
  m_MQTTServer.loop();
}


boolean MQTTServer::mqttCallback(char* queue, byte* payload, unsigned int length)
{
  String data = String((char *)payload);
  Serial.println("**********************"+String(queue)+"> ["+data+"]");
  return true;
}
