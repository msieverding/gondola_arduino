#include "WiFiConnection.hpp"


WiFiConnection::WiFiConnection(WebServer *server, std::string ssid, std::string passphrase, std::string hostname)
 : m_WebServer(server)
 , m_SSID(ssid)
 , m_Passphrase(passphrase)
 , m_Hostname(hostname)
{
  WiFi.begin(m_SSID.c_str(), m_Passphrase.c_str());

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

  setupDNS();
}

WiFiConnection::~WiFiConnection()
{
  WiFi.disconnect();
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
