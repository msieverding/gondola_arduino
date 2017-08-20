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
  static WiFiConnection *create(WebServer *server, std::string ssid, std::string passphrase, std::string hostname, IPAddress ip, IPAddress gw, IPAddress nm);
  virtual ~WiFiConnection();

  virtual void loop();
private:
  WiFiConnection(WebServer *server, std::string ssid, std::string passphrase, std::string hostname, IPAddress ip, IPAddress gw, IPAddress nm);
  void setupDNS();

  static WiFiConnection     *s_Instance;

  WebServer                 *m_WebServer;
  std::string                m_SSID;
  std::string                m_Passphrase;
  IPAddress                  m_IPAddres;
  std::string                m_Hostname;
  IPAddress                  m_IPAddress;
  IPAddress                  m_Gateway;
  IPAddress                  m_Netmask;
};

#endif /* _WIFI_CONNECTION_HPP_ */
