#include "Config.hpp"
#include <EEPROM.h>
#include "ConnectionMgr.hpp"
#include "CommandInterpreter.hpp"
#include "Log.hpp"

#define EEPROM_LENGTH                   512

#define EEPROM_WC_SSID_START            0
#define EEPROM_WC_SSID_LENGTH           40
#define EEPROM_WC_PASSPHRASAE_START     40
#define EEPROM_WC_PASSPHRASE_LENGTH     40
#define EEPROM_WC_IPADDRESS_START       80
#define EEPROM_WC_IPADDRESS_LENGTH      4
#define EEPROM_WC_GATEWAY_START         84
#define EEPROM_WC_GATEWAY_LENGTH        4
#define EEPROM_WC_NETMASK_START         88
#define EEPROM_WC_NETMASAK_LENGTH       4
#define EEPROM_WC_HOSTNAME_START        92
#define EEPROM_WC_HOSTNAME_LENGTH       40
// Placeholder for more features until address 149

#define EEPROM_AP_SSID_START            150
#define EEPROM_AP_SSID_LENGTH           40
#define EEPROM_AP_PASSPHRASAE_START     190
#define EEPROM_AP_PASSPHRASE_LENGTH     40
#define EEPROM_AP_IPADDRESS_START       230
#define EEPROM_AP_IPADDRESS_LENGTH      4
#define EEPROM_AP_GATEWAY_START         234
#define EEPROM_AP_GATEWAY_LENGTH        4
#define EEPROM_AP_NETMASK_START         238
#define EEPROM_AP_NETMASAK_LENGTH       4
#define EEPROM_AP_URL_START             242
#define EEPROM_AP_URL_LENGTH            40
// Placeholder for more features until address 299

#define EEPROM_CM_connectionType_sTART         300
#define EEPROM_WS_TYPE_START            301
#define EEPROM_WS_MASTER_START          302
#define EEPROM_WS_MASTER_URL_LENGTH     40



Config *Config::s_Instance = NULL;

Config *Config::get()
{
  if (!s_Instance)
    s_Instance = new Config();

  return s_Instance;
}

Config::Config()
 // WiFi Connection
 : WC_IPADDRESS(0, 0, 0, 0)
 , WC_GATEWAY(0, 0, 0, 0)
 , WC_NETMASK(0, 0, 0, 0)
 // , WC_SSID("Virus.exe")
 // , WC_PASSPHRASE("10542284208956097103")
 , WC_SSID("FRITZ!Box 7360")
 , WC_PASSPHRASE("04688692052151450821")
 , WC_HOSTNAME("gondola")
 // Access Point
 , AP_IPADDRESS(192, 168, 5, 10)
 , AP_GATEWAY(192, 168, 5, 1)
 , AP_NETMASK(255, 255, 255, 0)
 , AP_SSID("GondolaWiFi")
 , AP_PASSPHRASE("TU_GRAZ_ITI")
 , AP_URL("www.gondola-slave.com")
 // ConnectionMgr Setup
 , CM_CONNECTIONTYPE(CON_DUAL_CONNECTION)
 // WebServer
 , WS_PORT(80)
 , WS_TYPE(SERV_NORMAL)
 , WS_MASTER_URL("www.gondola-master.com")
 // Serial
 , SE_BAUDRATE(115200)
{
  CommandInterpreter::get()->addCommand("configReset", configResetCommand);
  EEPROM.begin(EEPROM_LENGTH);
}

Config::~Config()
{
  CommandInterpreter::get()->deleteCommand("configReset", configResetCommand);
  EEPROM.end();
  s_Instance = NULL;
}

void Config::resetConfig()
{
  delete(s_Instance);
  s_Instance = new Config();
  s_Instance->writeToEEPROM();
  // TODO Chip will crash here since the destructor will change list wich is
  // used to call this function
}

void Config::configResetCommand(std::string &s)
{
  Log::logDebug("CI: configResetCommand: ");
  Log::logDebug(s);
  resetConfig();
}

bool Config::writeToEEPROM()
{
  // WiFi Connection Setup
  persistString(    WC_SSID,        EEPROM_WC_SSID_START,           EEPROM_WC_SSID_LENGTH);
  persistString(    WC_PASSPHRASE,  EEPROM_WC_PASSPHRASAE_START,    EEPROM_WC_PASSPHRASE_LENGTH);
  persistIPAddress( WC_IPADDRESS,   EEPROM_WC_IPADDRESS_START);
  persistIPAddress( WC_GATEWAY,     EEPROM_WC_GATEWAY_START);
  persistIPAddress( WC_NETMASK,     EEPROM_WC_NETMASK_START);
  persistString(    WC_HOSTNAME,    EEPROM_WC_HOSTNAME_START,       EEPROM_WC_HOSTNAME_LENGTH);

  // Access Point Setup
  persistString(    AP_SSID,        EEPROM_AP_SSID_START,           EEPROM_AP_SSID_LENGTH);
  persistString(    AP_PASSPHRASE,  EEPROM_AP_PASSPHRASAE_START,    EEPROM_AP_PASSPHRASE_LENGTH);
  persistIPAddress( AP_IPADDRESS,   EEPROM_AP_IPADDRESS_START);
  persistIPAddress( AP_GATEWAY,     EEPROM_AP_GATEWAY_START);
  persistIPAddress( AP_NETMASK,     EEPROM_AP_NETMASK_START);
  persistString(    AP_URL,         EEPROM_AP_URL_START,            EEPROM_AP_URL_LENGTH);

  EEPROM.write(EEPROM_CM_connectionType_sTART, static_cast<uint8_t>(CM_CONNECTIONTYPE));

  EEPROM.write(EEPROM_WS_TYPE_START, static_cast<uint8_t>(WS_TYPE));
  persistString(    WS_MASTER_URL,  EEPROM_WS_MASTER_START,        EEPROM_WS_MASTER_URL_LENGTH);

  return EEPROM.commit();
}

void Config::readFromEEPROM()
{
  // WiFI Connection Setup
  readString(    WC_SSID,        EEPROM_WC_SSID_START,           EEPROM_WC_SSID_LENGTH);
  readString(    WC_PASSPHRASE,  EEPROM_WC_PASSPHRASAE_START,    EEPROM_WC_PASSPHRASE_LENGTH);
  readIPAddress( WC_IPADDRESS,   EEPROM_WC_IPADDRESS_START);
  readIPAddress( WC_GATEWAY,     EEPROM_WC_GATEWAY_START);
  readIPAddress( WC_IPADDRESS,   EEPROM_WC_IPADDRESS_START);
  readString(    WC_HOSTNAME,    EEPROM_WC_HOSTNAME_START,       EEPROM_WC_HOSTNAME_LENGTH);

  // Access Point Setup
  readString(    AP_SSID,        EEPROM_AP_SSID_START,           EEPROM_AP_SSID_LENGTH);
  readString(    AP_PASSPHRASE,  EEPROM_AP_PASSPHRASAE_START,    EEPROM_AP_PASSPHRASE_LENGTH);
  readIPAddress( AP_IPADDRESS,   EEPROM_AP_IPADDRESS_START);
  readIPAddress( AP_GATEWAY,     EEPROM_AP_GATEWAY_START);
  readIPAddress( AP_NETMASK,     EEPROM_AP_NETMASK_START);
  readString(    AP_URL,         EEPROM_AP_URL_START,            EEPROM_AP_URL_LENGTH);

  CM_CONNECTIONTYPE = static_cast<connectionType_t>(EEPROM.read(EEPROM_CM_connectionType_sTART));

  WS_TYPE = static_cast<serverType_t>(EEPROM.read(EEPROM_WS_TYPE_START));
  readString(    WS_MASTER_URL,  EEPROM_WS_MASTER_START,     EEPROM_WS_MASTER_URL_LENGTH);
}

void Config::persistString(std::string &s, uint16_t start, uint8_t maxLength)
{
  int length = s.length();
  uint8_t byte;

  // Figure out max length
  if (length > maxLength)
    length = maxLength;

  // write string bytewise
  for (byte = 0; byte < length; byte++)
    EEPROM.write(start + byte, static_cast<uint8_t>(s[byte]));

  // when string has not max length add end sequence
  if (length < maxLength)
    EEPROM.write(start + byte, static_cast<uint8_t>('\0'));
}

void Config::readString(std::string &s, uint16_t start, uint8_t maxLength)
{
  int length = 0;
  uint8_t byte;
  char * str = new char[maxLength + 1];

  s.clear();
  for (byte = 0; byte < maxLength; byte++)
  {
    char c = EEPROM.read(start + byte);
    if (c != '\0')
      str[byte] = c;
    else
      break;
  }
  str[byte] = '\0';
  s.append(str);
}

void Config::persistIPAddress(IPAddress &ip, uint16_t start)
{
  for (uint8_t i = 0; i < 4; i++)
    EEPROM.write(start + i, ip[i]);
}

void Config::readIPAddress(IPAddress &ip, uint16_t start)
{
  for (uint8_t i = 0; i < 4; i++)
    ip[i] = EEPROM.read(start + i);
}

// WiFi Connection

void Config::setWC_IPADDRESS(IPAddress ip)
{
  WC_IPADDRESS = ip;
}

void Config::setWC_GATEWAY(IPAddress gw)
{
  WC_GATEWAY = gw;
}

void Config::setWC_NETMASK(IPAddress nm)
{
  WC_NETMASK = nm;
}

void Config::setWC_IPADDRESS(String ip)
{
  IPAddress tmp;
  tmp.fromString(ip);
  setWC_IPADDRESS(tmp);
}

void Config::setWC_GATEWAY(String gw)
{
  IPAddress tmp;
  tmp.fromString(gw);
  setWC_GATEWAY(tmp);
}

void Config::setWC_NETMASK(String nm)
{
  IPAddress tmp;
  tmp.fromString(nm);
  setWC_NETMASK(tmp);
}

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
  setAP_IPADDRESS(tmp);
}

void Config::setAP_GATEWAY(String gw)
{
  IPAddress tmp;
  tmp.fromString(gw);
  setAP_GATEWAY(tmp);
}

void Config::setAP_NETMASK(String nm)
{
  IPAddress tmp;
  tmp.fromString(nm);
  setAP_NETMASK(tmp);
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

// ConnectionMgr
void Config::setCM_CONNECTIONTYPE(connectionType_t connectionType)
{
  CM_CONNECTIONTYPE = connectionType;
}


void Config::setWS_TYPE(serverType_t serverType)
{
  WS_TYPE = serverType;
}

void Config::setWS_MASTER_URL(std::string url)
{
  WS_MASTER_URL = url;
}
