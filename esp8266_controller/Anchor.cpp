#include <Arduino.h>
#include "Anchor.hpp"
#include "Log.hpp"

Anchor *Anchor::s_Instance = NULL;

Anchor *Anchor::get()
{
  if (!s_Instance)
    s_Instance = new Anchor();

  return s_Instance;
}

Anchor::Anchor()
 : m_AnchorPosition(Config::get()->getGO_ANCHORPOS())
 , m_Pins({0, 5, 4})
 , m_CurrentSpooledDistance(0 /*TODO get from EEPROM: Coordinate::euclideanDistance(m_AnchorPosition, Config::get()->getGO_POSITION())*/)
 , m_TargetSpooledDistance(m_CurrentSpooledDistance)
 , m_Speed(1.0f)
 , m_StepsTodo(0)
 , m_Direction(1)
{
  logDebug("Creating anchor\n");
  configurePins();
}

Anchor::~Anchor()
{
  logDebug("Deleting Anchor\n");
  s_Instance = NULL;
}

void Anchor::setTargetSpooledDistance(float targetDistance, float speed)
{
  m_TargetSpooledDistance = targetDistance;
  m_Speed = speed;

  float distanceTodo = m_TargetSpooledDistance - m_CurrentSpooledDistance;

  if (distanceTodo == 0)
    return;

  logDebug("Spooled: %scm, Delta: %scm\n", floatToString(m_CurrentSpooledDistance).c_str(), floatToString(distanceTodo).c_str());

  if (distanceTodo < 0)
  {
    // direction_todo = 1;
    digitalWrite(m_Pins.dir, HIGH);
    // build absolute value
    distanceTodo = abs(distanceTodo);
    m_Direction = -1;
  }
  else
  {
    // direction_todo = -1;
    digitalWrite(m_Pins.dir, LOW);
    m_Direction = 1;
  }
  // round to a given precision
  distanceTodo = roundPrecision(distanceTodo, MIN_PRECISION);
  // calculate number of steps todo
  m_StepsTodo = distanceTodo * STEP_CM;

  logDebug("Rounded to (%scm): %scm, steps: %ld, microsteps: %ld\n", floatToString(MIN_PRECISION).c_str(), floatToString(distanceTodo).c_str(), m_StepsTodo, m_StepsTodo * MICROSTEPS);

  m_StepsTodo *= MICROSTEPS; // we need to account for all microsteps
}

long Anchor::missingSteps()
{
  return m_StepsTodo;
}

void Anchor::configurePins()
{
  pinMode(m_Pins.en, OUTPUT);
  pinMode(m_Pins.stp, OUTPUT);
  pinMode(m_Pins.dir, OUTPUT);
  digitalWrite(m_Pins.en, LOW);
}

void Anchor::startStep()
{
  // start stepp trigger
  digitalWrite(m_Pins.stp, HIGH);
}

void Anchor::endStep()
{
  // stop step trigger
  digitalWrite(m_Pins.stp, LOW);
}

void Anchor::move()
{
  static bool init = false;
  static uint32_t time = 0;
  // TODO find possibility to decrease speed
  if (m_StepsTodo == 0 && init == true)
  {
    init = false;
    logDebug("No more steps todo\n");
    if (m_ReadyCallback)
      m_ReadyCallback();
    return;
  }
  else if (m_StepsTodo == 0)
  {
    return;
  }
  
  if (!init)
  {
    init = true;
    time = millis();
  }

  startStep();
  delayMicroseconds(STEP_DELAY);
  endStep();
  // TODO second delay necessary?
  // TODO check speed
  m_StepsTodo--;
  m_CurrentSpooledDistance += m_Direction * ( 1 / STEP_CM / MICROSTEPS);

  if (millis() >= time)
  {
    time += 1000;
    logDebug("Time %d - CurrentSpooledDistance:'%s'\n", millis(), floatToString(m_CurrentSpooledDistance).c_str());
  }
}

float Anchor::roundPrecision(float f, float precision)
{
  return round(f * (1.0f / precision)) / (1.0f / precision);
}

float Anchor::getCurrentSpooledDistance()
{
  return m_CurrentSpooledDistance;
}

float Anchor::getTargetSpooledDistance()
{
  return m_TargetSpooledDistance;
}

int Anchor::getStepsTodo()
{
  return m_StepsTodo;
}

Coordinate Anchor::getAnchorPos()
{
  return m_AnchorPosition;
}

void Anchor::registerReadyCallback(readyCallback cb)
{
  m_ReadyCallback = cb;
}
