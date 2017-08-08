#ifndef _WIFI_CONNECTION_HPP_
#define _WIFI_CONNECTION_HPP_

#include <ESP8266WiFi.h>
#include "Gondola.hpp"
#include "WebServer.hpp"
#include "IConnection.hpp"
#include <ESP8266mDNS.h>

class WiFiConnection : public IConnection
{
public:
  WiFiConnection(WebServer *server, std::string ssid, std::string passphrase, std::string hostname);
  virtual ~WiFiConnection();

  virtual void loop();
private:
  void setupDNS();

  WebServer          *m_WebServer;
  std::string         m_SSID;
  std::string         m_Passphrase;
  IPAddress           m_IPAddres;
  std::string         m_Hostname;
};

#endif /* _WIFI_CONNECTION_HPP_ */
