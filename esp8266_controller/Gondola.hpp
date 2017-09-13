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
  anchorInformation_s(uint8_t _id)
  : id(_id)
  , anchorPos(0.0f, 0.0f, 0.0f)
  , spooledDistance(0.0f)
  , targetSpooledDistance(0.0f)
  , speed(1.0f)
  {}
  uint8_t         id;
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

  // TODO Doc;
  void deleteAnchor(uint8_t num);

  void reportAnchorFinished(uint8_t num);

  /**
  * Get gondolas current positon
  * @return Coordinate with current position
  */
  Coordinate getCurrentPosition();

  /**
  * Get gondolas target positon
  * @return Coordinate with target position
  */
  Coordinate getTargetPosition();

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

  // TODO Doc
  bool moveCommand(std::string &s);

  // TODO Doc
  void hardwareAnchorReadyCallback();

  // TODO Doc
  void checkForReady();

  // instance
  static Gondola                 *s_Instance;

  // membervariables
  Coordinate                      m_CurrentPosition;    //!< Current position of gondola
  Coordinate                      m_TargetPosition;     //!< Target position of gondola
  float                           m_Speed;              //!< Allowed speed for movement
  Anchor                         *m_HardwareAnchor;     //!< the anchor on this board
  // TODO DOCU
  std::list<anchorInformation_t>  m_AnchorList;
  uint8_t                         m_UnfinishedAnchors;
};



#endif /* _GONDOLA_HPP_ */
