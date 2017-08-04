#ifndef _WEB_INTERFACE_HPP_
#define _WEB_INTERFACE_HPP_

#include <ESP8266WebServer.h>
#include <DNSServer.h>

class webInterface
{
public:
  webInterface();
  void start();
  IPAddress getIP();
  void task();

private:
  static ESP8266WebServer* getServerP();
  static void handleRoot();

  // Member variables
  ESP8266WebServer  m_Server;
  String            m_SSID;
  String            m_Passphrase;
  IPAddress         m_IP;
  IPAddress         m_Gateway;
  IPAddress         m_Netmask;
  DNSServer         m_DnsServer;
};

#endif
