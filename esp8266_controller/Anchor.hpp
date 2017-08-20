#ifndef _ANCHOR_HPP_
#define _ANCHOR_HPP_

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
class Anchor
{
public:
  /**
   * constructor
   * @param id       Anchor ID
   * @param pinSetup pinsetup for stepper driver
   * @param coord    coordinate of anchor
   */
  Anchor(int id, pins_t pinSetup, Coordinate coord);

  /**
   * get the position of the anchor
   * @return coordinate of position
   */
  Coordinate getPosition();

  /**
   * prepare a movement
   * @param newPosition target position
   */
  void prepareToSpool(Coordinate newPosition);
  /**
   * start a step with the hardware
   * @param start_time time, when movement starts
   * @param budget     time budget
   */
  void startStep(long start_time, float budget);
  /**
   * end a step with the hardware
   */
  void endStep();
  /**
   * Returns how many steps are missed
   * @return number of missed stepps
   */
  long missingSteps();

private:
  /**
   * Configure the hardware pins
   */
  void configurePins();
  /**
   * Set the position and initialize the anchor
   * @param coord    coordinate of anchor
   * @param gondola  coordiante of the gondola
   */
  void setPosition(Coordinate coord, Coordinate gondola);

  // membervariables
  uint8_t     m_ID;
  Coordinate  m_AnchorPosition;
  pins_t      m_Pins;
  float       m_SpooledDistance;
  long        m_StepsTodo,
              m_StepsDone,
              m_StepsGoal;
};

#endif /* _ANCHOR_HPP_ */
