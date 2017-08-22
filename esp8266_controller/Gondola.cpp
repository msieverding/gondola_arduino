#include <Arduino.h>
#include "config.hpp"
#include "Log.hpp"
#include "Gondola.hpp"

Gondola *Gondola::s_Instance = NULL;

Gondola* Gondola::create(Coordinate startPos)
{
  if (!s_Instance)
    s_Instance = new Gondola(startPos);

  return s_Instance;
}

Gondola* Gondola::get()
{
  return s_Instance;
}

Gondola::Gondola(Coordinate startPos)
 : m_CurrentPosition(startPos)
 , m_TargetPosition(startPos)
 , m_Speed(1.0)
 , m_Anchors(NULL)
 , m_NumAnchors(0)
{
  Log::logDebug("Creating gondola at: \n");
  Log::logDebug(m_CurrentPosition.toString().c_str());
  Log::logDebug("\n");
}

Gondola::~Gondola()
{
  s_Instance = NULL;
}

Coordinate Gondola::getCurrentPosition()
{
  return m_CurrentPosition;
}

void Gondola::setInitialPosition(Coordinate &initPos)
{
  m_CurrentPosition = initPos;
  m_TargetPosition = initPos;
}

Coordinate Gondola::getTargetPosition()
{
  return m_TargetPosition;
}

void Gondola::setTargetPosition(Coordinate &targetPos, float &speed)
{
  m_TargetPosition = targetPos;
  if (speed == 0.0f)
    m_Speed = 1.0f;
  else
    m_Speed = speed;

  IAnchor *anchor = NULL;
  long maxSteps = 0;

  if (m_CurrentPosition == m_TargetPosition)
  {
    Log::logDebug("Travel distance = 0. Nothing to do\n");
    return;
  }

  for (uint8_t i = 0; i < m_NumAnchors; i++)
  {
    anchor = getAnchor(i);
    if (anchor == NULL)
    {
      Log::logWarning("__FILE__:__LINE__:__FUNCTION__:getAnchor returns NULL.. return\n");
      return;
    }
    Coordinate anchorPos = anchor->getAnchorPosition();
    float newSpooledDistance = Coordinate::euclideanDistance(anchorPos, m_TargetPosition);
    anchor->setTargetSpooledDistance(newSpooledDistance, speed);
  }
}

void Gondola::addAnchor(IAnchor *anchor)
{
  anchorList_t *ptr;
  anchorList_t *entry = new anchorList_t(anchor, NULL);

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
  m_NumAnchors++;
}

IAnchor *Gondola::getAnchor(uint8_t id)
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
