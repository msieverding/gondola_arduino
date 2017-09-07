#ifndef _GONDOLA_HPP_
#define _GONDOLA_HPP_

#include <Arduino.h>
#include "config.hpp"
#include "Coordinate.hpp"
#include "Anchor.hpp"

/**
 * Class to store all information and provide functions
 * for gondola with its anchors
 */
class IGondola
{
public:

  /**
  * Constrcutor
  * @param startPos start position of gondola
  */
  IGondola(Coordinate startPos);

  /*
    virtual destructor
   */
  virtual ~IGondola();

  /**
  * Get gondolas current positon
  * @return Coordinate with current position
  */
  Coordinate getCurrentPosition();

  /**
   * Sets the current and the target position
   * @param initPos Coordinate to set
   */
  void setInitialPosition(Coordinate &initPos);

  /**
   * Get the target position of gondola
   * @return coordinate as which is target position
   */
  Coordinate getTargetPosition();

  /**
   * Set target position of gondola
   * @param targetPos  target position as coordinate
   * @param speed      speed to use during movement to target
   */
  virtual void setTargetPosition(Coordinate &targetPos, float &speed) = 0;

protected:
  // membervariables
  Anchor                  m_Anchor;             //!< The one Anchor of this PCB
  Coordinate              m_CurrentPosition;    //!< Current position of gondola
  Coordinate              m_TargetPosition;     //!< Target position of gondola
  float                   m_Speed;              //!< Allowed speed for movement
};



#endif /* _GONDOLA_HPP_ */
