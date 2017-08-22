#ifndef _AP_CONNECTION_HPP_
#define _AP_CONNECTION_HPP_

#include "IConnection.hpp"
#include "WebServer.hpp"
#include <DNSServer.h>

/**
 * Class to open an access point
 */
class APConnection : public IConnection
{
public:
  /**
   * Create an access point
   * @param  ssid       SSID of the access point
   * @param  passphrase passphrase of the access point
   * @param  ip         IP Address of the chip
   * @param  gateway    Gateway to use
   * @param  netmask    Netmask of the network
   * @param  url        URL to provide
   * @return            pointer to instance of access point
   */
  static APConnection *create(std::string ssid, std::string passphrase, IPAddress ip, IPAddress gateway, IPAddress netmask, std::string url);
  /**
   * virtual destructor
   */
  virtual ~APConnection();

  /**
   * Call this frequently to handle the webserver and the DNSServer
   */
  virtual void loop();

private:
  /**
   * Constructor
   * @param  ssid       SSID of the access point
   * @param  passphrase passphrase of the access point
   * @param  ip         IP Address of the chip
   * @param  gateway    Gateway to use
   * @param  netmask    Netmask of the network
   * @param  url        URL to provide
   * @return            pointer to instance of access point
   */
  APConnection(std::string ssid, std::string passphrase, IPAddress ip, IPAddress gateway, IPAddress netmask, std::string url);
  /**
   * Setup the DNS Server
   */
  void setupDNS();

  // Instance
  static APConnection     *s_Instance;

  // Membervariables
  std::string              m_SSID;
  std::string              m_Passphrase;
  IPAddress                m_IPAddress;
  IPAddress                m_Gateway;
  IPAddress                m_Netmask;
  DNSServer                m_DnsServer;
  std::string              m_URL;
};

#endif /* _AP_CONNECTION_HPP_ */
