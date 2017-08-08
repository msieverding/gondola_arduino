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

  virtual void loop();
private:

  WebServer          *m_WebServer;
  std::string         m_SSID;
  std::string         m_Passphrase;
  IPAddress           m_IPAddres;
};

#endif /* _WIFI_CONNECTION_HPP_ */
