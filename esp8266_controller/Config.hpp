#ifndef _CONFIG_HPP_
#define _CONFIG_HPP_

#include <string>
#include "Anchor.hpp"
#include "Coordinate.hpp"
#include <IPAddress.h>

class Config
{
public:
  // WiFi Connection
  static std::string getWC_SSID() { return WC_SSID; }
  static std::string getWC_PASSPHRASE() { return WC_PASSPHRASE; }
  static std::string getWC_HOSTNAME() { return WC_HOSTNAME; }

  static void setWC_SSID(std::string ssid);
  static void setWC_PASSPHRASE(std::string passphrase);
  static void setWC_HOSTNAME(std::string name);

  // Access Point
  static IPAddress getAP_IPADDRESS() { return AP_IPADDRESS; }
  static IPAddress getAP_GATEWAY() { return AP_GATEWAY; }
  static IPAddress getAP_NETMASK() { return AP_NETMASK; }
  static std::string getAP_SSID() { return AP_SSID; }
  static std::string getAP_PASSPHRASE() { return AP_PASSPHRASE; }
  static std::string getAP_URL() { return AP_URL; }

  static void setAP_IPADDRESS(IPAddress ip);
  static void setAP_IPADDRESS(String ip);
  static void setAP_GATEWAY(IPAddress gw);
  static void setAP_GATEWAY(String gw);
  static void setAP_NETMASK(IPAddress nm);
  static void setAP_NETMASK(String nm);
  static void setAP_SSID(std::string ssid);
  static void setAP_PASSPHRASE(std::string passphrase);
  static void setAP_URL(std::string url);

  // WebServer
  static uint16_t getWS_PORT() { return WS_PORT; }

  // Serial
  static uint32_t getSE_BAUDRATE() { return SE_BAUDRATE; }

private:
  Config() {};
  virtual ~Config() {};

  // WiFi Connection
  static std::string WC_SSID;
  static std::string WC_PASSPHRASE;
  static std::string WC_HOSTNAME;

  // Access Point
  static IPAddress AP_IPADDRESS;
  static IPAddress AP_GATEWAY;
  static IPAddress AP_NETMASK;
  static std::string AP_SSID;
  static std::string AP_PASSPHRASE;
  static std::string AP_URL;

  // WebServer
  static uint16_t WS_PORT;

  // Serial
  static uint32_t SE_BAUDRATE;
};

// MACROS
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

// CONSTANTS
#define DEBUG 1


// STEPPER SETTINGS
// https://www.allaboutcircuits.com/tools/stepper-motor-calculator/ and
// 42BYGHW811 Wantai stepper motor)
// delay for stepper in microseconds (computer for 32 microsteps, using calculator
#define STEP_DELAY     9000

// reduced in precision (0.05 cm = MIN_PRECISION = 1step = 1.8', 1 cm =
// 20steps = 36', 10 cm = 200steps = 360')
// the difference between the old distance and the new one is then
#define STEP_CM        20.0

// precision of 1 step in cm
#define MIN_PRECISION (1 / STEP_CM)
#define MICROSTEPS 16L

// GONDOLA START POSITIONG
static Coordinate gondolaStart = {0.0, 0.0, 0.0};

// ANCHORS POSITION
#define NUM_ANCHORS 1
static Coordinate anchorPos[NUM_ANCHORS] = {
  {0.0, 0.0, 0.0}
};

// ARDUINO SETTINGS (wemos D1 mini based on esp8266)
// data from
// https://github.com/esp8266/Arduino/blob/master/variants/d1_mini/pins_arduino.h#L49-L61
// schema from
// https://a.pololu-files.com/picture/0J3360.600.png?d94ef1356fab28463db67ff0619afadf

//#define LED_PIN LED_BUILTIN
static uint8_t enable_pin[]  = {0};    // D3
static uint8_t step_pin[]    = {5};    // D1
static uint8_t dir_pin[]     = {4};    // D2

#endif /* _CONFIG_HPP_ */
