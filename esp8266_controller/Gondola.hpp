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
    anchorList_s(Anchor *_anchor) : anchor(_anchor), next(next) {}
    Anchor *anchor;
    anchorList_s *next;
  } anchorList_t;

  Gondola(Coordinate newPosition);
  void addAnchor(uint8_t id, pins_t pinSetup);
  Anchor *getAnchor(uint8_t id);

  Coordinate getCurrentPosition();
  void setCurrentPosition(Coordinate &newPosition);
  void setInitialPosition(Coordinate &newPosition);
  Coordinate getTragetPosition();
  void setTargetPosition(Coordinate &targetPosition, float &speed);

  void move();
  void move(Coordinate &targetPosition, float &speed);

private:
  
  // Member variables
  Coordinate       m_CurrentPosition;
  Coordinate       m_TargetPosition;
  float            m_Speed;
  anchorList_t    *m_Anchors;
  uint8_t          m_NumAnchors;
  // TODO float massaker beseitigen
  float            m_TravelDistance;
  float            m_TravelTime;
  float            m_StartTime;
  uint32_t         m_TotalMissedTime;
  uint32_t         m_StepsLeft;
};



#endif /* _GONDOLA_HPP_ */
