#include "MQTTClient.hpp"
#include "Arduino.h"
#include "Log.hpp"
#include <functional>

// TODO add to Config class
#define mqtt_server "192.168.5.1"


MQTTClient::MQTTClient()
 : m_espClient()
 , m_mqttClient(m_espClient)
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
  static long lastMsg = 0;

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

  Log::logDebug("Attempting MQTT connection...");
  // Attempt to connect

  if (m_mqttClient.connect("ESP8266Client"))
  {
    m_mqttClient.subscribe("test/temperature");
    Log::logDebug("connected\n");
  }
  else
  {
    Log::logDebug("failed, rc=");
    Log::logDebug(m_mqttClient.state());
    Log::logDebug(" try again in 5 seconds\n");
    // Wait 5 seconds before retrying
    nextTry = millis() + 5000;
  }
}

void MQTTClient::callback(char* topic, byte* payload, unsigned int length)
{
  Log::logDebug("***************Message arrived [");
  Log::logDebug(topic);
  Log::logDebug("] ");
  for (unsigned int i = 0; i < length; i++)
  {
    Log::logDebug((char)payload[i]);
  }
  Log::logDebug("\n");
}
