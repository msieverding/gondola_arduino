#ifndef _I_ANCHOR_HPP_
#define _I_ANCHOR_HPP_

#include "Coordinate.hpp"

/**
 * Abstract interface for an Anchor
 */
class IAnchor
{
public:
  /**
   * Set the target spooled distance of this anchor
   * @param targetDistance distance to reach
   * @param speed          speed to use
   */
  virtual void setTargetSpooledDistance(float targetDistance, float speed) = 0;

  /**
   * Get the target spooled distance
   * @return target spooled distance
   */
  virtual float getTargetSpooledDistance() { return m_TargetSpooledDistance; }

  /**
   * Set the current spooled distance
   * @param distance current spooled distance
   */
  virtual void setCurrentSpooledDistance(float distance) { m_CurrentSpooledDistance = distance; }

  /**
   * Get the current spooled distance
   * @return current spooled distance
   */
  virtual float getCurrentSpooledDistance() { return m_CurrentSpooledDistance; }

  /**
   * get the current speed
   * @return current speed
   */
  virtual float getSpeed() { return m_Speed; }

  /**
   * Get the mounting position of this anchor
   * @return mounting position
   */
  virtual Coordinate getAnchorPosition() { return m_AnchorPosition; }

  /**
   * Virtual destrcutor
   */
  virtual ~IAnchor() {}

  /**
   * The the unique ID of this anchor
   * @param id id to set
   */
  void setID(uint8_t id) { m_ID = id; }

  /**
   * Get the unique ID of this anchor
   * @return unique ID
   */
  uint8_t getID() { return m_ID; }

  /**
   * Get information about the existance of the anchor. Wether the anchor is
   * locally present or a remote anchor
   * @return    true if anchor is not locally present (remote)
   */
  virtual bool isRemoteAnchor() { return false; }

protected:
  /**
   * Constructor of Interface
   * @param currentSpooledDistance current spooled distance
   * @param anchorPos              mounting position of anchor
   */
  IAnchor(float currentSpooledDistance, Coordinate anchorPos)
   : m_ID(255)
   , m_CurrentSpooledDistance(currentSpooledDistance)
   , m_TargetSpooledDistance(currentSpooledDistance)
   , m_Speed(1.0)
   , m_AnchorPosition(anchorPos)
  {}

  uint8_t           m_ID;                         //!< unique ID
  float             m_CurrentSpooledDistance;     //!< Current spooled distance
  float             m_TargetSpooledDistance;      //!< Target spooled distance
  float             m_Speed;                      //!< maximum speed
  Coordinate        m_AnchorPosition;             //!< Mounting position of anchor
};

#endif /* _I_ANCHOR_HPP_ */
