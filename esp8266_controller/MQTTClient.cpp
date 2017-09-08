#include "MQTTClient.hpp"
#include "Arduino.h"
#include "Log.hpp"
#include <functional>


MQTTClient::MQTTClient()
 : m_WiFiClient()
 , m_MqTTClient(m_WiFiClient)
 , m_Anchor(Anchor::get())
{
  m_MqTTClient.setServer(Config::get()->getMQTT_CLIENT_SERVER().c_str(), 1883);
  m_MqTTClient.setCallback(std::bind(&MQTTClient::callback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
}

MQTTClient::~MQTTClient()
{
  WiFi.disconnect(false);
}

void MQTTClient::loop()
{
  if (!m_MqTTClient.connected())
  {
    reconnect();
  }
  m_MqTTClient.loop();

}

void MQTTClient::reconnect()
{
  static uint32_t nextTry = 0;

  if (millis() < nextTry)
  {
    return;
  }

  logDebug("Attempting MQTT connection...");
  // Attempt to connect

  if (m_MqTTClient.connect("ESP8266Client"))
  {
    m_MqTTClient.subscribe("gondola/move");
    logDebug("connected\n");
  }
  else
  {
    logDebug("failed, rc=%d. Try again in 5 seconds\n", m_MqTTClient.state());
    // Wait 5 seconds before retrying
    nextTry = millis() + 5000;
  }
}

void MQTTClient::callback(char* topic, byte* payload, unsigned int length)
{
  std::string topicS(topic);
  logDebug("***************Message arrived [%s] ", topicS.c_str());

  for (unsigned int i = 0; i < length; i++)
  {
    logDebug("%u", payload[i]);
  }
  logDebug("\n");

  if (topicS.compare("gondola/move") == 0 && length == 4 * sizeof(float))
  {
    callbackGondolaMove(payload);
  }
}

void MQTTClient::callbackGondolaMove(byte *payload)
{
  Coordinate newPos;
  float speed;

  float *pPayload = reinterpret_cast<float *>(payload);
  newPos.x = pPayload[0];
  newPos.y = pPayload[1];
  newPos.z = pPayload[2];
  speed = pPayload[3];

  m_Anchor->setTargetPosition(newPos, speed);
}
