#include "MQTTClient.hpp"
#include "Arduino.h"
#include "Log.hpp"
#include <functional>

// TODO add to Config class
#define mqtt_server "192.168.5.1"


MQTTClient::MQTTClient()
 : m_espClient()
 , m_mqttClient(m_espClient)
 , m_Anchor({0, 5, 4}, {0.0f, 0.0f, 0.0f}, Config::get()->getGO_POSITION())
{
  m_mqttClient.setServer(mqtt_server, 1883);
  m_mqttClient.setCallback(std::bind(&MQTTClient::callback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
}

MQTTClient::~MQTTClient()
{
  WiFi.disconnect(false);
}

void MQTTClient::loop()
{
  if (!m_mqttClient.connected())
  {
    reconnect();
  }
  m_mqttClient.loop();

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

  if (m_mqttClient.connect("ESP8266Client"))
  {
    m_mqttClient.subscribe("test/temperature");
    logDebug("connected\n");
  }
  else
  {
    logDebug("failed, rc=%d. Try again in 5 seconds\n", m_mqttClient.state());
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
    logDebug("%hhu", payload[i]);
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

  m_Anchor.setTargetPosition(newPos, speed);
}
