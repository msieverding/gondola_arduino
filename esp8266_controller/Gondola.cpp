#include <Arduino.h>
#include "config.hpp"
#include "Log.hpp"
#include "Gondola.hpp"
#include "CommandInterpreter.hpp"

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
  CommandInterpreter::get()->addCommand("move", std::bind(&Gondola::moveCommand, this, std::placeholders::_1));
}

Gondola::~Gondola()
{
  CommandInterpreter::get()->deleteCommand("move");
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
  logDebug("check m_MoveCommand\n");
  if (m_MoveCommand)
  {
    logDebug("exec m_MoveCommand\n");
    m_MoveCommand(m_TargetPosition, m_Speed);
  }
  m_Anchor->setTargetPosition(m_TargetPosition, m_Speed);
}

void Gondola::registerMoveFunction(moveFunc func)
{
  m_MoveCommand = func;
}

bool Gondola::moveCommand(std::string &s)
{
  CommandInterpreter *CI = CommandInterpreter::get();
  uint8_t args = CI->getNumArgument(s);
  Coordinate newPosition;
  float speed;

  if(args != 4)
  {
    logWarning("Unsupported!\n");
    logWarning("Usage: move x y z s\n");
    logWarning("\tx - float for x coordinate (e.g. 1.0)\n");
    logWarning("\ty - float for y coordinate (e.g. 1.0)\n");
    logWarning("\tz - float for z coordinate (e.g. 1.0)\n");
    logWarning("\ts - float for speed (e.g. 1.0)\n");
    return false;
  }
  std::string arg;
  CI->getArgument(s, arg, 0);
  newPosition.x = atof(arg.c_str());
  CI->getArgument(s, arg, 1);
  newPosition.y = atof(arg.c_str());
  CI->getArgument(s, arg, 2);
  newPosition.z = atof(arg.c_str());
  CI->getArgument(s, arg, 3);
  speed = atof(arg.c_str());

  setTargetPosition(newPosition, speed);
  return true;
}
