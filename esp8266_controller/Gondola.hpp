#ifndef _GONDOLA_HPP_
#define _GONDOLA_HPP_

#include <Arduino.h>
#include "config.hpp"
#include "Coordinate.hpp"
#include "Anchor.hpp"
#include <functional>
#include <list>

//!< assume we have less than 7 remote anchors, so that the hardwareAnchor is number 7.
#define HW_ANCHOR_ID    7

/**
 * Struct to save all information about an anchor (hardware or remote)
 * @param _id ID of this anchor.
 */
typedef struct anchorInformation_s {
  anchorInformation_s(uint8_t _id)
  : id(_id)
  , anchorPos(0.0f, 0.0f, 0.0f)
  , spooledDistance(0.0f)
  , targetSpooledDistance(0.0f)
  , travelTime(0)
  , moveFunc()
  {}
  uint8_t         id;                                   //!< ID of the anchor. Could be used for the above protocoll. Actually used for WebSockets
  Coordinate      anchorPos;                            //!< Installation positon of the anchor
  float           spooledDistance;                      //!< current spooled distance of the anchor
  float           targetSpooledDistance;                //!< target spooled distance of the anchor
  uint32_t        travelTime;                           //!< Given travel time
  std::function<bool (anchorInformation_s&)> moveFunc;  //!< Function to move the anchor. Must set by the protocall above
  std::function<bool (anchorInformation_s&)> initFunc;  //!< Tell client initial spooled distance
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
  void addAnchor(anchorInformation_t &anchorInfo);

  /**
   * Delete an anchor from the list of gondolas anchor
   * Will call 'reportAnchorFinished' to be sure, that no bloked state will appear
   * @param num   ID of anchor to delete
   */
  void deleteAnchor(uint8_t num);

  /**
   * Report to gondola that an anchor is finished
   * @param num Id of the anchors
   */
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

  /**
   * Get the list of registered anchors
   * @return  List of registered anchors
   */
  std::list<anchorInformation_t> getAnchorList(void);

private:

  /**
  * Constrcutor
  */
  Gondola();

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

  /**
   * moveFunc for the hardware anchor
   * @param anchorInfo anchorInfo of hardware anchor
   * @return           success
   */
  bool moveHardwareAnchor(anchorInformation_t &anchorInfo);

  /**
   * initFunc for the hardware anchor
   * @param anchorInfo anchorInfo of hardware anchor
   * @return           success
   */
  bool initHardwareAnchor(anchorInformation_t &anchorInfo);

  // instance
  static Gondola                 *s_Instance;           //!< static instance of gondola

  // membervariables
  Coordinate                      m_CurrentPosition;    //!< Current position of gondola
  Coordinate                      m_TargetPosition;     //!< Target position of gondola
  Anchor                         *m_HardwareAnchor;     //!< the anchor on this board
  std::list<anchorInformation_t>  m_AnchorList;         //!< List of all hardware and remote anchors
  uint8_t                         m_UnfinishedAnchors;  //!< Bitflied to indicate which anchor is ready and which isn't
};



#endif /* _GONDOLA_HPP_ */
