#include "MQTTServer.hpp"
#include <functional>
#include <string>
#include "Log.hpp"

MQTTServer::MQTTServer()
 : m_mqttServer("MqTT")
 , m_Gondola(Gondola::get())
{
  // start
  m_mqttServer.begin(&Serial);

  // add callback
  m_mqttServer.setCallback(std::bind(&MQTTServer::mqttCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
  m_mqttServer.setloglevel(1);

  m_Gondola->registerMoveCommand(std::bind(&MQTTServer::gondolaMove, this, std::placeholders::_1, std::placeholders::_2));
}

MQTTServer::~MQTTServer()
{
  m_Gondola->registerMoveCommand(NULL);
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

void MQTTServer::gondolaMove(Coordinate &targetPos, float &speed)
{
  logDebug("gondolaMove\n");
  
  float messageBuf[4];
  messageBuf[0] = targetPos.x;
  messageBuf[1] = targetPos.y;
  messageBuf[2] = targetPos.z;
  messageBuf[3] = speed;

  uint8_t *pMessageBuf = reinterpret_cast<uint8_t *>(messageBuf);

  m_mqttServer.publish("gondola/move", pMessageBuf, 4 * sizeof(float));
}
