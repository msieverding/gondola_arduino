#include "APConnection.hpp"
#include "config.hpp"
#include "Log.hpp"

APConnection *APConnection::s_Instance = NULL;

APConnection *APConnection::create(std::string ssid, std::string passphrase, IPAddress ip, IPAddress gateway, IPAddress netmask, std::string url)
{
  if (!s_Instance)
    s_Instance = new APConnection(ssid, passphrase, ip, gateway, netmask, url);
  return s_Instance;
}

APConnection::APConnection(std::string ssid, std::string passphrase, IPAddress ip, IPAddress gateway, IPAddress netmask, std::string url)
 : m_SSID(ssid)
 , m_Passphrase(passphrase)
 , m_IPAddress(ip)
 , m_Gateway(gateway)
 , m_Netmask(netmask)
 , m_DnsServer()
 , m_URL(url)
{
  WiFi.mode(WIFI_AP);
  WiFi.setOutputPower(20.5);            // set highest WiFi power
  WiFi.setPhyMode(WIFI_PHY_MODE_11N);   // activate mixed WiFi

  Log::logInfo("Setting soft-AP configuration... ");
  Log::logInfo(WiFi.softAPConfig(m_IPAddress, m_Gateway, m_Netmask) ? "Ready\n" : "Failed!\n");

  Log::logInfo("Setting soft-AP... ");
  if (m_Passphrase.length() >= 8 && m_Passphrase.length() <= 63)
    Log::logInfo(WiFi.softAP(m_SSID.c_str(), m_Passphrase.c_str()) ? "Ready\n" : "Failed!\n");
  else
    Log::logInfo(WiFi.softAP(m_SSID.c_str()) ? "Ready\n" : "Failed!\n");

  Log::logInfo("Soft-AP IP address: ");
  Log::logInfo(WiFi.softAPIP().toString().c_str());
  Log::logInfo("\n");

  Log::logInfo("WiFi AP SSID: ");
  Log::logInfo(m_SSID.c_str());
  Log::logInfo("\n");

  if (m_Passphrase.length() > 8 && m_Passphrase.length() < 32)
  {
    Log::logInfo("Connect with passphrase: ");
    Log::logInfo(m_Passphrase.c_str());
    Log::logInfo("\n");
  }
  else
  {
    Log::logInfo("No passphrase used\n");
  }

  setupDNS();
}

APConnection::~APConnection()
{
  m_DnsServer.stop();
  WiFi.softAPdisconnect(false);
  WiFi.disconnect(false);
}

void APConnection::loop()
{
  m_DnsServer.processNextRequest();
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
  m_DnsServer.start(53, m_URL.c_str(), m_IPAddress);

  Log::logInfo("You can access gondola's main page with: http://");
  Log::logInfo(m_URL.c_str());
  Log::logInfo("/\n");
}
