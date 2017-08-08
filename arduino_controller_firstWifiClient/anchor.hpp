#ifndef _ANCHOR_HPP_
#define _ANCHOR_HPP_

#include "coordinate.hpp"
#include "config.hpp"

// TODO struct gehört da nicht hin, problem mit config
typedef struct {
  uint8_t en;
  uint8_t stp;
  uint8_t dir;
} pins_t;
class Anchor
{
public:

  Anchor(int id, pins_t pinSetup, Coordinate coord);

  Coordinate getPosition();



  void prepareToSpool(Coordinate newPosition);
  void startStep(long start_time, float budget);
  void endStep();

private:
  // functions for setup
  void setPins(pins_t pinSetup);
  void setPosition(Coordinate coord, Coordinate _gondola);

  long missingSteps();

  // state
  uint8_t     m_ID;
  Coordinate  m_AnchorPosition;
  pins_t      m_Pins;
  float       m_SpooledDistance;
  long        m_StepsTodo,
              m_StepsDone,
              m_StepsGoal;
};

#endif /* _ANCHOR_HPP_ */
