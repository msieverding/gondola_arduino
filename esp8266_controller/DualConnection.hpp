#ifndef _DUAL_CONNECTION_
#define _DUAL_CONNECTION_

#include <Arduino.h>
#include <ESP8266mDNS.h>
#include <DNSServer.h>
#include <IPAddress.h>
#include "ESP8266WiFiType.h"

#include "IConnection.hpp"
#include "WebServer.hpp"

/**
 * Class for a WiFi Connection and an Access Point at the same tine
 */
class DualConnection : public IConnection
{
public:
  /**
   * Create the instance of the dual connection
   * @param  ap_ssid     SSID for the AP
   * @param  ap_pw       Passphrase for the AP
   * @param  ap_ip       IP for the AP
   * @param  ap_gw       Gateway for the AP
   * @param  ap_nm       Netmask for the AP
   * @param  ap_url      URL for the AP
   * @param  wc_ssid     SSID of the network to connect
   * @param  wc_pw       Passphrase of the network to connect
   * @param  wc_hostname Hostname of the ESP8266 in this network
   * @param  wc_ip       static IP to use / 0.0.0.0 for DHCP
   * @param  wc_gw       static gateway to use / 0.0.0.0 for DHCP
   * @param  wc_nm       static netmask to use / 0.0.0.0 for DHCP
   * @return             pointer to instance
   */
  static DualConnection *create(std::string ap_ssid, std::string ap_pw, IPAddress ap_ip, IPAddress ap_gw, IPAddress ap_nm, std::string ap_url,
                                std::string wc_ssid, std::string wc_pw, std::string wc_hostname, IPAddress wc_ip, IPAddress wc_gw, IPAddress wc_nm);
  /**
   * virtual destructor
   */
  virtual ~DualConnection();

  /**
   * Loop to call periodically
   */
  virtual void loop();
private:
  /**
   * Constructor of the dual connection
   * @param  ap_ssid     SSID for the AP
   * @param  ap_pw       Passphrase for the AP
   * @param  ap_ip       IP for the AP
   * @param  ap_gw       Gateway for the AP
   * @param  ap_nm       Netmask for the AP
   * @param  ap_url      URL for the AP
   * @param  wc_ssid     SSID of the network to connect
   * @param  wc_pw       Passphrase of the network to connect
   * @param  wc_hostname Hostname of the ESP8266 in this network
   * @param  wc_ip       static IP to use / 0.0.0.0 for DHCP
   * @param  wc_gw       static gateway to use / 0.0.0.0 for DHCP
   * @param  wc_nm       static netmask to use / 0.0.0.0 for DHCP
   */
  DualConnection(std::string ap_ssid, std::string ap_pw, IPAddress ap_ip, IPAddress ap_gw, IPAddress ap_nm, std::string ap_url,
                 std::string wc_ssid, std::string wc_pw, std::string wc_hostname, IPAddress wc_ip, IPAddress wc_gw, IPAddress wc_nm);

  /**
   * setup the WiFi Connection
   */
  void setupWiFiConnection();
  /**
   * setup mDNS for WiFi Connection
   */
  void setupMDNS();
  /**
   * Event handler when the WiFi Connection gets an IP
   * @param event event to handle
   */
  static void onEventGotIP(const WiFiEventStationModeGotIP &event);

  /**
   * setup the access point
   */
  void setupAccessPoint();
  /**
   * Setup DNS Serveer for access point
   */
  void setupDNS();

  // instance
  static DualConnection   *s_Instance;
  // membervariables
  // general
  WebServer               *m_WebServer;
  // access point
  std::string              m_AP_SSID;
  std::string              m_AP_Passphrase;
  IPAddress                m_AP_IPAddress;
  IPAddress                m_AP_Gateway;
  IPAddress                m_AP_Netmask;
  std::string              m_AP_URL;
  DNSServer                m_AP_DnsServer;
  WiFiEventHandler         m_APClientConnected;
  // wifi connection
  std::string              m_WC_SSID;
  std::string              m_WC_Passphrase;
  IPAddress                m_WC_IPAddress;
  std::string              m_WC_Hostname;
  IPAddress                m_WC_Gateway;
  IPAddress                m_WC_Netmask;
  WiFiEventHandler         m_StationGotIPHandler;


};
#endif /* _DUAL_CONNECTION_ */
