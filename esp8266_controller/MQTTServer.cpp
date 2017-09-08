#include "MQTTServer.hpp"
#include <functional>
#include <string>

MQTTServer::MQTTServer()
 : IGondola(Config::get()->getGO_POSITION())
 , m_mqttServer("MqTT")
{
  // start
  m_mqttServer.begin(&Serial);

  // add callback
  m_mqttServer.setCallback(std::bind(&MQTTServer::mqttCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
  // m_mqttServer.setloglevel(1);
}

MQTTServer::~MQTTServer()
{
}

void MQTTServer::loop()
{
  m_mqttServer.loop();
}


boolean MQTTServer::mqttCallback(char* queue, byte* payload, unsigned int length)
{
  // TODO
  String data = String((char *)payload);
  Serial.println("**********************"+String(queue)+"> ["+data+"]");
  return true;
}

void MQTTServer::setTargetPosition(Coordinate &targetPos, float &speed)
{
  m_Anchor.setTargetPosition(targetPos, speed);
  float messageBuf[4];
  messageBuf[0] = targetPos.x;
  messageBuf[1] = targetPos.y;
  messageBuf[2] = targetPos.z;
  messageBuf[3] = speed;

  uint8_t *pMessageBuf = reinterpret_cast<uint8_t *>(messageBuf);

  m_mqttServer.publish("gondola/move", pMessageBuf, 4 * sizeof(float));
}
