#include <Arduino.h>
#include "config.hpp"
#include "Log.hpp"
#include "Gondola.hpp"

Gondola *Gondola::s_Instance = NULL;

Gondola* Gondola::get()
{
  if (!s_Instance)
    s_Instance = new Gondola();

  return s_Instance;
}

Gondola::Gondola()
 : m_CurrentPosition(Config::get()->getGO_POSITION())
 , m_TargetPosition(m_CurrentPosition)
 , m_Anchor(Anchor::get())
 , m_Speed(1.0)
 , m_MoveCommand(NULL)
{
  logDebug("Creating gondola at: %s\n", m_CurrentPosition.toString().c_str());
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
  // TODO implement protocoll for moving/ready state of the anchors
  m_CurrentPosition = m_TargetPosition = targetPos;
  m_Speed = speed;
  logDebug("setTargetPosition\n");
  if (m_MoveCommand)
  {
    logDebug("exec m_MoveCommand\n");
    m_MoveCommand(m_TargetPosition, m_Speed);
  }
  m_Anchor->setTargetPosition(m_TargetPosition, m_Speed);
}

void Gondola::registerMoveCommand(moveFunc func)
{
  m_MoveCommand = func;
}
