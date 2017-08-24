#ifndef _GONDOLA_HPP_
#define _GONDOLA_HPP_

#include <Arduino.h>
#include "config.hpp"
#include "Coordinate.hpp"
#include "IAnchor.hpp"
#include "HardwareAnchor.hpp"

/**
 * Class to store all information and provide functions
 * for gondola with its anchors
 */
class Gondola
{
public:
  /**
   * List to store all anchors of this gondola
   */
  typedef struct anchorList_s {
    /**
     * Constructor
     * @param _anchor anchor to store
     * @param _next   next element int list
     */
    anchorList_s(IAnchor *_anchor, anchorList_s *_next) : anchor(_anchor), ready(true), next(_next) {}
    IAnchor *anchor;        //!< Pointer to an anchor
    bool ready;             //!< Ready to spool
    anchorList_s *next;     //!< Pointe to the next Anchor
  } anchorList_t;

  /**
  * create the instance
  * @param startPos start position of gondola
  */
  static Gondola *create(Coordinate startPos);

  /**
   * get the instance
   * If instance wasn't created before, data will be loaded from Config class
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
  void setTargetPosition(Coordinate &targetPos, float &speed);

  /**
   * Get an entry of the anchor list
   * @param  id id of anchor
   * @return    pointer to an element of the list / NULL if invalid id
   */
  anchorList_t *getAnchorListEntry(uint8_t id);

  /**
   * Set or reset the ready bit in an anchor of the anchorlist
   * @param id    id of anchor
   * @param ready anchor is ready(=true) or not (=false)
   */
  void setAnchorReady(uint8_t id, bool ready);

  /**
   * Get the number of registered anchors
   * @return  number of registered anchors
   */
  uint8_t getNumAnchors(void);

private:
  /**
  * Constrcutor
  * @param startPos start position of gondola
  */
  Gondola(Coordinate startPos);

  /**
   * Delete the anchorlist
   */
  void deleteAnchorList(void);

  // instance
  static Gondola         *s_Instance;           //!< Instance of singleton
  // membervariables
  Coordinate              m_CurrentPosition;    //!< Current position of gondola
  Coordinate              m_TargetPosition;     //!< Target position of gondola
  float                   m_Speed;              //!< Allowed speed for movement
  anchorList_t           *m_Anchors;            //!< List of registerd anchors
  uint8_t                 m_NumAnchors;         //!< Number of registerd anchors
};



#endif /* _GONDOLA_HPP_ */
