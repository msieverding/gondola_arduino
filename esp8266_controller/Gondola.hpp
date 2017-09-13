#ifndef _GONDOLA_HPP_
#define _GONDOLA_HPP_

#include <Arduino.h>
#include "config.hpp"
#include "Coordinate.hpp"
#include "Anchor.hpp"
#include <functional>
#include <list>


// TODO Doc
typedef struct anchorInformation_s {
  anchorInformation_s()
  : anchorPos(0.0f, 0.0f, 0.0f)
  , spooledDistance(0.0f)
  , targetSpooledDistance(0.0f)
  , speed(1.0f)
  {}
  uint8_t         num;
  Coordinate      anchorPos;
  float           spooledDistance;
  float           targetSpooledDistance;
  float           speed;
  std::function<void (anchorInformation_s&)>  moveFunc;
} anchorInformation_t;


/**
 * Class to store all information and provide functions
 * for gondola with its anchors
 */
class Gondola
{
public:

  /**
   * Get the instance of gondola
   * @return instance of gondola
   */
  static Gondola* get();

  /*
    virtual destructor
   */
  virtual ~Gondola();

  // TODO Doc
  void addAnchor(anchorInformation_t &anchorInfo);

  // TODO deleteAnchorByNum;
  // void deleteAnchor(uint8_t num)

  /**
  * Get gondolas current positon
  * @return Coordinate with current position
  */
  Coordinate getCurrentPosition();

  /**
   * Set target position of gondola
   * @param targetPos  target position as coordinate
   * @param speed      speed to use during movement to target
   */
  void setTargetPosition(Coordinate &targetPos, float &speed);

private:

  /**
  * Constrcutor
  */
  Gondola();

  // instance
  static Gondola                 *s_Instance;

  // membervariables
  Coordinate                      m_CurrentPosition;    //!< Current position of gondola
  Coordinate                      m_TargetPosition;     //!< Target position of gondola
  float                           m_Speed;              //!< Allowed speed for movement
  Anchor                         *m_HardwareAnchor;     //!< the anchor on this board
  // TODO DOCU
  std::list<anchorInformation_t>  m_AnchorList;
};



#endif /* _GONDOLA_HPP_ */
