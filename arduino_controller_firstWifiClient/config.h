// MACROS
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

// CONSTANTS 
#define DEBUG 1

// SERIAL SETTINGS
#define TOKENS ":, \n"
#define BAUDRATE 115200

// ARDUINO SETTINGS
#define LED_PIN 13
int enable_pin[] =  {54, 57, 60, 63};
int step_pin[] =    {55, 58, 61, 64};
int dir_pin[] =     {56, 59, 62, 65};

// STEPPER SETTINGS
#define             STEP_DELAY 90 //delay for stepper in microseconds (computer for 32 microsteps, using calculator https://www.allaboutcircuits.com/tools/stepper-motor-calculator/ and 42BYGHW811 Wantai stepper motor)
#define             STEP_CM 20.0 // the difference between the old distance and the new one is then reduced in precision (0.05 cm = MIN_PRECISION = 1step = 1.8', 1 cm = 20steps = 36', 10 cm = 200steps = 360')
#define             MIN_PRECISION (1/STEP_CM) //precision of 1 step in cm
#define             MICROSTEPS 16L

// ANCHORS POSITION
#define NUM_ANCHORS 4
float x[] =         {0.0, 1.0, 0.0, 0.0};
float y[] =         {0.0, 0.0, 1.0, 0.0};
float z[] =         {0.0, 0.0, 0.0, 1.0};

