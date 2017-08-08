#ifndef _CONFIG_WEMOS_HPP_
#define _CONFIG_WEMOS_HPP_

#include <string>
#include "anchor.hpp"
#include "coordinate.hpp"

// MACROS
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

// CONSTANTS
#define DEBUG 1

// WiFi Connection SETTINGS
static std::string WC_SSID("Virus.exe");
static std::string WC_PASSPHRASE("10542284208956097103");
static uint16_t WC_PORT = 80;

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

#endif /* _CONFIG_WEMOS_HPP_ */
