// #ifndef _MQTT_SERVER_HPP_
// #define _MQTT_SERVER_HPP_
//
// #include "IMQTTService.hpp"
// #include "ESP8266MqTT.h"
// #include "ESP.h"
// #include "Gondola.hpp"
//
// #include <PubSubClient.h> // https://github.com/knolleary/pubsubclient/releases/tag/v2.3
//
// // TODO DOCU
//
// class MQTTServer : public IMQTTService
// {
// public:
//
//   MQTTServer();
//   virtual ~MQTTServer();
//
//   // implement IMQTTService
//   virtual void loop();
//
//   void gondolaMove(Coordinate &targetPos, float &speed);
//
// private:
//
//   boolean mqttCallback(char* queue, byte* payload, unsigned int length);
//
//   bool mqttloglevelCommand(std::string &s);
//
//   ME          m_mqttServer;
//   Gondola    *m_Gondola;
// };
//
// #endif /* _MQTT_SERVER_HPP_ */
