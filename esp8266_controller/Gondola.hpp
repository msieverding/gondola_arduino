#ifndef _GONDOLA_HPP_
#define _GONDOLA_HPP_

#include <Arduino.h>
#include "config.hpp"
#include "Coordinate.hpp"
#include "IAnchor.hpp"
#include <functional>
#include <list>

//!< assume we have less than 7 remote anchors, so that the hardwareAnchor is number 7.
#define HW_ANCHOR_ID    7


/**
 * Class to store all information and provide functions
 * for gondola with its anchors
 */
class Gondola
{
public:

  /**
  * Constrcutor
  */
  Gondola();

  /*
    virtual destructor
   */
  virtual ~Gondola();

  /**
   * Set the initial position of the gondola
   * Will use the initFunc of anchorInformation_t
   *
   * @param position position to set
   */
  void setInitialPosition(Coordinate position);

  /**
   * Add an anchor to the list of gondolas anchor
   * @param anchorInfo anchor to add
   */
  void addAnchor(IAnchor *anchor);

  // TODO Doc
  void deleteAnchor(std::list<IAnchor *>::iterator it);

  /**
  * Delete an anchor from the list of gondolas anchor
  * Will call 'reportAnchorFinished' to be sure, that no bloked state will appear
  * @param id   ID of anchor to delete
  */
  void deleteAnchor(uint8_t id);

  /**
   * Report to gondola that an anchor is finished
   * @param id Id of the anchors
   */
  void reportAnchorFinished(uint8_t id);

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

  // TODO Doc
  float getTravelTime();

  /**
   * Set target position of gondola
   * @param targetPos  target position as coordinate
   * @param speed      speed to use during movement to target
   */
  void setTargetPosition(Coordinate &targetPos, float &speed);

  /**
   * Get the list of registered anchors
   * @return  List of registered anchors
   */
  std::list<IAnchor *> getAnchorList(void);

  // TODO Doc
  IAnchor *getAnchor(uint8_t id);
private:

  /**
   * Command for the command line interpreter to move gondola
   * @param  s string to interprete
   * @return   success of command
   */
  bool moveCommand(std::string &s);

  /**
   * Check if all anchors are ready, or if gondola if still busy
   */
  void checkForReady();

  // TODO Doc
  float calculateCorrectedSpooling(IAnchor *anchor, float targetSpooledDistance);

  // membervariables
  Coordinate                      m_CurrentPosition;    //!< Current position of gondola
  Coordinate                      m_TargetPosition;     //!< Target position of gondola
  std::list<IAnchor *>            m_AnchorList;         //!< List of all hardware and remote anchors
  uint8_t                         m_UnfinishedAnchors;  //!< Bitflied to indicate which anchor is ready and which isn't
  float                           m_TravelTime; // TODO Doc
};



#endif /* _GONDOLA_HPP_ */
