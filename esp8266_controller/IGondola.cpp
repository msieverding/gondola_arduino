#include <Arduino.h>
#include "config.hpp"
#include "Log.hpp"
#include "IGondola.hpp"

IGondola::IGondola(Coordinate startPos)
 : m_Anchor({0, 5, 4}, {0.0f, 0.0f, 0.0f}, startPos)
 , m_CurrentPosition(startPos)
 , m_TargetPosition(startPos)
 , m_Speed(1.0)
{
  logDebug("Creating gondola at: %s \n", m_CurrentPosition.toString().c_str());
}

IGondola::~IGondola()
{

}

Coordinate IGondola::getCurrentPosition()
{
  return m_CurrentPosition;
}

void IGondola::setInitialPosition(Coordinate &initPos)
{
  // TODO is that right anymore?
  m_CurrentPosition = initPos;
  m_TargetPosition = initPos;
}

Coordinate IGondola::getTargetPosition()
{
  return m_TargetPosition;
}
