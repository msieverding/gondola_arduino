#ifndef _WIFI_CONNECTION_HPP_
#define _WIFI_CONNECTION_HPP_

#include <ESP8266WiFi.h>
#include "Gondola.hpp"
#include "WebServer.hpp"
#include "IConnection.hpp"

class WiFiConnection : public IConnection
{
public:
  WiFiConnection(WebServer *server, std::string ssid, std::string passphrase);
  virtual ~WiFiConnection();

private:

  WebServer          *m_Server;
  std::string         m_SSID;
  std::string         m_Passphrase;

};

#endif /* _WIFI_CONNECTION_HPP_ */
