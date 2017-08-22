#ifndef _WEB_ANCHOR_HPP_
#define _WEB_ANCHOR_HPP_

#include <IPAddress.h>
#include "IAnchor.hpp"
#include "Coordinate.hpp"

class WebAnchor : public IAnchor
{
public:
  WebAnchor(Coordinate anchorPos, float spooledDistance, IPAddress ip);
  virtual ~WebAnchor();

  virtual void setTargetSpooledDistance(float targetDistance, float speed);

  IPAddress getIPAddress();

private:
  IPAddress         m_IPAddress;
};

#endif /* _WEB_ANCHOR_HPP_ */
