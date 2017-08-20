#include <Arduino.h>
#include "config.hpp"
#include "Log.hpp"
#include "Gondola.hpp"

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
  Log::logDebug("Creating gondola at: \n");
  Log::logDebug(m_CurrentPosition.toString().c_str());
  Log::logDebug("\n");
}

Coordinate Gondola::getCurrentPosition()
{
  return m_CurrentPosition;
}

void Gondola::setCurrentPosition(Coordinate &newPosition)
{
  m_CurrentPosition = newPosition;
}

void Gondola::setInitialPosition(Coordinate &newPosition)
{
  m_CurrentPosition = newPosition;
  m_TargetPosition = newPosition;
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
    Log::logDebug("Travel distance = 0. Nothing to do.n");
    return;
  }
  m_TravelTime = m_TravelDistance / m_Speed;
  for (uint8_t i = 0; i < m_NumAnchors; i++)
  {
    anchor = getAnchor(i);
    if (anchor == NULL)
    {
      Log::logWarning("getAnchor returns NULL.. return");
      return;
    }
    anchor->prepareToSpool(m_TargetPosition);
    maxSteps = std::max(anchor->missingSteps(), maxSteps);
  }

  Log::logDebug("Budget ");
  Log::logDebug(m_TravelTime);
  Log::logDebug("s, Minimum ");
  Log::logDebug(maxSteps / 2000.0f); // each microsteps takes 0.5 ms
  Log::logDebug("s\n");
  Log::logDebug("Target position: ");
  Log::logDebug(m_TargetPosition.toString().c_str());
  Log::logDebug("\n");

  m_TravelTime = std::max(m_TravelTime, maxSteps / 2000.0f);
  m_StartTime = millis();
  m_TravelTime *= 1000; // convert into ms
}

void Gondola::addAnchor(pins_t pinSetup)
{
  anchorList_t *ptr;
  anchorList_t *entry = new anchorList_t(new Anchor(m_NumAnchors++, pinSetup, m_CurrentPosition));

  if (m_Anchors == NULL)
  {
    m_Anchors = entry;
  }
  else
  {
    ptr = m_Anchors;
    while(ptr->next != NULL)
      ptr = ptr->next;

    ptr->next = entry;
  }
}

Anchor *Gondola::getAnchor(uint8_t id)
{
  if (id >= m_NumAnchors)
  {
    Log::logWarning("Anchor not initialized... return NULL");
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

void Gondola::move()
{
  static uint32_t endTime = 0;
  static bool finished = true;
  Anchor *anchor;

  if (endTime != 0)
  {
    uint32_t missedTime = millis() - endTime;
    m_TotalMissedTime += missedTime;
  }
  if ((millis() < (m_StartTime + m_TravelTime + m_TotalMissedTime)) || m_StepsLeft != 0)
  {
    finished = false;
    m_StepsLeft = 0;
    for (uint8_t i = 0; i < m_NumAnchors; i++)
    {
      anchor = getAnchor(i);
      if(anchor == NULL)
      {
        Log::logWarning("getAnchor delivered NULL!");
        return;
      }
      anchor->startStep(m_StartTime, m_TravelTime);
    }
    // leave the pins up for a bit in order to be detected
    delay(STEP_DELAY / 1000);
    for (uint8_t i = 0; i < m_NumAnchors; i++)
    {
      anchor = getAnchor(i);
      if(anchor == NULL)
      {
        Log::logWarning("getAnchor delivered NULL!");
        return;
      }
      anchor->endStep();
      m_StepsLeft += anchor->missingSteps();
    }
    // TODO calculate new position every loop
  }
  else if (!finished)
  {
    finished = true;
    setCurrentPosition(m_TargetPosition);
    m_TotalMissedTime = 0;
  }
  endTime = millis();
}
