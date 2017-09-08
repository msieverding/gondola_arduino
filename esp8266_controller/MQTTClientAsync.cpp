#include "MQTTClientAsync.hpp"
#include <functional>
#include "Log.hpp"

MQTTClientAsync::MQTTClientAsync()
 : m_Anchor(Anchor::get())
 , m_WiFiConnectHandler()
 , m_MqTTClient()
 , m_ReconnectTime(0)
{
  m_WiFiConnectHandler = WiFi.onStationModeGotIP(std::bind(&MQTTClientAsync::onWifiConnect, this, std::placeholders::_1));

  m_MqTTClient.onConnect(std::bind(&MQTTClientAsync::onMqttConnect, this, std::placeholders::_1));
  m_MqTTClient.onDisconnect(std::bind(&MQTTClientAsync::onMqttDisconnect, this, std::placeholders::_1));
  m_MqTTClient.onMessage(std::bind(&MQTTClientAsync::onMqttMessage, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5, std::placeholders::_6));
  // m_MqTTClient.onSubscribe(onMqttSubscribe);
  // m_MqTTClient.onUnsubscribe(onMqttUnsubscribe);
  // m_MqTTClient.onPublish(onMqttPublish);

  m_MqTTClient.setServer(Config::get()->getMQTT_CLIENT_SERVER().c_str(), 1883);

  if (WiFi.isConnected())
    m_MqTTClient.connect();
}

MQTTClientAsync::~MQTTClientAsync()
{
  m_MqTTClient.disconnect();
}

void MQTTClientAsync::loop()
{
  if (millis() > m_ReconnectTime && !m_MqTTClient.connected())
  {
    m_ReconnectTime = 0xFFFFFFFF;     // -1 -> trigger reconnect once is enough for async communication
    m_MqTTClient.connect();
  }
}

void MQTTClientAsync::onWifiConnect(const WiFiEventStationModeGotIP& event)
{
  m_MqTTClient.connect();
}

void MQTTClientAsync::onMqttDisconnect(AsyncMqttClientDisconnectReason reason)
{
  logDebug("Disconnected from MQTT with reason %d.\n", static_cast<int8_t>(reason));

  if (WiFi.isConnected())
  {
    m_ReconnectTime = millis() + 5000; // Reconnect in 5s
    logDebug("Lost connection. Try again in 5s.\n");
  }
  else
  {
    logDebug("Lost connection. Try again after WiFi connect.\n");
  }
}


void MQTTClientAsync::onMqttConnect(bool sessionPresent)
{
  m_MqTTClient.subscribe("gondola/move", 0);
}

void MQTTClientAsync::onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total)
{
  Serial.println("Publish received.");
  Serial.print("  topic: ");
  Serial.println(topic);
  Serial.print("  qos: ");
  Serial.println(properties.qos);
  Serial.print("  dup: ");
  Serial.println(properties.dup);
  Serial.print("  retain: ");
  Serial.println(properties.retain);
  Serial.print("  len: ");
  Serial.println(len);
  Serial.print("  index: ");
  Serial.println(index);
  Serial.print("  total: ");
  Serial.println(total);
}

void MQTTClientAsync::callbackGondolaMove(byte *payload)
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
