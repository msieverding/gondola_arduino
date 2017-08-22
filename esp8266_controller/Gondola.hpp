#ifndef _GONDOLA_HPP_
#define _GONDOLA_HPP_

#include <Arduino.h>
#include "config.hpp"
#include "Coordinate.hpp"
#include "IAnchor.hpp"
#include "HardwareAnchor.hpp"

/**
 * Class to store all informations and provide function
 * for the gondola with its anchors
 */
class Gondola
{
public:
  /**
   * List to store all anchors of this gondola
   * @param _anchor [description]
   */
  typedef struct anchorList_s {
    anchorList_s(IAnchor *_anchor, anchorList_s *_next) : anchor(_anchor), next(_next) {}
    IAnchor *anchor;       // Pointer to an anchor
    anchorList_s *next;   // Pointe to the next Anchor
  } anchorList_t;

  /**
  * create the instance
  * @param newPosition start position of gondola
  */
  static Gondola *create(Coordinate startPos);

  /**
   * get the instance
   * ATTENTION: createt must be called before get()
   * @return instance of gondola
   */
  static Gondola *get();
  /*
    virtual destructor
   */
  virtual ~Gondola();
  /**
   * Add an anchor to gondola
   * @param anchor anchor to add
   */
  void addAnchor(IAnchor *anchor);
  /**
   * Get an anchor of gondola
   * @param  id Anchor ID to get
   * @return    pointer of anchor
   */
  IAnchor *getAnchor(uint8_t id);

  /**
  * Get gondolas current positon
  * @return Coordinate with current position
  */
  Coordinate getCurrentPosition();

  /**
   * Set m_CurrentPosition and m_TargetPosition
   * @param newPosition Coordinate to set
   */
  void setInitialPosition(Coordinate &initPos);

  /**
   * Get the target position of gondola
   * @return coordinate as which is target position
   */
  Coordinate getTargetPosition();
  /**
   * Ser m_TargetPosition for gondola
   * @param targetPosition target position as coordinate
   * @param speed          speed to use during movement to target
   */
  void setTargetPosition(Coordinate &targetPos, float &speed);

private:
  /**
  * Constrcutor
  * @param newPosition start position of gondola
  */
  Gondola(Coordinate startPos);

  // instance
  static Gondola  *s_Instance;
  // membervariables
  Coordinate       m_CurrentPosition;
  Coordinate       m_TargetPosition;
  float            m_Speed;
  anchorList_t    *m_Anchors;
  uint8_t          m_NumAnchors;
};



#endif /* _GONDOLA_HPP_ */
