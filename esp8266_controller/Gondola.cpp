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
 , m_Speed(1.0)
 , m_HardwareAnchor(Anchor::get())
 , m_AnchorList()
 , m_UnfinishedAnchors(0)
{
  logDebug("Creating gondola at: %s\n", m_CurrentPosition.toString().c_str());
  CommandInterpreter::get()->addCommand("move", std::bind(&Gondola::moveCommand, this, std::placeholders::_1));
  m_HardwareAnchor->registerReadyCallback(std::bind(&Gondola::hardwareAnchorReadyCallback, this));
}

Gondola::~Gondola()
{
  CommandInterpreter::get()->deleteCommand("move");
  s_Instance = NULL;
}

void Gondola::addAnchor(anchorInformation_t &anchorInfo)
{
  m_AnchorList.push_back(anchorInfo);
}

void Gondola::deleteAnchor(uint8_t num)
{
  std::list<anchorInformation_t>::iterator it = m_AnchorList.begin();
  while (it != m_AnchorList.end())
  {
    if (it->id == num)
    {
      m_AnchorList.erase(it++);
    }
    else
    {
      it++;
    }
  }
}

void Gondola::reportAnchorFinished(uint8_t num)
{
  std::list<anchorInformation_t>::iterator it;
  for (it = m_AnchorList.begin(); it != m_AnchorList.end(); it++)
  {
    if (it->id == num)
    {
      it->spooledDistance = it->targetSpooledDistance;
      m_UnfinishedAnchors--;
    }
  }
  checkForReady();
}

Coordinate Gondola::getCurrentPosition()
{
  return m_CurrentPosition;
}

Coordinate Gondola::getTargetPosition()
{
  return m_TargetPosition;
}

void Gondola::setTargetPosition(Coordinate &targetPos, float &speed)
{
  m_TargetPosition = targetPos;
  m_Speed = speed;

  // First: calculate everything
  std::list<anchorInformation_t>::iterator it;
  for (it = m_AnchorList.begin(); it != m_AnchorList.end(); it++)
  {
    it->targetSpooledDistance = Coordinate::euclideanDistance(it->anchorPos, targetPos);
    it->speed = speed; // TODO adapt to old speed calculation
  }
  {
    float targetSpooledDistance = Coordinate::euclideanDistance(m_HardwareAnchor->getAnchorPos(), targetPos);
    float targetSpeed = speed; // TODO adapt to old speed calculation
    m_HardwareAnchor->setTargetSpooledDistance(targetSpooledDistance, targetSpeed);
    m_UnfinishedAnchors++;
  }

  // Second: Make move calls
  for (it = m_AnchorList.begin(); it != m_AnchorList.end(); it++)
  {
    if (it->moveFunc)
    {
      it->moveFunc(*it);
      // TODO maybe moveFunc with boolean return value to identify timeouts
      m_UnfinishedAnchors++;
    }
  }
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

void Gondola::hardwareAnchorReadyCallback()
{
  logDebug("Hardware Anchor finished Moving\n");
  m_UnfinishedAnchors--;
  checkForReady();
}

void Gondola::checkForReady()
{
  if (m_UnfinishedAnchors == 0)
  {
    m_CurrentPosition = m_TargetPosition;
    Config::get()->setGO_POSITION(m_CurrentPosition);
    Config::get()->writeToEEPROM();
  }
}
