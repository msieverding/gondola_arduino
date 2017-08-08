#include "APConnection.hpp"
#include "config.hpp"

APConnection::APConnection(WebServer *server, std::string ssid, std::string passphrase, IPAddress ip, IPAddress gateway, IPAddress netmask)
 : m_WebServer(server)
 , m_SSID(ssid)
 , m_Passphrase(passphrase)
 , m_IPAddress(ip)
 , m_Gateway(gateway)
 , m_Netmask(netmask)
 , m_DnsServer()
{
  WiFi.mode(WIFI_AP_STA);

  Serial.print("Setting soft-AP configuration ... ");
  Serial.println(WiFi.softAPConfig(m_IPAddress, m_Gateway, m_Netmask) ? "Ready" : "Failed!");

  Serial.print("Setting soft-AP ... ");
  Serial.println(WiFi.softAP(m_SSID.c_str(), m_Passphrase.c_str()) ? "Ready" : "Failed!");

  Serial.print("Soft-AP IP address = ");
  Serial.println(WiFi.softAPIP());

  Serial.print("Opened WiFi AP! Connect with passphrase: ");
  Serial.println(m_Passphrase.c_str());

  setupDNS();
  Serial.print("You can access gondola's main page with: ");
  Serial.println(AP_URL.c_str());
}

APConnection::~APConnection()
{
  m_DnsServer.stop();
  WiFi.softAPdisconnect();
}

void APConnection::loop()
{
  m_DnsServer.processNextRequest();
  m_WebServer->loop();
}

void APConnection::setupDNS()
{
  // Setup DNS server for easy access without IP address
  // modify TTL associated  with the domain name (in seconds)
  // default is 60 seconds
  m_DnsServer.setTTL(300);
  // set which return code will be used for all other domains (e.g. sending
  // ServerFailure instead of NonExistentDomain will reduce number of queries
  // sent by clients)
  // default is DNSReplyCode::NonExistentDomain
  m_DnsServer.setErrorReplyCode(DNSReplyCode::ServerFailure);

  // start DNS server for a specific domain name
  m_DnsServer.start(53, AP_URL.c_str(), m_IPAddress);
}
