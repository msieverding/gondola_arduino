#include <Arduino.h>
#include "HardwareAnchor.hpp"
#include "Log.hpp"

HardwareAnchor::HardwareAnchor(pins_t pinSetup, Coordinate coord, float spooledDistance)
 : m_AnchorPosition(coord)
 , m_Pins(pinSetup)
 , m_SpooledDistance(spooledDistance)
 , m_SpoolingSpeed(1.0)
 , m_StepsTodo(0)
{
  Log::logDebug("Creating anchor\n");
  configurePins();
}

HardwareAnchor::~HardwareAnchor()
{
  Log::logDebug("Deleting Anchror\n");
}

void HardwareAnchor::setTargetSpooledDistance(float targetDistance, float speed)
{
  m_SpoolingSpeed = speed;
  m_TargetSpooledDistance = targetDistance;
  calculate();
}

float HardwareAnchor::getTargetSpooledDistance(void)
{
  return m_TargetSpooledDistance;
}

long HardwareAnchor::missingSteps()
{
  return m_StepsTodo;
}

void HardwareAnchor::configurePins()
{
  pinMode(m_Pins.en, OUTPUT);
  pinMode(m_Pins.stp, OUTPUT);
  pinMode(m_Pins.dir, OUTPUT);
  digitalWrite(m_Pins.en, LOW);
}

Coordinate HardwareAnchor::getAnchorPosition()
{
  return m_AnchorPosition;
}

void HardwareAnchor::setAnchorPosition(Coordinate coord)
{
  m_AnchorPosition = coord;
}

void HardwareAnchor::startStep()
{
  // start stepp trigger
  digitalWrite(m_Pins.stp, HIGH);
}

void HardwareAnchor::endStep()
{
  // stop step trigger
  digitalWrite(m_Pins.stp, LOW);
}

void HardwareAnchor::move()
{
  // TODO find possibility to decrease speed
  if (m_StepsTodo == 0)
  {
    Log::logDebug("No steps todo\n");
    return;
  }
  startStep();
  delay(STEP_DELAY / 1000);
  endStep();
  delay(STEP_DELAY / 1000);
  m_StepsTodo--;
}

void HardwareAnchor::calculate()
{
  float distanceTodo = m_TargetSpooledDistance - m_SpooledDistance;

  if (distanceTodo == 0)
    return;

  Log::logDebug("Spooled: ");
  Log::logDebug(m_SpooledDistance);
  Log::logDebug("cm, Delta ");
  Log::logDebug(distanceTodo);

  if (distanceTodo < 0)
  {
    // direction_todo = 1;
    digitalWrite(m_Pins.dir, HIGH);
    // build absolute value
    distanceTodo = abs(distanceTodo);
  }
  else
  {
    // direction_todo = -1;
    digitalWrite(m_Pins.dir, LOW);
  }
  // round to a given precision
  distanceTodo = roundPrecision(distanceTodo, MIN_PRECISION);
  // calculate number of steps todo
  m_StepsTodo = distanceTodo * STEP_CM;

  Log::logDebug("cm, rounded to (");
  Log::logDebug(MIN_PRECISION);
  Log::logDebug("): ");
  Log::logDebug(distanceTodo);
  Log::logDebug("cm, steps: ");
  Log::logDebug(m_StepsTodo); // 200 steps per cm
  Log::logDebug(", microsteps: ");
  Log::logDebug(m_StepsTodo * MICROSTEPS);
  Log::logDebug("\n");

  m_StepsTodo *= MICROSTEPS; // we need to account for all microsteps
}

float HardwareAnchor::roundPrecision(float f, float precision)
{
  return round(f * (1.0f / precision)) / (1.0f / precision);
}
