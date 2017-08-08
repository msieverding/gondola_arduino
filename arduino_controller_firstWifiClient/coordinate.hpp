#ifndef _COORDINATE_HPP_
#define _COORDINATE_HPP_

#include <string>
#include <Arduino.h>

class Coordinate
{
public:
  Coordinate();
  Coordinate(const Coordinate &c);
  Coordinate(float _x, float _y, float _z);

  std::string compToString(char comp);
  std::string toString();

  static float euclidean_distance(Coordinate p1, Coordinate p2);
  static float round_precision(float number, float min_precision);
  static float pow2(float x);

  float x;
  float y;
  float z;
};

#endif /* _COORDINATE_HPP_ */
