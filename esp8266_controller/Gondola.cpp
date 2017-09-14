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
 , m_HardwareAnchor(Anchor::get())
 , m_AnchorList()
 , m_UnfinishedAnchors(0)
{
  logDebug("Creating gondola at: %s\n", m_CurrentPosition.toString().c_str());
  CommandInterpreter::get()->addCommand("move", std::bind(&Gondola::moveCommand, this, std::placeholders::_1));
  m_HardwareAnchor->registerReadyCallback(std::bind(&Gondola::reportAnchorFinished, this, HW_ANCHOR_ID));

  anchorInformation_t hardwareAnchor(HW_ANCHOR_ID);
  hardwareAnchor.anchorPos = m_HardwareAnchor->getAnchorPos();
  hardwareAnchor.moveFunc = std::bind(&Gondola::moveHardWareAnchor, this, std::placeholders::_1);
  hardwareAnchor.initFunc = std::bind(&Gondola::initHardWareAnchor, this, std::placeholders::_1);
  addAnchor(hardwareAnchor);
}

Gondola::~Gondola()
{
  CommandInterpreter::get()->deleteCommand("move");
  s_Instance = NULL;
}

void Gondola::setInitialPosition(Coordinate position)
{
  m_CurrentPosition = position;
  m_TargetPosition = position;
  std::list<anchorInformation_t>::iterator it;
  for (it = m_AnchorList.begin(); it != m_AnchorList.end(); it++)
  {
    it->initFunc(*it);
  }
}

void Gondola::addAnchor(anchorInformation_t &anchorInfo)
{
  anchorInfo.spooledDistance = Coordinate::euclideanDistance(anchorInfo.anchorPos, m_CurrentPosition);
  anchorInfo.targetSpooledDistance = anchorInfo.spooledDistance;
  m_AnchorList.push_back(anchorInfo);
  anchorInfo.initFunc(anchorInfo);
}

void Gondola::deleteAnchor(uint8_t num)
{
  std::list<anchorInformation_t>::iterator it = m_AnchorList.begin();
  while (it != m_AnchorList.end())
  {
    if (it->id == num)
    {
      // Finish spooling to unblock system
      reportAnchorFinished(it->id);
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
  logVerbose("Anchor '%d' finished moving\n", num);
  std::list<anchorInformation_t>::iterator it;
  for (it = m_AnchorList.begin(); it != m_AnchorList.end(); it++)
  {
    if (it->id == num)
    {
      noInterrupts();
      it->spooledDistance = it->targetSpooledDistance;
      m_UnfinishedAnchors &= ~(1 << it->id);
      interrupts();
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
  if (speed == 0.0f)
    speed = 1.0f;

  float travelDistance = Coordinate::euclideanDistance(m_CurrentPosition, m_TargetPosition);

  logVerbose("============= Gondola Computing all Anchors ==========\n");

  float travelTime = travelDistance / speed;
  logVerbose("TravelDistance: %s, TravelTime: %s\n", FTOS(travelDistance), FTOS(travelTime));
  uint32_t max_steps = 0;

  // prepare to spool
  std::list<anchorInformation_t>::iterator it;
  for (it = m_AnchorList.begin(); it != m_AnchorList.end(); it++)
  {
    float cmTodo, cmTodoRounded;
    uint32_t stepsTodo;

    it->targetSpooledDistance = Coordinate::euclideanDistance(it->anchorPos, targetPos);
    cmTodo = it->targetSpooledDistance - it->spooledDistance;        //in cm

    logVerbose("AnchorNum: %d\n", (int16_t)it->id);
    logVerbose("Spooled: %scm, Delta: %scm\n", floatToString(it->spooledDistance).c_str(), floatToString(cmTodo).c_str());

    cmTodoRounded = abs(Anchor::roundPrecision(cmTodo, MIN_PRECISION));
    stepsTodo = (cmTodoRounded * STEP_CM);

    logVerbose("Rounded to: (%scm): %scm, steps: %ld, microsteps: %ld\n", floatToString(MIN_PRECISION).c_str(), floatToString(cmTodoRounded).c_str(), stepsTodo, stepsTodo * MICROSTEPS);
    stepsTodo *= MICROSTEPS;
    max_steps = std::max(max_steps, stepsTodo);
  }

  logVerbose("Budget: %ss, Minimum %ss\n", floatToString(travelTime).c_str(), floatToString(max_steps / 1000.f).c_str());

  logVerbose("==============================================\n");

  travelTime = std::max(travelTime, max_steps / 1000.0f);     // Fastest step is 1 ms(due to timer in Anchor.cpp) so give more time to all motors to have a smooth momvement
  travelTime *= 1000;

  for (it = m_AnchorList.begin(); it != m_AnchorList.end(); it++)
  {
    it->travelTime = travelTime;
    if (it->moveFunc)
    {
      // TODO maybe moveFunc with boolean return value to identify timeouts
      m_UnfinishedAnchors |= (1 << it->id);
      it->moveFunc(*it);
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

void Gondola::checkForReady()
{
  if (m_UnfinishedAnchors == 0)
  {
    m_CurrentPosition = m_TargetPosition;
    Config::get()->setGO_POSITION(m_CurrentPosition);
    Config::get()->writeGOToEEPROM(true);
  }
}

void Gondola::moveHardWareAnchor(anchorInformation_t &anchorInfo)
{
  m_HardwareAnchor->setTargetSpooledDistance(anchorInfo.targetSpooledDistance, anchorInfo.travelTime);
}

void Gondola::initHardWareAnchor(anchorInformation_t &anchorInfo)
{
  m_HardwareAnchor->setInitialSpooledDistance(anchorInfo.spooledDistance);
}
