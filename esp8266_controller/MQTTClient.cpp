#include "MQTTClient.hpp"
#include "Arduino.h"

#define wifi_ssid "MqTT"
#define wifi_password "passw0rd"

#define mqtt_server "192.168.4.1"
#define mqtt_user "admin"
#define mqtt_password "passw0rd"

MQTTClient::MQTTClient()
 : m_espClient()
 , m_mqttClient(m_espClient)
{
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(wifi_ssid);

  WiFi.begin(wifi_ssid, wifi_password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  m_mqttClient.setServer(mqtt_server, 1883);
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

  long now = millis();
  if (now - lastMsg > 1000)
  {
    lastMsg = now;

    float temp = 0.0f;

    Serial.print("New temperature:");
    Serial.println(String(temp).c_str());
    m_mqttClient.publish("/test/temperature", String(temp).c_str(), true);
  }
}

void MQTTClient::reconnect()
{
  // TODO make it static
  bool userAdded = false;
  static uint32_t nextTry = 0;

  if (millis() < nextTry)
  {
    return;
  }

  // Loop until we're reconnected
  while (!m_mqttClient.connected())
  {
    Serial.print("Attempting MQTT connection...");
    if (!userAdded)
    {
      if (m_mqttClient.connect("ESP8266Admin", "admin", "passw0rd"))
      {
        Serial.println("connected -> Add user");
        m_mqttClient.publish("/SYS/MqTT/console", "ADDUSER pippo passw0rd", true);
        m_mqttClient.disconnect();
        userAdded = true;
      }
      else
      {
        Serial.println("Couldn't connect as admin");
      }
    }
    // Attempt to connect
    // If you do not want to use a username and password, change next line to
    // if (m_mqttClient.connect("ESP8266Client")) {
    if (m_mqttClient.connect("ESP8266Client", "pippo", "passw0rd"))
    {
      Serial.println("connected");
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(m_mqttClient.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      nextTry = millis() + 5000;
    }
  }
}
