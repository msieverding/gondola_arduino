#ifndef _COORDINATE_HPP_
#define _COORDINATE_HPP_

#include <string>
#include <Arduino.h>

/**
 * Class to store a coordinate
 */
class Coordinate
{
public:
  /**
   * constructor
   */
  Coordinate();
  /**
   * copy constructor
   * @param c coordinate to copy
   */
  Coordinate(const Coordinate &c);
  /**
   * constructor with initial coordinate
   * @param _x x part of coordinate
   * @param _y y part of coordinate
   * @param _z z part of coordinate
   */
  Coordinate(float _x, float _y, float _z);

  /**
   * Get whole coordinate as string
   * @return String with coordinate
   */
  std::string toString();

  /**
   * equal operator
   * @param  c second coordinate
   * @return    equal
   */
  bool operator==(Coordinate const& c);

  /**
   * unequal operator
   * @param  c second coordinate
   * @return    unequal
   */
  bool operator!=(Coordinate const& c);

  /**
   * Calculate euclidean distance of two coordinates
   * @param  p1 coordinate 1
   * @param  p2 coordinate 2
   * @return   euclidean distance
   */
  static float euclideanDistance(Coordinate p1, Coordinate p2);

  /**
   * x to the power of 2
   * @param  x  x to square
   * @return   x^2
   */
  static float pow2(float x);

  float x;      //!< x part of coordinate
  float y;      //!< y part of coordinate
  float z;      //!< z part of coordinate
};

#endif /* _COORDINATE_HPP_ */
