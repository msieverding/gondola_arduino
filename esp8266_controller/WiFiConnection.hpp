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
   * @param  ssid       SSID of network
   * @param  passphrase Passphrase for network with ssid
   * @param  hostname   Hostname to use during connection
   * @param  ip         IP address to use during static IP operation / 0.0.0.0 for DHCP
   * @param  gw         Gateway to use during static IP operation / 0.0.0.0 for DHCP
   * @param  nm         Netmask to use during static IP operation / 0.0.0.0 for DHCP
   * @return            Pointer to instance of singleton
   */
  static WiFiConnection *create(std::string ssid, std::string passphrase, std::string hostname, IPAddress ip, IPAddress gw, IPAddress nm);

  /**
   * Destructor
   */
  virtual ~WiFiConnection();

private:
  /**
   * private Constructor
   * @param  ssid       SSID of network
   * @param  passphrase Passphrase for network with ssid
   * @param  hostname   Hostname to use during connection
   * @param  ip         IP address to use during static IP operation / 0.0.0.0 for DHCP
   * @param  gw         Gateway to use during static IP operation / 0.0.0.0 for DHCP
   * @param  nm         Netmask to use during static IP operation / 0.0.0.0 for DHCP
   */
  WiFiConnection(std::string ssid, std::string passphrase, std::string hostname, IPAddress ip, IPAddress gw, IPAddress nm);

  /**
   * Setup the MDNS functionality
   * Needed host software:
   * - Linux: Avahi (http://avahi.org/)
   * - Windows: Bonjour (http://www.apple.com/support/bonjour/)
   * - Max OSX and iOs is built in through Bonjour already
   */
  void setupMDNS();

  // instance
  static WiFiConnection     *s_Instance;        //!< Instace of the connection
  // Membervariables
  std::string                m_SSID;            //!< SSID to connect to
  std::string                m_Passphrase;      //!< Passphrase to use for the network
  IPAddress                  m_IPAddress;       //!< IPaddres to use / 0.0.0.0 for DHCP
  std::string                m_Hostname;        //!< Hostname for this chip
  IPAddress                  m_Gateway;         //!< Gateway to use / 0.0.0.0 for DHCP
  IPAddress                  m_Netmask;         //!< Netmask to use / 0.0.0.0 for DHCP
};

#endif /* _WIFI_CONNECTION_HPP_ */
