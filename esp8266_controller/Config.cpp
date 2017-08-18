#include "Config.hpp"

// WiFi Connection
// std::string Config::WC_SSID("Virus.exe");
// std::string Config::WC_PASSPHRASE("10542284208956097103");
std::string Config::WC_SSID("FRITZ!Box 7360");
std::string Config::WC_PASSPHRASE("04688692052151450821");
std::string Config::WC_HOSTNAME("gondola");

void Config::setWC_SSID(std::string ssid)
{
  WC_SSID = ssid;
}

void Config::setWC_PASSPHRASE(std::string passphrase)
{
  WC_PASSPHRASE = passphrase;
}

void Config::setWC_HOSTNAME(std::string name)
{
  WC_HOSTNAME = name;
}

// Access Point
IPAddress Config::AP_IPADDRESS(192, 168, 5, 10);
IPAddress Config::AP_GATEWAY(192, 168, 5, 1);
IPAddress Config::AP_NETMASK(255, 255, 255, 0);
std::string Config::AP_SSID("GondolaWifi");
std::string Config::AP_PASSPHRASE("TU_GRAZ_ITI");
std::string Config::AP_URL("www.gondola.com");

void Config::setAP_IPADDRESS(IPAddress ip)
{
  AP_IPADDRESS = ip;
}

void Config::setAP_GATEWAY(IPAddress gw)
{
  AP_GATEWAY = gw;
}

void Config::setAP_NETMASK(IPAddress nm)
{
  AP_NETMASK = nm;
}

void Config::setAP_IPADDRESS(String ip)
{
  IPAddress tmp;
  tmp.fromString(ip);
  AP_IPADDRESS = tmp;
}

void Config::setAP_GATEWAY(String gw)
{
  IPAddress tmp;
  tmp.fromString(gw);
  AP_GATEWAY = tmp;
}

void Config::setAP_NETMASK(String nm)
{
  IPAddress tmp;
  tmp.fromString(nm);
  AP_NETMASK = tmp;
}

void Config::setAP_SSID(std::string ssid)
{
  AP_SSID = ssid;
}

void Config::setAP_PASSPHRASE(std::string passphrase)
{
  AP_PASSPHRASE = passphrase;
}

void Config::setAP_URL(std::string url)
{
  AP_URL = url;
}

// WebServer
uint16_t Config::WS_PORT = 80;

// Serial
uint32_t Config::SE_BAUDRATE = 115200;
