#include <Arduino.h>
#include "Anchor.hpp"
#include "Log.hpp"

Anchor *Anchor::s_Instance = NULL;

Anchor *Anchor::get()
{
  if (s_Instance == NULL)
    s_Instance = new Anchor();

  return s_Instance;
}

Anchor::Anchor()
 : m_AnchorPosition(Config::get()->getGO_ANCHORPOS())
 , m_Pins({0, 5, 4})
 , m_SpooledDistance(0.0f)
 , m_TargetSpooledDistance(m_SpooledDistance)
 , m_StepsTodo(0)
 , m_StepsDone(0)
 , m_Direction(1)
 , m_ReadyCallback()
 , m_MoveStartTime(0)
 , m_TravelTime(0)
 , m_MovementFinished(false)
 , m_Timer()
{
  logDebug("Creating anchor at (%s) with spooledDistance (%s)\n", m_AnchorPosition.toString().c_str(), FTOS(m_SpooledDistance));
  configurePins();
}

Anchor::~Anchor()
{
  logDebug("Deleting Anchor\n");
  s_Instance = NULL;
}

void Anchor::setInitialSpooledDistance(float spooledDistance)
{
  m_SpooledDistance = spooledDistance;
  m_TargetSpooledDistance = m_SpooledDistance;
  logDebug("Incoming initial spooling '%s'.\n", FTOS(m_SpooledDistance));
}

void Anchor::setTargetSpooledDistance(float targetDistance, uint32_t travelTime)
{
  m_TargetSpooledDistance = targetDistance;
  m_TravelTime = travelTime;

  float distanceTodo = m_TargetSpooledDistance - m_SpooledDistance;
  distanceTodo = roundPrecision(distanceTodo, MIN_PRECISION);   // round to a given precision

  logVerbose("============ Anchor Computing stuff ============\n");

  if (distanceTodo == 0)
  {
    logDebug("Nothing to do.\n");
    if (m_ReadyCallback)
      m_ReadyCallback();
    return;
  }

  logVerbose("Spooled: %scm, Delta: %scm\n", floatToString(m_SpooledDistance).c_str(), floatToString(distanceTodo).c_str());

  if (distanceTodo < 0)
  {
    digitalWrite(m_Pins.dir, LOW);
    distanceTodo = abs(distanceTodo);
    m_Direction = -1;
  }
  else
  {
    digitalWrite(m_Pins.dir, HIGH);
    m_Direction = 1;
  }
  // calculate number of steps todo
  m_StepsTodo = distanceTodo * STEP_CM;

  logVerbose("Rounded to: (%scm): %scm, steps: %ld, microsteps: %ld\n", floatToString(MIN_PRECISION).c_str(), floatToString(distanceTodo).c_str(), m_StepsTodo, m_StepsTodo * MICROSTEPS);

  m_StepsTodo *= MICROSTEPS; // we need to account for all microsteps
  m_StepsDone = 0;

  logDebug("Start: %d\n", millis());

  m_MoveStartTime = millis();
  m_Timer.attach_ms(1, move);

  logVerbose("======================================================\n");
}

void Anchor::configurePins()
{
  pinMode(m_Pins.en, OUTPUT);
  pinMode(m_Pins.stp, OUTPUT);
  pinMode(m_Pins.dir, OUTPUT);
  digitalWrite(m_Pins.en, LOW);
}

inline void Anchor::startStep()
{
  digitalWrite(m_Pins.stp, HIGH);   // start stepp trigger
}

inline void Anchor::endStep()
{

  digitalWrite(m_Pins.stp, LOW);    // stop step trigger
}

void Anchor::move()
{
  uint32_t stepsGoal = ceil(((float)((millis() - s_Instance->m_MoveStartTime) * s_Instance->m_StepsTodo)) / s_Instance->m_TravelTime);
  if (stepsGoal > s_Instance->m_StepsTodo)
    stepsGoal = s_Instance->m_StepsTodo;
  for ( ; s_Instance->m_StepsDone <= stepsGoal; s_Instance->m_StepsDone++)
  {
    s_Instance->startStep();
    delayMicroseconds(10); // delay to be sure that the step was done;
    s_Instance->endStep();
    delayMicroseconds(10); // delay to be sure that the step was done;
    s_Instance->m_SpooledDistance += s_Instance->m_Direction * (1.0f / STEP_CM / MICROSTEPS);
    if (s_Instance->m_StepsTodo == s_Instance->m_StepsDone)
      s_Instance->m_MovementFinished = true;
  }
}

void Anchor::loop()
{
  if (m_MovementFinished == true)
  {
    m_MovementFinished = false;
    m_MoveStartTime = 0;
    if (m_ReadyCallback)      // Following shouldn't/can't be done in ISR.
    m_ReadyCallback();
    logDebug("Finish: %d, m_SpooledDistance=%s.\n", millis(), floatToString(m_SpooledDistance).c_str());
  }
}

Coordinate Anchor::getAnchorPos()
{
  return m_AnchorPosition;
}

void Anchor::setAnchorPos(Coordinate anchorPos)
{
  m_AnchorPosition = anchorPos;
}

void Anchor::registerReadyCallback(readyCallback cb)
{
  m_ReadyCallback = cb;
}

float Anchor::roundPrecision(float f, float precision)
{
  return round(f * (1.0f / precision)) / (1.0f / precision);
}
