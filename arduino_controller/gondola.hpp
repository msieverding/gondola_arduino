#ifndef _GONDOLA_HPP_
#define _GONDOLA_HPP_

#include <Arduino.h>
#include "config.hpp"
#include "coordinate.hpp"

// =============== GONDOLA CLASS ===============

class Gondola
{
public:
  explicit
  Gondola(Coordinate new_position);

  Coordinate get_position();
  void set_position(Coordinate new_position);

private:
  Coordinate m_currentPosition;
};



#endif /* _GONDOLA_HPP_ */
