#include "WiFiConnection.hpp"


WiFiConnection::WiFiConnection(WebServer *server, std::string ssid, std::string passphrase, std::string hostname)
 : m_WebServer(server)
 , m_SSID(ssid)
 , m_Passphrase(passphrase)
 , m_Hostname(hostname)
{
  WiFi.begin(m_SSID.c_str(), m_Passphrase.c_str());

  Serial.print("\nConnect to WiFi: ");
  Serial.print(m_SSID.c_str());

  while(WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  m_IPAddres = WiFi.localIP();
  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(m_IPAddres);
  Serial.println("/");

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
    Serial.println("Error setting up MDNS responder!");
    return;
  }
  Serial.println("mDNS responder started");
  Serial.print("http://");
  Serial.print(m_Hostname.c_str());
  Serial.print(".local")
  Serial.println("/");

  // Add service to MDNS-SD
  MDNS.addService("http", "tcp", 80);
}
