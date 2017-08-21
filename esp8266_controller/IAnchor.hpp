#ifndef _I_ANCHOR_
#define _I_ANCHOR_

#include "Coordinate.hpp"

class IAnchor
{
public:
  virtual void setTargetSpooledDistance(float distance, float speed) = 0;
  virtual Coordinate getAnchorPosition() = 0;
  virtual void move() = 0;
  virtual ~IAnchor() {}

protected:
  IAnchor() {}
};

#endif /* _I_ANCHOR_ */
