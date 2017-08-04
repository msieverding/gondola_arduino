#ifndef _ANCHOR_HPP_
#define _ANCHOR_HPP_

#include "coordinate.hpp"
#include "config.hpp"

class Anchor
{

typedef struct {
  int en;
  int stp;
  int dir;
} pins_t;

public:
  explicit
  Anchor(int id);


  Coordinate get_position();


  long missing_steps();

  void set_pins(int _enable, int _step, int _direction);

  void set_position(float _x, float _y, float _z, Coordinate _gondola);

  void prepare_to_spool(Coordinate new_position);

  void start_step(long start_time, float budget);

  void end_step();

private:
    Coordinate m_anchorPosition;
    pins_t m_pins;

    // state
    float m_spooledDistance;
    long m_stepsTodo, m_stepsDone, m_stepsGoal;
};

#endif /* _ANCHOR_HPP_ */
