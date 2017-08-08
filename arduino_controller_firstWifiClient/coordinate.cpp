#include "coordinate.hpp"
#include <tgmath.h> // for sqrt

Coordinate::Coordinate()
 : x(0.0)
 , y(0.0)
 , z(0.0)
{
}

Coordinate::Coordinate(const Coordinate &c)
 : x(c.x)
 , y(c.y)
 , z(c.z)
{
}

Coordinate::Coordinate(float _x, float _y, float _z)
 : x(_x)
 , y(_y)
 , z(_z)
{
}

std::string Coordinate::compToString(char comp)
{
  char buf[20];
  switch(comp)
  {
    case 'x': return std::string(dtostrf(x, 4, 2, buf));
    case 'y': return std::string(dtostrf(y, 4, 2, buf));
    case 'z': return std::string(dtostrf(z, 4, 2, buf));
    default: return std::string("");
  }
}

std::string Coordinate::toString()
{
  std::string s;
  char buf[20];

  s.append("x= "),
  dtostrf(x, 4, 2, buf);
  s.append(buf);
  s.append(" y= "),
  dtostrf(y, 4, 2, buf);
  s.append(buf);
  s.append(" z= "),
  dtostrf(z, 4, 2, buf);
  s.append(buf);
  return s;
}

// =============== FUNCTIONS ===============
float Coordinate::euclidean_distance(Coordinate p1, Coordinate p2)
{
  return sqrt(pow2(p1.x - p2.x) + pow2(p1.y - p2.y) + pow2(p1.z - p2.z));
}

float Coordinate::round_precision(float number, float min_precision)
{
  return round(number * (1.0 / min_precision)) / (1.0 / min_precision);
}

float Coordinate::pow2(float x)
{
  return x * x;
}
