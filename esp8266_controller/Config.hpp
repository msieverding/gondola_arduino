#ifndef _CONFIG_HPP_
#define _CONFIG_HPP_

#include <string>
#include "Coordinate.hpp"
#include <IPAddress.h>

// Forward declaration of connectionType_t from ConnectionMgr
enum connectionType_s : byte;
typedef enum connectionType_s connectionType_t;
// Forward declaration of serverType from ConnectionMgr
enum serverType_s : byte;
typedef enum serverType_s serverType_t;

/**
 * Singleton class to store the setup of the application
 */
class Config
{
public:
  /**
   * get configuration instance
   * @return pointer to instance
   */
  static Config *get();

  /**
   * virtual destructor
   */
  virtual ~Config();

  /**
   * save the setup to the internal EEPROM
   * @return success
   */
  bool writeToEEPROM();

  /**
   * read the setup from the internal EEPROM
   */
  void readFromEEPROM();

  /**
   * load the default config. Use writeToEEPROM to store it into the EEPROM after loading it
   */
  static void resetConfig();

  /**
   * Command to reset the config with serial input
   * @param s line to interprete
   */
  static void configResetCommand(std::string &s);

private:
  /**
   * private constrcutor
   */
  Config();

public:
  // WiFi Connection
  std::string getWC_SSID() { return WC_SSID; }
  std::string getWC_PASSPHRASE() { return WC_PASSPHRASE; }
  std::string getWC_HOSTNAME() { return WC_HOSTNAME; }
  IPAddress getWC_IPADDRESS() { return WC_IPADDRESS; }
  IPAddress getWC_GATEWAY() { return WC_GATEWAY; }
  IPAddress getWC_NETMASK() { return WC_NETMASK; }

  void setWC_SSID(std::string ssid);
  void setWC_PASSPHRASE(std::string passphrase);
  void setWC_HOSTNAME(std::string name);
  void setWC_IPADDRESS(IPAddress ip);
  void setWC_IPADDRESS(String ip);
  void setWC_GATEWAY(IPAddress gw);
  void setWC_GATEWAY(String gw);
  void setWC_NETMASK(IPAddress nm);
  void setWC_NETMASK(String nm);

  // Access Point
  IPAddress getAP_IPADDRESS() { return AP_IPADDRESS; }
  IPAddress getAP_GATEWAY() { return AP_GATEWAY; }
  IPAddress getAP_NETMASK() { return AP_NETMASK; }
  std::string getAP_SSID() { return AP_SSID; }
  std::string getAP_PASSPHRASE() { return AP_PASSPHRASE; }
  std::string getAP_URL() { return AP_URL; }

  void setAP_IPADDRESS(IPAddress ip);
  void setAP_IPADDRESS(String ip);
  void setAP_GATEWAY(IPAddress gw);
  void setAP_GATEWAY(String gw);
  void setAP_NETMASK(IPAddress nm);
  void setAP_NETMASK(String nm);
  void setAP_SSID(std::string ssid);
  void setAP_PASSPHRASE(std::string passphrase);
  void setAP_URL(std::string url);

  // ConnectionMgr
  void setCM_CONNECTIONTYPE(connectionType_t connecrtionType);
  connectionType_t getCM_CONNECTIONTYPE() { return CM_CONNECTIONTYPE; }

  // WebServer
  uint16_t getWS_PORT() { return WS_PORT; }
  void setWS_TYPE(serverType_t serverType);
  serverType_t getWS_TYPE() { return WS_TYPE; }
  void setWS_MASTER_URL(std::string url);
  std::string getWS_MASTER_URL() { return WS_MASTER_URL; }

  // Gondola
  Coordinate getGO_POSITION(void) { return GO_POSITION; }
  void setGO_POSITION(Coordinate position);

  // MQTT Server
  uint8_t getMQTT_SERV_CFG() { return MQTT_SERV_CFG; }
  void setMQTT_SERV_CFG(uint8_t cfg);
  std::string getMQTT_SERV_DEVICE_NAME() { return MQTT_SERV_DEVICE_NAME; }
  void setMQTT_SERV_DEVICE_NAME(std::string devname);
  std::string getMQTT_SERV_SERIAL() { return MQTT_SERV_SERIAL; }
  void setMQTT_SERV_SERIAL(std::string serial);
  uint8_t getMQTT_SERV_CRC() { return MQTT_SERV_CRC; }
  void setMQTT_SERV_CRC(uint8_t crc);
  uint16_t getMQTT_SERV_PORT() { return MQTT_SERV_PORT; }
  void setMQTT_SERV_PORT(uint16_t port);
  std::string getMQTT_SERV_PSW() { return MQTT_SERV_PSW; }
  void setMQTT_SERV_PSW(std::string psw);

private:
  /**
   * persist a string to the EEPROM
   * If string doesn't use the maximum length a '\0' charachter will be appended
   * @param s         string to persist
   * @param start     start address in EEPRON
   * @param maxLength maximum length in EEPROM
   */
  void persistString(std::string &s, uint16_t start, uint8_t maxLength);

  /**
   * Read a string from a given address
   * @param s         string to read
   * @param start     start address in EERPON
   * @param maxLength maximum length of string
   */
  void readString(std::string &s, uint16_t start, uint8_t maxLength);

  /**
   * Write an IPAddres to the EEPROM
   * @param ip    IPAddress to save
   * @param start start address in EEPROM
   */
  void persistIPAddress(IPAddress &ip, uint16_t start);

  /**
   * Read an IPAddress from the EEPROM
   * @param ip    IPAddress to read
   * @param start start address in EEPROM
   */
  void readIPAddress(IPAddress &ip, uint16_t start);

  /**
   * Write a coordinate to the EEPROM
   * @param coord Coordinate to persist
   * @param start start address in EEPROM
   */
  void persistCoordinate(Coordinate &coord, uint16_t start);

  /**
   * Read a coordinate from the EEPROM
   * @param coord Coordinate to read
   * @param start start address in EEPROM
   */
  void readCoordinate(Coordinate &coord, uint16_t start);

  // Checksum
  /**
   * Calculates and erites the checksum to the EEPROM
   * @param start start address of checksum in EEPROM
   */
  void writeChecksum(uint16_t start);

  /**
   * Check the checksum from EEPROM with EEPROM Data
   * @param  start start address of checksum in EEPROM
   * @return       true if checksum is valid, false otherwise
   */
  bool checkChecksum(uint16_t start);

  // instance of Configuration
  static Config       *s_Instance;

  // WiFi Connection
  IPAddress WC_IPADDRESS;
  IPAddress WC_GATEWAY;
  IPAddress WC_NETMASK;
  std::string WC_SSID;
  std::string WC_PASSPHRASE;
  std::string WC_HOSTNAME;

  // Access Point
  IPAddress AP_IPADDRESS;
  IPAddress AP_GATEWAY;
  IPAddress AP_NETMASK;
  std::string AP_SSID;
  std::string AP_PASSPHRASE;
  std::string AP_URL;

  // ConnectionMgr
  connectionType_t CM_CONNECTIONTYPE;

  // WebServer
  uint16_t WS_PORT;
  serverType_t WS_TYPE;
  std::string WS_MASTER_URL;

  // Gondola
  Coordinate GO_POSITION;

  // MQTT Server
  uint8_t MQTT_SERV_CFG;
  std::string MQTT_SERV_DEVICE_NAME;
  std::string MQTT_SERV_SERIAL;
  uint8_t MQTT_SERV_CRC;
  uint16_t MQTT_SERV_PORT;
  std::string MQTT_SERV_PSW;
};

// STEPPER SETTINGS
// https://www.allaboutcircuits.com/tools/stepper-motor-calculator/ and
// 42BYGHW811 Wantai stepper motor)
// delay for stepper in microseconds (computer for 32 microsteps, using calculator
#define STEP_DELAY     9000

// reduced in precision (0.05 cm = MIN_PRECISION = 1step = 1.8', 1 cm =
// 20steps = 36', 10 cm = 200steps = 360')
// the difference between the old distance and the new one is then
#define STEP_CM        20.0f

// precision of 1 step in cm
#define MIN_PRECISION ((float)(1 / STEP_CM))
#define MICROSTEPS 16L

#endif /* _CONFIG_HPP_ */
