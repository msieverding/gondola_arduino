#include "WiFiConnection.hpp"
#include "ConnectionMgr.hpp"
#include "Log.hpp"

WiFiConnection *WiFiConnection::s_Instance = NULL;

WiFiConnection *WiFiConnection::create(WebServer *webServer, std::string ssid, std::string passphrase, std::string hostname, IPAddress ip, IPAddress gw, IPAddress nm)
{
  if (!s_Instance)
    s_Instance = new WiFiConnection(webServer, ssid, passphrase, hostname, ip, gw, nm);
  return s_Instance;
}

WiFiConnection::WiFiConnection(WebServer *webServer, std::string ssid, std::string passphrase, std::string hostname, IPAddress ip, IPAddress gw, IPAddress nm)
 : m_WebServer(webServer)
 , m_SSID(ssid)
 , m_Passphrase(passphrase)
 , m_Hostname(hostname)
 , m_IPAddress(ip)
 , m_Gateway(gw)
 , m_Netmask(nm)
{
  // DHCP is activated if ip = gw = nm = 0.0.0.0
  WiFi.config(ip, gw, nm);
  // Connect to Network
  WiFi.begin(m_SSID.c_str(), m_Passphrase.c_str());
  // Set stationary mode
  WiFi.mode(WIFI_STA);

  Log::logInfo("Connect to WiFi: ");
  Log::logInfo(m_SSID.c_str());

  while(WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Log::logInfo(".");
  }
  Log::logInfo(" Ready\n");

  m_IPAddres = WiFi.localIP();
  // Print the IP address
  Log::logInfo("Local IP address: ");
  Log::logInfo(m_IPAddres.toString().c_str());
  Log::logInfo("\n");

  // Set hostname for Client
  // https://github.com/esp8266/Arduino/blob/master/doc/esp8266wifi/station-class.rst#disconnect
  WiFi.hostname(m_Hostname.c_str());

  // Setup
  setupMDNS();
}

WiFiConnection::~WiFiConnection()
{
  WiFi.disconnect(false);
  s_Instance = NULL;
}


void WiFiConnection::loop()
{
  m_WebServer->loop();
}

void WiFiConnection::setupMDNS()
{
  // Set up mDNS responder:
  // - first argument is the domain name, in this example
  //   the fully-qualified domain name is "esp8266.local"
  // - second argument is the IP address to advertise
  //   we send our IP address on the WiFi network
  if (!MDNS.begin(m_Hostname.c_str()))
  {
    Log::logWarning("Error setting up mDNS responder!");
    return;
  }
  Log::logInfo("Use this URL to connect (mDNS): http://");
  Log::logInfo(m_Hostname.c_str());
  Log::logInfo(".local/\n");

  // Add service to MDNS-SD
  MDNS.addService("http", "tcp", 80);
}
