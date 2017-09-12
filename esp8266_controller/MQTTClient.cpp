// #include "MQTTClient.hpp"
// #include "Arduino.h"
// #include "Log.hpp"
// #include <functional>
// #include "ConnectionMgr.hpp"
//
// #define STATUS_TIME   5000      // 5 seconds between to status messages
//
// MQTTClient::MQTTClient()
//  : m_WiFiClient()
//  , m_MqTTClient(m_WiFiClient)
//  , m_Anchor(Anchor::get())
//  , m_NextStatus(millis())
// {
//   m_MqTTClient.setServer(Config::get()->getMQTT_CLIENT_SERVER().c_str(), 1883);
//   m_MqTTClient.setCallback(std::bind(&MQTTClient::callback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
// }
//
// MQTTClient::~MQTTClient()
// {
//   WiFi.disconnect(false);
// }
//
// void MQTTClient::loop()
// {
//   if (!m_MqTTClient.connected())
//   {
//     reconnect();
//   }
//   m_MqTTClient.loop();
//
//   if (millis() >= m_NextStatus)
//   {
//     m_NextStatus += STATUS_TIME;
//     sendAnchorStatus();
//   }
// }
//
// void MQTTClient::reconnect()
// {
//   static uint32_t nextTry = 0;
//
//   if (millis() < nextTry)
//   {
//     return;
//   }
//
//   logDebug("Attempting MQTT connection...");
//   // Attempt to connect
//
//   if (m_MqTTClient.connect("ESP8266Client"))
//   {
//     m_MqTTClient.subscribe("gondola/move");
//     logDebug("connected\n");
//   }
//   else
//   {
//     int response = m_MqTTClient.state();
//     logDebug("failed, '%s' (=%d). Try again in 5 seconds\n", responseToString(response).c_str(), response);
//     // Wait 5 seconds before retrying
//     nextTry = millis() + 5000;
//   }
// }
//
// void MQTTClient::callback(char* topic, byte* payload, unsigned int length)
// {
//   std::string topicS(topic);
//   std::string message;
//
//   for (uint16_t i = 0; i < length; i++)
//   {
//     message.push_back(static_cast<char>(payload[i]));
//   }
//
//   logDebug("***************Message arrived! Topic='%s', length=%u, content:'%s'\n", topicS.c_str(), length, message.c_str());
//
//   // TODO length is only valid for strings... due to implementation of server...
//   if (topicS.compare("gondola/move") == 0)
//   {
//     callbackGondolaMove(message);
//   }
// }
//
// void MQTTClient::callbackGondolaMove(std::string &msg)
// {
//   Coordinate newPos;
//   float speed;
//
//   uint16_t posx = msg.find("x");
//   uint16_t posy = msg.find("y");
//   uint16_t posz = msg.find("z");
//   uint16_t poss = msg.find("s");
//
//   if (posx == std::string::npos || posy == std::string::npos || posz == std::string::npos || poss == std::string::npos)
//     return;
//
//   std::string s = msg.substr(posx + 2, posy);
//   newPos.x = stringtoFloat(s);
//   logDebug("str = '%s'\n", s.c_str());
//   s = msg.substr(posy + 2, posz);
//   newPos.y = stringtoFloat(s);
//   logDebug("str = '%s'\n", s.c_str());
//   s = msg.substr(posz + 2, poss);
//   newPos.z = stringtoFloat(s);
//   logDebug("str = '%s'\n", s.c_str());
//   s = msg.substr(poss + 2);
//   speed = stringtoFloat(s);
//   logDebug("str = '%s'\n", s.c_str());
//
//   logDebug("Set new Target position: %s with speed %s", newPos.toString().c_str(), floatToString(speed).c_str());
//   m_Anchor->setTargetPosition(newPos, speed);
// }
//
// void MQTTClient::sendAnchorStatus(void)
// {
//   std::string message;
//
//   message.append("ip:");
//
//   // TODO what about ip of dual connection?
//   if (ConnectionMgr::get()->getConnectionType() == CON_ACCESS_POINT)
//     message.append(WiFi.softAPIP().toString().c_str());
//   else
//     message.append(WiFi.localIP().toString().c_str());
//
//   message.append("spooledDistance:");
//   message.append(floatToString(m_Anchor->getCurrentSpooledDistance()).c_str());
//   message.append("stepsToDo:");
//   message.append(floatToString(m_Anchor->getStepsTodo()).c_str());
//
//   m_MqTTClient.publish("anchor/status", message.c_str());
// }
//
// std::string MQTTClient::responseToString(int response)
// {
//   switch(response)
//   {
//     case MQTT_CONNECTION_TIMEOUT:
//       return std::string("Connection timeout");
//       break;
//     case MQTT_CONNECTION_LOST:
//       return std::string("Connection lost");
//       break;
//     case MQTT_CONNECT_FAILED:
//       return std::string("Connect failed");
//       break;
//     case MQTT_DISCONNECTED:
//       return std::string("MqTT disconnected");
//       break;
//     case MQTT_CONNECTED:
//       return std::string("MqTT connected");
//       break;
//     case MQTT_CONNECT_BAD_PROTOCOL:
//       return std::string("Connected bad protocoll");
//       break;
//     case MQTT_CONNECT_BAD_CLIENT_ID:
//       return std::string("Connected bad client id");
//       break;
//     case MQTT_CONNECT_UNAVAILABLE:
//       return std::string("Connect unavailable");
//       break;
//     case MQTT_CONNECT_BAD_CREDENTIALS:
//       return std::string("Connect bad credentials");
//       break;
//     case MQTT_CONNECT_UNAUTHORIZED:
//       return std::string("Connect unauthorized");
//       break;
//     default:
//       return std::string("");
//   }
// }
