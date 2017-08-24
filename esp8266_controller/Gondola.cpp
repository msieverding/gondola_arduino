#include <Arduino.h>
#include "config.hpp"
#include "Log.hpp"
#include "Gondola.hpp"
#include "WebAnchor.hpp"

Gondola *Gondola::s_Instance = NULL;

Gondola* Gondola::create(Coordinate startPos)
{
  if (!s_Instance)
    s_Instance = new Gondola(startPos);

  return s_Instance;
}

Gondola* Gondola::get()
{
  if (!s_Instance)
    s_Instance = new Gondola(Config::get()->getGO_POSITION());

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
  deleteAnchorList();
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

  anchorList_t *anchorListEntry = NULL;
  long maxSteps = 0;

  if (m_CurrentPosition == m_TargetPosition)
  {
    Log::logDebug("Travel distance = 0. Nothing to do\n");
    return;
  }

  for (uint8_t i = 0; i < m_NumAnchors; i++)
  {
    anchorListEntry = getAnchorListEntry(i);
    if (anchorListEntry == NULL)
    {
      Log::logWarning("getAnchor returns NULL... (Line: )");
      Log::logWarning(__LINE__);
      Log::logWarning("id: ");
      Log::logWarning(i);
      return;
    }
    Coordinate anchorPos = anchorListEntry->anchor->getAnchorPosition();
    float newSpooledDistance = Coordinate::euclideanDistance(anchorPos, m_TargetPosition);
    anchorListEntry->ready = false;
    anchorListEntry->anchor->setTargetSpooledDistance(newSpooledDistance, speed);
  }
}

void Gondola::addAnchor(IAnchor *anchor)
{
  anchorList_t *ptr;
  anchorList_t *entry = new anchorList_t(anchor, NULL);
  anchor->setID(m_NumAnchors++);

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

IAnchor *Gondola::getAnchor(uint8_t id)
{
  if (id >= m_NumAnchors)
  {
    Log::logWarning("Anchor not initialized... return NULL\n");
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

Gondola::anchorList_t *Gondola::getAnchorListEntry(uint8_t id)
{
  if (id >= m_NumAnchors)
  {
    Log::logWarning("Anchor not initialized... return NULL\n");
    return NULL;
  }
  else
  {
    anchorList_t *ptr = m_Anchors;
    for (uint8_t i = 0; i < id; i++)
    {
      ptr = ptr->next;
    }
    return ptr;
  }
}

void Gondola::setAnchorReady(uint8_t id, bool ready)
{
  anchorList_t *anchorListEntry = getAnchorListEntry(id);
  anchorListEntry->ready = ready;

  for (uint8_t i = 0; i < m_NumAnchors; i++)
  {
    anchorListEntry = getAnchorListEntry(i);
    if (anchorListEntry->ready == false)
      return;
  }

  // If all anchors are ready, update current po
  m_CurrentPosition = m_TargetPosition;
}

uint8_t Gondola::getNumAnchors(void)
{
  return m_NumAnchors;
}

void Gondola::deleteAnchorList(void)
{
  anchorList_t *ptr = m_Anchors;
  anchorList_t *next;

  while(ptr != NULL)
  {
    next = ptr->next;
    if (ptr->anchor->isRemoteAnchor())
      delete(ptr->anchor);
    delete(ptr);
    ptr = next;
  }
}
