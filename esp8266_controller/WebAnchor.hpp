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

  virtual void setTargetSpooledDistance(float distance, float speed);

  virtual Coordinate getAnchorPosition();

private:
  Coordinate        m_AnchorPosition;
  float             m_SpooledDistance;
  float             m_SpoolingSpeed;
  IPAddress         m_IPAddress;
};

#endif /* _WEB_ANCHOR_HPP_ */
