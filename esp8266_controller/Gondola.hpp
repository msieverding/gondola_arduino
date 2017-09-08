#ifndef _GONDOLA_HPP_
#define _GONDOLA_HPP_

#include <Arduino.h>
#include "config.hpp"
#include "Coordinate.hpp"
#include "Anchor.hpp"
#include <functional>

/**
 * Class to store all information and provide functions
 * for gondola with its anchors
 */
class Gondola
{
public:

  /**
   * Type of move function
   */
  typedef std::function<void (Coordinate&, float&)>  moveFunc;

  /**
   * Get the instance of gondola
   * @return instance of gondola
   */
  static Gondola* get();

  /*
    virtual destructor
   */
  virtual ~Gondola();

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
  virtual void setTargetPosition(Coordinate &targetPos, float &speed);

  // TODO Docu
  void registerMoveCommand(moveFunc func);

private:

  /**
  * Constrcutor
  */
  Gondola();

  // instance
  static Gondola         *s_Instance;

  // membervariables
  Coordinate              m_CurrentPosition;    //!< Current position of gondola
  Coordinate              m_TargetPosition;     //!< Target position of gondola
  Anchor                 *m_Anchor;             //!< The one Anchor of this PCB
  float                   m_Speed;              //!< Allowed speed for movement

  // TODO DOCU
  moveFunc                m_MoveCommand;
};



#endif /* _GONDOLA_HPP_ */
