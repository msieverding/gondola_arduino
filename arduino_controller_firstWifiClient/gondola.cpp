#include "config.hpp"
#include "Gondola.hpp"
#include <Arduino.h>

Gondola::Gondola(Coordinate newPosition)
 : m_CurrentPosition(newPosition)
 , m_TargetPosition(newPosition)
 , m_Speed(1.0)
 , m_Anchors(NULL)
 , m_NumAnchors(0)
 , m_TravelDistance(0)
 , m_TravelTime(0)
 , m_StartTime(0)
 , m_StepsLeft(0)
{
  if (DEBUG)
  {
    Serial.print("Creating gondola at: ");
    Serial.print(m_CurrentPosition.toString().c_str());
  }
}

Coordinate Gondola::getCurrentPosition()
{
  return m_CurrentPosition;
}

void Gondola::setCurrentPosition(Coordinate &newPosition)
{
  m_CurrentPosition = newPosition;

  if (DEBUG)
  {
    Serial.print("Set gondola position to :");
    Serial.println(m_CurrentPosition.toString().c_str());
  }
}

Coordinate Gondola::getTragetPosition()
{
  return m_TargetPosition;
}

void Gondola::setTargetPosition(Coordinate &targetPosition, float &speed)
{
  m_TargetPosition = targetPosition;
  if (speed == 0.0f)
    m_Speed = 1.0f;
  else
    m_Speed = speed;

  Anchor *anchor = NULL;
  long maxSteps = 0;

  m_TravelDistance = Coordinate::euclideanDistance(m_CurrentPosition, m_TargetPosition);
  if (m_TravelDistance == 0)
  {
    Serial.println("Travel distance = 0. Nothing to do");
    return;
  }
  m_TravelTime = m_TravelDistance / m_Speed;
  for (uint8_t i = 0; i < m_NumAnchors; i++)
  {
    anchor = getAnchor(i);
    if (anchor == NULL)
    {
      Serial.println("getAnchor returns NULL.. return");
      return;
    }
    anchor->prepareToSpool(m_TargetPosition);
    maxSteps = MAX(anchor->missingSteps(), maxSteps);
  }

  if (DEBUG)
  {
    Serial.print("Budget ");
    Serial.print(m_TravelTime);
    Serial.print("s, Minimum ");
    Serial.print(maxSteps / 2000.0); // each microsteps takes 0.5 ms
    Serial.println("s");
    Serial.print("Target position: ");
    Serial.println(m_TargetPosition.toString().c_str());
  }

  m_TravelTime = MAX(m_TravelTime, maxSteps / 2000.0);
  m_StartTime = millis();
  m_TravelTime *= 1000; // convert into ms
}

void Gondola::addAnchor(uint8_t id, pins_t pinSetup)
{
  anchorList_t *ptr = m_Anchors;

  while(ptr != NULL)
  {
    ptr = ptr->next;
  }

  ptr = new anchorList_t;
  if (m_Anchors == NULL)
  {
    m_Anchors = ptr;
  }
  // TODO wieso muss ich da die position übergeben?
  ptr->anchor = new Anchor(id, pinSetup, m_CurrentPosition);
  ptr->next = NULL;
  m_NumAnchors++;
}

Anchor *Gondola::getAnchor(uint8_t id)
{
  if (id >= m_NumAnchors)
  {
    Serial.println("Anchor not initialized... return NULL");
    return NULL;
  }
  else
  {
    anchorList_t *ptr = m_Anchors;
    for (uint8_t i = 0; i < id; i++)
    {
      ptr = ptr->next;
    }
    return ptr->anchor;
  }
}

void Gondola::move(Coordinate &targetPosition, float &speed)
{
  setTargetPosition(targetPosition, speed);
  move();
}

void Gondola::move()
{
  Anchor *anchor;

  if ((millis() < (m_StartTime + m_TravelTime)) || m_StepsLeft != 0)
  {
    Serial.println("Move");
    m_StepsLeft = 0;
    for (uint8_t i = 0; i < m_NumAnchors; i++)
    {
      anchor = getAnchor(i);
      if(anchor == NULL)
        return;
      anchor->startStep(m_StartTime, m_TravelTime);
    }
    // leave the pins up for abit in order to be detected
    delay(STEP_DELAY / 1000);
    for (uint8_t i = 0; i < m_NumAnchors; i++)
    {
      anchor = getAnchor(i);
      if(anchor == NULL)
        return;
      anchor->endStep();
      m_StepsLeft += anchor->missingSteps();
    }
  }
  else
  {
    // TODO eigentlich nach jedem durchlauf position aufshcreiben
    // Serial.println("Movement completed");
    m_CurrentPosition = m_TargetPosition;
  }
}
