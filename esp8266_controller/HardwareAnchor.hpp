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
   * constructor
   * @param id       Anchor ID
   * @param pinSetup pinsetup for stepper driver
   * @param coord    coordinate of anchor
   */
  HardwareAnchor(pins_t pinSetup, Coordinate pos, float spooledDistance);

  /**
   * vitual Destructor
   */
  ~HardwareAnchor();

  /**
   * get the position of the anchor mouting
   * @return coordinate of position
   */
   Coordinate getAnchorPosition();

  /**
   * Set the position and initialize the anchor
   * @param coord    coordinate of anchor
   * @param gondola  coordiante of the gondola
   */
  void setAnchorPosition(Coordinate coord);

  /**
   * Set the target of the anchor spooling
   * @param targetDistance target distance
   */
  virtual void setTargetSpooledDistance(float targetDistance, float speed);

  /**
   * Get the target of the anchor spooling
   * @return  target distance
   */
  float getTargetSpooledDistance(void);

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
  virtual void move();

private:
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

  // membervariables
  Coordinate  m_AnchorPosition;
  pins_t      m_Pins;
  float       m_SpooledDistance;
  float       m_TargetSpooledDistance;
  float       m_SpoolingSpeed;
  long        m_StepsTodo;
};

#endif /* _ANCHOR_HPP_ */
