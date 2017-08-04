#include "config.hpp"
#include "gondola.hpp"
#include <Arduino.h>

Gondola::Gondola(Coordinate new_position)
{
  m_currentPosition = new_position;
  if (DEBUG)
  {
    Serial.print("Creating gondola at (");
    Serial.print(m_currentPosition.x);
    Serial.print(",");
    Serial.print(m_currentPosition.y);
    Serial.print(",");
    Serial.print(m_currentPosition.z);
    Serial.println(")");
  }
}

Coordinate Gondola::get_position()
{
  return m_currentPosition;
}

void Gondola::set_position(Coordinate new_position)
{
  m_currentPosition.x = new_position.x;
  m_currentPosition.y = new_position.y;
  m_currentPosition.z = new_position.z;

  if (DEBUG)
  {
    Serial.print("Set gondola position to (");
    Serial.print(m_currentPosition.x);
    Serial.print(",");
    Serial.print(m_currentPosition.y);
    Serial.print(",");
    Serial.print(m_currentPosition.z);
    Serial.println(")");
  }
}
