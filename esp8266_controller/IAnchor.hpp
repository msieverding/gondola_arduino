#ifndef _I_ANCHOR_HPP_
#define _I_ANCHOR_HPP_

#include "Coordinate.hpp"
#include "Config.hpp"
#include <functional>
#include <Ticker.h>

/**
 * Class for an Anchor of a gondola
 */
class IAnchor
{
public:
  /**
   * Type for the callback, that should be executed, when movement is finished
   */
  typedef std::function<bool(IAnchor *)> callback;

  /**
   * constructor
   * TODO
   */
  IAnchor(uint8_t id, Coordinate anchorPosition, float spooledDistance, float targetSpooledDistance, float ropeOffset);

  /**
   * vitual Destructor
   */
  virtual ~IAnchor();

  // TODO Doc
  void setAnchorPos(Coordinate anchorPos);

  /**
   * @return Coordinate, where the anchor is mounted
   * Get the mounting position of the anchor
   */
  Coordinate getAnchorPos();

  // TODO DOc
  float getSpooledDistance();

  // TODO Doc
  float getTargetSpooledDistance();

  // TODO DOc
  uint32_t getTravelTime();

  /**
   * Set the spooled distance initially
   * @param spooledDistance spooled distance to set
   */
  virtual bool setInitialSpooledDistance(float spooledDistance);

  /**
   * Set the new spooling targetDistance.
   * @param targetDistance Distance to spool to
   * @return               steps needed for movement
   */
  virtual uint32_t setTargetSpooledDistance(float targetDistance) = 0;

  // TODO Doc
  // * @param travelTime     Time that should be needed
  virtual bool startMovement(uint32_t traveltime) = 0;

  /**
   * Register a callback, that should be executed, when an initial spooled distance should be set
   * @param cb callback to execute
   */
  void registerInitCallback(callback cb);

  // TODO Doc
  virtual bool executeInitCallback();

  /**
   * Register a callback, that should be executed, when the movement is finished
   * @param cb callback to execute
   */
  void registerReadyCallback(callback cb);

  // TODO Doc
  virtual bool executeReadyCallback();
  /**
   * Loop that should be executed with high frequency in the main arduino loop
   *
   * Makes the movemetn of the stepper motor when requestes
   */
  virtual void loop() = 0;

  // TODO Doc
  uint8_t getID();

  // TODO Doc
  float getRopeOffset();

  // TODO Doc
  void setRopeOffset(float offset);

  /**
   * Round a float to a given precision
   * @param f             float to round
   * @param precision     precision of rounding
   * @return  rounded f
  */
  static float roundPrecision(float f, float precision);


protected:

  // Membervariables
  uint8_t           m_ID;                         //!< ID of Anchor. Could be used for a protocoll above
  Coordinate        m_AnchorPosition;             //!< Mounting position
  float             m_SpooledDistance;            //!< Current spooled distance
  float             m_TargetSpooledDistance;      //!< Target spooled distance
  callback          m_InitCallback;               //!< Callback to set an initial spooled distance
  callback          m_ReadyCallback;              //!< Callback that should be executed to propagate the finished movement
  uint32_t          m_TravelTime;                 //!< Time budget for movement
  float             m_RopeOffset;                 //!< Spool that is spooled from motor to anchor
};

#endif /* _I_ANCHOR_HPP_ */
