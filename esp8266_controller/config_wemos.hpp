#ifndef _CONFIG_WEMOS_HPP_
#define _CONFIG_WEMOS_HPP_

#include <string>
#include "Anchor.hpp"
#include "Coordinate.hpp"
#include <IPAddress.h>

// MACROS
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

// CONSTANTS
#define DEBUG 1

// WiFi CONNECTION SETTINGS
static std::string WC_SSID("Virus.exe");
static std::string WC_PASSPHRASE("10542284208956097103");
static std::string WC_NAME("gondola");
static uint16_t WC_PORT = 80;

// AP CONNECTION SETTING
static IPAddress AP_IPAddress(192, 168, 5, 10);
static IPAddress AP_Gateway(192, 168, 5, 1);
static IPAddress AP_Netmask(255, 255, 255, 0);
static std::string AP_Name("GondolaWifi");
static std::string AP_Passphrase("TU_GRAZ_ITI");
static std::string AP_URL("www.gondola.com");

// SERIAL SETTINGS
#define TOKENS ":, \n"
#define BAUDRATE 115200

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


// Connection Setting
#define   WIFI_CONNECTION       1
#define   WIFI_ACCESS_POINT     2
#define   WIFI_MODE             WIFI_CONNECTION

#endif /* _CONFIG_WEMOS_HPP_ */
