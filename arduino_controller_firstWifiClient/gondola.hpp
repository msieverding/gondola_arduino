#ifndef _GONDOLA_HPP_
#define _GONDOLA_HPP_

#include <Arduino.h>
#include "config.hpp"
#include "Coordinate.hpp"
#include "Anchor.hpp"


class Gondola
{
public:
  typedef struct anchorList_s {
    Anchor *anchor;
    anchorList_s *next;
  } anchorList_t;

  Gondola(Coordinate newPosition);
  void addAnchor(uint8_t id, pins_t pinSetup);

  Coordinate getPosition();
  void setPosition(Coordinate newPosition);

private:

  // Member variables
  Coordinate       m_CurrentPosition;
  anchorList_t    *m_Anchors;
};



#endif /* _GONDOLA_HPP_ */
