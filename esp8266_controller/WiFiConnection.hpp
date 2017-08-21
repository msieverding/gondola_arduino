#ifndef _WIFI_CONNECTION_HPP_
#define _WIFI_CONNECTION_HPP_

#include <ESP8266WiFi.h>
#include "Gondola.hpp"
#include "WebServer.hpp"
#include "IConnection.hpp"
#include <ESP8266mDNS.h>

/**
 * Singleton for a WiFi Connection to an established WiFi Network.
 */
class WiFiConnection : public IConnection
{
public:
  /**
   * Create instance of WiFiConnection
   * @param  server     WebServer to handle with this connection
   * @param  ssid       SSID of network
   * @param  passphrase Passphrase for network with ssid
   * @param  hostname   Hostname to use during connection
   * @param  ip         IP address to use during static IP operation / 0.0.0.0 for DHCP
   * @param  gw         Gateway to use during static IP operation / 0.0.0.0 for DHCP
   * @param  nm         Netmask to use during static IP operation / 0.0.0.0 for DHCP
   * @return            Pointer to instance of singleton
   */
  static WiFiConnection *create(WebServer *webServer, std::string ssid, std::string passphrase, std::string hostname, IPAddress ip, IPAddress gw, IPAddress nm);

  /**
   * Destructor
   */
  virtual ~WiFiConnection();

  /**
   * Call this loop in periodically to handle WebServer and DNS requests
   */
  virtual void loop();

private:
  /**
   * private Constructor
   * @param  server     WebServer to handle with this connection
   * @param  ssid       SSID of network
   * @param  passphrase Passphrase for network with ssid
   * @param  hostname   Hostname to use during connection
   * @param  ip         IP address to use during static IP operation / 0.0.0.0 for DHCP
   * @param  gw         Gateway to use during static IP operation / 0.0.0.0 for DHCP
   * @param  nm         Netmask to use during static IP operation / 0.0.0.0 for DHCP
   */
  WiFiConnection(WebServer *webServer, std::string ssid, std::string passphrase, std::string hostname, IPAddress ip, IPAddress gw, IPAddress nm);

  /**
   * Setup the MDNS functionality
   * Needed host software:
   * - Linux: Avahi (http://avahi.org/)
   * - Windows: Bonjour (http://www.apple.com/support/bonjour/)
   * - Max OSX and iOs is built in through Bonjour already
   */
  void setupMDNS();

  // instance
  static WiFiConnection     *s_Instance;
  // Membervariables
  WebServer                 *m_WebServer;
  std::string                m_SSID;
  std::string                m_Passphrase;
  IPAddress                  m_IPAddress;
  std::string                m_Hostname;
  IPAddress                  m_Gateway;
  IPAddress                  m_Netmask;
};

#endif /* _WIFI_CONNECTION_HPP_ */
