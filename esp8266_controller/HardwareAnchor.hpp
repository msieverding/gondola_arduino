#ifndef _HARDWARE_ANCHOR_HPP_
#define _HARDWARE_ANCHOR_HPP_

#include "IAnchor.hpp"
#include "Coordinate.hpp"
#include "Config.hpp"

/** Struct for pin setup of ESP8266 */
typedef struct {
  uint8_t en;     // enable pin
  uint8_t stp;    // step pin
  uint8_t dir;    // direction pin
} pins_t;

/**
 * Class for an Anchor of a gondola
 */
class HardwareAnchor : public IAnchor
{
public:
  /**
   * create instance of anchor
   * @param id       Anchor ID
   * @param pinSetup pinsetup for stepper driver
   * @param coord    coordinate of anchor
   */
  static HardwareAnchor *create(pins_t pinSetup, Coordinate anchorPos, float spooledDistance);

  // TODO docu
  static HardwareAnchor *get();

  /**
   * vitual Destructor
   */
  virtual ~HardwareAnchor();

  /**
   * Set the target of the anchor spooling
   * @param targetDistance target distance
   */
  virtual void setTargetSpooledDistance(float targetDistance, float speed);

  /**
   * start a step with the hardware
   */
  void startStep();

  /**
   * end a step with the hardware
   */
  void endStep();

  /**
   * Returns how many steps are missed
   * @return number of missed stepps
   */
  long missingSteps();

  /**
   * Call this function frequently to spool the anchor. Timing is considered within this function
   */
   void move();

private:
  /**
   * constructor
   * @param id       Anchor ID
   * @param pinSetup pinsetup for stepper driver
   * @param coord    coordinate of anchor
   */
  HardwareAnchor(pins_t pinSetup, Coordinate pos, float spooledDistance);
  /**
   * Configure the hardware pins
   */
  void configurePins();

  /**
   * Round precision of float
   */
  float roundPrecision(float f, float precision);

  /**
   * calculate all details for movement
   */
  void calculate(void);

  // instance
  static HardwareAnchor  *s_Instance;
  // membervariables
  Coordinate              m_AnchorPosition;
  pins_t                  m_Pins;
  long                    m_StepsTodo;
  int8_t                  m_Direction;
};

#endif /* _ANCHOR_HPP_ */
