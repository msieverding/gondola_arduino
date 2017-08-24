#ifndef _WEB_ANCHOR_HPP_
#define _WEB_ANCHOR_HPP_

#include <IPAddress.h>
#include "IAnchor.hpp"
#include "Coordinate.hpp"

/**
 * Represents a HardwreAnchor of a an other board
 */
class WebAnchor : public IAnchor
{
public:
  /**
   * Constructor
   * @param anchorPos       Mounting position of the anchor
   * @param spooledDistance current spooled distance
   * @param ip              ip of the board
   */
  WebAnchor(Coordinate anchorPos, float spooledDistance, IPAddress ip);

  /**
   * virtual destructor
   */
  virtual ~WebAnchor();

  /**
   * set the target spooled distance of ths WebAnchor
   * @param targetDistance target spooled distance
   * @param speed          speed to use during movement
   */
  virtual void setTargetSpooledDistance(float targetDistance, float speed);

  /**
   * Get the IPAddress of the board
   * @return IPAddress
   */
  IPAddress getIPAddress();

  /**
   * Since this ancor is already remote, return true
   */
  virtual bool isRemoteAnchor(void) { return true; }

private:
  IPAddress         m_IPAddress;        //!< IPAddress of the board with the given WebAnchor
};

#endif /* _WEB_ANCHOR_HPP_ */
