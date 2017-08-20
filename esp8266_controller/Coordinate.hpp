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
   * Get component of coordinate as string
   * @param  comp 'x', 'y' or 'z' to get a component of the coordinate
   * @return      'comp' as string
   */
  std::string compToString(char comp);
  /**
   * Get whole coordinate as string
   * @return String with coordinate
   */
  std::string toString();

  /**
   * Calculate euclidean distance of two coordinates
   * @param  p1 coordinate 1
   * @param  p2 coordinate 2
   * @return   euclidean distance
   */
  static float euclideanDistance(Coordinate p1, Coordinate p2);
  /**
   * Round precision of a float
   * @param  number        number to round
   * @param  min_precision precision to round to
   * @return               rounded float
   */
  static float round_precision(float number, float min_precision);
  /**
   * x to the power of 2
   * @param  x  x to square
   * @return   square of x
   */
  static float pow2(float x);

  float x;
  float y;
  float z;
};

#endif /* _COORDINATE_HPP_ */
