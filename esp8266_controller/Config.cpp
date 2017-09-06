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

#define EEPROM_CM_CONNECTIONTYPE_START  300
#define EEPROM_WS_TYPE_START            301
#define EEPROM_WS_MASTER_START          302
#define EEPROM_WS_MASTER_URL_LENGTH     40
#define EEPROM_GO_POSITIONG_START       342

#define EEPROM_MQTT_SERV_CFG_START            345
#define EEPROM_MQTT_SERV_DEVICE_NAME_START    346
#define EEPROM_MQTT_SERV_DEVICE_NAME_LENGTH   16
#define EEPROM_MQTT_SERV_SERIAL_START         362
#define EEPROM_MQTT_SERV_SERIAL_LENGTH        32
#define EEPROM_MQTT_SERV_CRC_START            394
#define EEPROM_MQTT_SERV_PORT_START           395
#define EEPROM_MQTT_SERV_PSW_START            397
#define EEPROM_MQTT_SERV_PSW_LENGTH           1

#define EEPROM_CHECKSUM_DATA_BEGIN      0
#define EEPROM_CHECKSUM_DATA_END        398
#define EEPROM_CHECKSUM_START           399



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
 // , WC_SSID("Marvins iPhone")
 // , WC_PASSPHRASE("marvin123")
 , WC_SSID("GondolaWiFi")
 , WC_PASSPHRASE("TU_GRAZ_ITI")
 , WC_HOSTNAME("gondola")
 // Access Point
 , AP_IPADDRESS(192, 168, 5, 1)
 , AP_GATEWAY(192, 168, 5, 1)
 , AP_NETMASK(255, 255, 255, 0)
 , AP_SSID("ESPBLANK")
 , AP_PASSPHRASE("alfafrancesco")
 , AP_URL("www.gondola.com")
 // ConnectionMgr Setup
 , CM_CONNECTIONTYPE(CON_MQTT_SERVER)
 // WebServer
 , WS_PORT(81)
 , WS_TYPE(SERV_NORMAL)
 , WS_MASTER_URL("gondola.local")
 // Gondola
 , GO_POSITION(0.0, 0.0, 0.0)
 // MQTT Server
 , MQTT_SERV_CFG(false)
 , MQTT_SERV_DEVICE_NAME("")
 , MQTT_SERV_SERIAL("")
 , MQTT_SERV_CRC("0")
 , MQTT_SERV_PORT(1883)
 , MQTT_SERV_PSW("0")
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

  EEPROM.write(EEPROM_CM_CONNECTIONTYPE_START, static_cast<uint8_t>(CM_CONNECTIONTYPE));

  EEPROM.write(EEPROM_WS_TYPE_START, static_cast<uint8_t>(WS_TYPE));
  persistString(    WS_MASTER_URL,  EEPROM_WS_MASTER_START,        EEPROM_WS_MASTER_URL_LENGTH);

  persistCoordinate(GO_POSITION, EEPROM_GO_POSITIONG_START);

  EEPROM.write(EEPROM_MQTT_SERV_CFG_START, static_cast<uint8_t>(MQTT_SERV_CFG));
  persistString(MQTT_SERV_DEVICE_NAME, EEPROM_MQTT_SERV_DEVICE_NAME_START, EEPROM_MQTT_SERV_DEVICE_NAME_LENGTH);
  persistString(MQTT_SERV_SERIAL, EEPROM_MQTT_SERV_SERIAL_START, EEPROM_MQTT_SERV_SERIAL_LENGTH);
  EEPROM.write(EEPROM_MQTT_SERV_CRC_START, MQTT_SERV_CRC);
  EEPROM.write(EEPROM_MQTT_SERV_PORT_START, static_cast<uint8_t>(MQTT_SERV_PORT & 0xFF));
  EEPROM.write(EEPROM_MQTT_SERV_PORT_START + 1, static_cast<uint8_t>((MQTT_SERV_PORT >> 8) & 0xFF));
  persistString(MQTT_SERV_PSW, EEPROM_MQTT_SERV_PSW_START, EEPROM_MQTT_SERV_PSW_LENGTH);

  writeChecksum(EEPROM_CHECKSUM_START);

  return EEPROM.commit();
}

void Config::readFromEEPROM()
{
  // Check validity of data
  if (checkChecksum(EEPROM_CHECKSUM_START) == false)
  {
    Log::logWarning("Checksum not valid! Use default values!\n");
    return;
  }
  else
  {
    Log::logInfo("Checksum valid! Load Data from EEPROM!\n");
  }
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

  CM_CONNECTIONTYPE = static_cast<connectionType_t>(EEPROM.read(EEPROM_CM_CONNECTIONTYPE_START));

  WS_TYPE = static_cast<serverType_t>(EEPROM.read(EEPROM_WS_TYPE_START));
  readString(    WS_MASTER_URL,  EEPROM_WS_MASTER_START,     EEPROM_WS_MASTER_URL_LENGTH);

  MQTT_SERV_CFG = static_cast<bool>(EEPROM.read(EEPROM_MQTT_SERV_CFG_START));
  readString(MQTT_SERV_DEVICE_NAME, EEPROM_MQTT_SERV_DEVICE_NAME_START, EEPROM_MQTT_SERV_DEVICE_NAME_LENGTH);
  readString(MQTT_SERV_SERIAL, EEPROM_MQTT_SERV_SERIAL_START, EEPROM_MQTT_SERV_SERIAL_LENGTH);
  MQTT_SERV_CRC = EEPROM.read(EEPROM_MQTT_SERV_CRC_START);
  MQTT_SERV_PORT = EEPROM.read(EEPROM_MQTT_SERV_PORT_START);
  MQTT_SERV_PORT |= EEPROM.read(EEPROM_MQTT_SERV_PORT_START + 1) << 8;
  readString(MQTT_SERV_PSW, EEPROM_MQTT_SERV_PSW_START, EEPROM_MQTT_SERV_PSW_LENGTH);

  readCoordinate(GO_POSITION, EEPROM_GO_POSITIONG_START);
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

void Config::persistCoordinate(Coordinate &coord, uint16_t start)
{
  EEPROM.write(start, coord.x);
  EEPROM.write(start + 1, coord.y);
  EEPROM.write(start + 2, coord.z);
}

void Config::readCoordinate(Coordinate &coord, uint16_t start)
{
  coord.x = EEPROM.read(start);
  coord.y = EEPROM.read(start + 1);
  coord.z = EEPROM.read(start + 2);
}

void Config::writeChecksum(uint16_t start)
{
  // Fletcher checksum
  uint8_t chk1 = 0, chk2 = 0;
  uint16_t i;
  for (i = EEPROM_CHECKSUM_DATA_BEGIN; i <= EEPROM_CHECKSUM_DATA_END; i++)
  {
    chk1 = chk1 + EEPROM.read(i);
    chk2 = chk2 + chk1;
  }
  EEPROM.write(EEPROM_CHECKSUM_START, chk1);
  EEPROM.write(EEPROM_CHECKSUM_START + 1, chk2);
}

bool Config::checkChecksum(uint16_t start)
{
  // Fletcher checksum
  uint8_t chk1 = 0, chk2 = 0;
  uint8_t chk1EEPROM = EEPROM.read(EEPROM_CHECKSUM_START);
  uint8_t chk2EEPROM = EEPROM.read(EEPROM_CHECKSUM_START + 1);
  uint16_t i;

  for (i = EEPROM_CHECKSUM_DATA_BEGIN; i <= EEPROM_CHECKSUM_DATA_END; i++)
  {
    chk1 = chk1 + EEPROM.read(i);
    chk2 = chk2 + chk1;
  }

  if (chk1 == chk1EEPROM && chk2 == chk2EEPROM)
    return true;
  else
    return false;
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

// Gondola
void Config::setGO_POSITION(Coordinate position)
{
  GO_POSITION = position;
}


// MQTT Server
void Config::setMQTT_SERV_CFG(bool cfg)
{
  MQTT_SERV_CFG = cfg;
}

void Config::setMQTT_SERV_DEVICE_NAME(std::string devname)
{
  MQTT_SERV_DEVICE_NAME = devname;
}

void Config::setMQTT_SERV_SERIAL(std::string serial)
{
  MQTT_SERV_SERIAL = serial;
}

void Config::setMQTT_SERV_CRC(uint8_t crc)
{
  MQTT_SERV_CRC = crc;
}

void Config::setMQTT_SERV_PORT(uint16_t port)
{
  MQTT_SERV_PORT = port;
}

void Config::setMQTT_SERV_PSW(std::string psw)
{
  MQTT_SERV_PSW = psw;
}
