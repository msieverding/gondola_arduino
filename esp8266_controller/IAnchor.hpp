#ifndef _I_ANCHOR_HPP_
#define _I_ANCHOR_HPP_

#include "Coordinate.hpp"

class IAnchor
{
public:
  virtual void setTargetSpooledDistance(float targetDistance, float speed) = 0;
  virtual float getTargetSpooledDistance() { return m_TargetSpooledDistance; }
  virtual void setCurrentSpooledDistance(float distance) { m_CurrentSpooledDistance = distance; }
  virtual float getCurrentSpooledDistance() { return m_CurrentSpooledDistance; }
  virtual float getSpeed() { return m_Speed; }

  virtual Coordinate getAnchorPosition() { return m_AnchorPosition; }

  virtual ~IAnchor() {}
  void setID(uint8_t id) { m_ID = id; }
  uint8_t getID() { return m_ID; }

protected:
  IAnchor(float currentSpooledDistance, Coordinate anchorPos)
   : m_ID(255)
   , m_CurrentSpooledDistance(currentSpooledDistance)
   , m_TargetSpooledDistance(currentSpooledDistance)
   , m_Speed(1.0)
   , m_AnchorPosition(anchorPos)
  {}

  uint8_t           m_ID;
  float             m_CurrentSpooledDistance;
  float             m_TargetSpooledDistance;
  float             m_Speed;
  Coordinate        m_AnchorPosition;
};

#endif /* _I_ANCHOR_HPP_ */
