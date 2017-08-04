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
