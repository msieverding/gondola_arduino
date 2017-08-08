#ifndef _AP_CONNECTION_HPP_
#define _AP_CONNECTION_HPP_

#include "IConnection.hpp"
#include "WebServer.hpp"
#include <DNSServer.h>

class APConnection : public IConnection
{
public:
  APConnection(WebServer *server, std::string ssid, std::string passphrase, IPAddress ip, IPAddress gateway, IPAddress netmask, std::string url);
  virtual ~APConnection();

  virtual void loop();
private:
  void setupDNS();

  WebServer       *m_WebServer;
  std::string      m_SSID;
  std::string      m_Passphrase;
  IPAddress        m_IPAddress;
  IPAddress        m_Gateway;
  IPAddress        m_Netmask;
  DNSServer        m_DnsServer;
  std::string      m_URL;
};

#endif /* _AP_CONNECTION_HPP_ */
