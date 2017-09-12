// #ifndef _MQTT_CLIENT_ASYNC_HPP_
// #define _MQTT_CLIENT_ASYNC_HPP_
//
// #include <AsyncMqttClient.h>
// #include <ESP8266WiFi.h>
// #include "Anchor.hpp"
// #include "IMQTTService.hpp"
//
// class MQTTClientAsync : public IMQTTService
// {
// public:
//   MQTTClientAsync();
//   virtual ~MQTTClientAsync();
//   virtual void loop();
//
// private:
//   void onWifiConnect(const WiFiEventStationModeGotIP& event);
//   void onWifiDisconnect(const WiFiEventStationModeDisconnected& event);
//
//   void onMqttDisconnect(AsyncMqttClientDisconnectReason reason);
//   void onMqttConnect(bool sessionPresent);
//   void onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total);
//
//   void callbackGondolaMove(byte *payload);
//
//   Anchor             *m_Anchor;
//   WiFiEventHandler    m_WiFiConnectHandler;
//   AsyncMqttClient     m_MqTTClient;
//   uint32_t            m_ReconnectTime;
// };
//
// #endif /* _MQTT_CLIENT_ASYNC_HPP_ */
