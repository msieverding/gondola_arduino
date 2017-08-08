#include "config.hpp"
#include "gondola.hpp"
#include <Arduino.h>

Gondola::Gondola(Coordinate newPosition)
 : m_CurrentPosition(newPosition)
 , m_Anchors(NULL)
{
  if (DEBUG)
  {
    // TODO use toString
    Serial.print("Creating gondola at (");
    Serial.print(m_CurrentPosition.x);
    Serial.print(",");
    Serial.print(m_CurrentPosition.y);
    Serial.print(",");
    Serial.print(m_CurrentPosition.z);
    Serial.println(")");
  }
}

Coordinate Gondola::getPosition()
{
  return m_CurrentPosition;
}

void Gondola::setPosition(Coordinate newPosition)
{
  m_CurrentPosition.x = newPosition.x;
  m_CurrentPosition.y = newPosition.y;
  m_CurrentPosition.z = newPosition.z;

  if (DEBUG)
  {
    Serial.print("Set gondola position to (");
    Serial.print(m_CurrentPosition.x);
    Serial.print(",");
    Serial.print(m_CurrentPosition.y);
    Serial.print(",");
    Serial.print(m_CurrentPosition.z);
    Serial.println(")");
  }
}

void Gondola::addAnchor(uint8_t id, pins_t pinSetup)
{
  anchorList_t *ptr = m_Anchors;

  while(ptr != NULL)
  {
    ptr = ptr->next;
  }

  ptr = new anchorList_t;
  // TODO wieso muss ich da die position übergeben?
  ptr->anchor = new Anchor(id, pinSetup, m_CurrentPosition);
  ptr->next = NULL;
}
