#include "MQTTServer.hpp"
#include <functional>
#include <string>
#include "Log.hpp"
#include "CommandInterpreter.hpp"

MQTTServer::MQTTServer()
 : m_mqttServer("MqTT")
 , m_Gondola(Gondola::get())
{
  // start
  m_mqttServer.begin(&Serial);

  // add callback
  m_mqttServer.setCallback(std::bind(&MQTTServer::mqttCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
  m_mqttServer.setloglevel(Config::get()->getDEBUG_MQTT());
  m_mqttServer.subscribe("anchor/status", true);

  m_Gondola->registerMoveFunction(std::bind(&MQTTServer::gondolaMove, this, std::placeholders::_1, std::placeholders::_2));

  CommandInterpreter::get()->addCommand("mqttloglevel", std::bind(&MQTTServer::mqttloglevelCommand, this, std::placeholders::_1));
}

MQTTServer::~MQTTServer()
{
  CommandInterpreter::get()->deleteCommand("mqttloglevel");
  delete(m_Gondola);
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
  logDebug("MQTTServer::gondolaMove\n");

  std::string msg;
  msg.append("x:");
  msg.append(floatToString(targetPos.x).c_str());
  msg.append("y:");
  msg.append(floatToString(targetPos.y).c_str());
  msg.append("z:");
  msg.append(floatToString(targetPos.z).c_str());
  msg.append("s:");
  msg.append(floatToString(speed).c_str());

  logDebug("Prepared string:'%s'\n", msg.c_str());

  m_mqttServer.publish("gondola/move", msg.c_str());
}

bool MQTTServer::mqttloglevelCommand(std::string &s)
{
  CommandInterpreter *CI = CommandInterpreter::get();
  std::string arg0;
  CI->getArgument(s, arg0, 0);

  uint8_t level = atoi(arg0.c_str());

  if (level > 3)
  {
    logWarning("Only log levels 0 - 3 are allowed!\n");
    return false;
  }

  Config::get()->setDEBUG_MQTT(level);
  m_mqttServer.setloglevel(level);
  return true;
}
