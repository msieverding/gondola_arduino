#include "WiFiConnection.hpp"
#include "ConnectionMgr.hpp"

WiFiConnection::WiFiConnection(WebServer *server, std::string ssid, std::string passphrase, std::string hostname)
 : m_WebServer(server)
 , m_SSID(ssid)
 , m_Passphrase(passphrase)
 , m_Hostname(hostname)
{
  Serial.print("WiFi.status()");
  Serial.println(WiFi.status());

  // TODO Disable/Enable DHCP could be an option. Following line enables DHCP
  // WiFi.config(IPAddress(0, 0, 0, 0), IPAddress(0, 0, 0, 0), IPAddress(0, 0, 0, 0));

  // Connect to Network
  WiFi.begin(m_SSID.c_str(), m_Passphrase.c_str());
  // Set stationary mode
  WiFi.mode(WIFI_STA);

  Serial.print("Connect to WiFi: ");
  Serial.print(m_SSID.c_str());

  while(WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" Ready");

  m_IPAddres = WiFi.localIP();
  // Print the IP address
  Serial.print("Local IP address: ");
  Serial.println(m_IPAddres);

  // TODO usw WiFi.hostname("xxx") to set dhcp host name
  // https://github.com/esp8266/Arduino/blob/master/doc/esp8266wifi/station-class.rst#disconnect
  // WiFi.hostname(m_Hostname.c_str());
  // setupDNS();
}

WiFiConnection::~WiFiConnection()
{
  Serial.println("Destruct WiFi Connection");
  Serial.println(WiFi.disconnect(false));
}


void WiFiConnection::loop()
{
  m_WebServer->loop();
}

void WiFiConnection::setupDNS()
{
  // Set up mDNS responder:
  // - first argument is the domain name, in this example
  //   the fully-qualified domain name is "esp8266.local"
  // - second argument is the IP address to advertise
  //   we send our IP address on the WiFi network
  if (!MDNS.begin(m_Hostname.c_str()))
  {
    Serial.println("Error setting up mDNS responder!");
    return;
  }
  Serial.print("Use this URL to connect (mDNS): http://");
  Serial.print(m_Hostname.c_str());
  Serial.println(".local/");

  // Add service to MDNS-SD
  MDNS.addService("http", "tcp", 80);
}
