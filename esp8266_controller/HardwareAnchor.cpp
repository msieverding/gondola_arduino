#include <Arduino.h>
#include "HardwareAnchor.hpp"
#include "Log.hpp"

HardwareAnchor *HardwareAnchor::s_Instance = NULL;

HardwareAnchor *HardwareAnchor::create(pins_t pinSetup, Coordinate coord, float spooledDistance)
{
  if (!s_Instance)
    s_Instance = new HardwareAnchor(pinSetup, coord, spooledDistance);
  return s_Instance;
}

HardwareAnchor::HardwareAnchor(pins_t pinSetup, Coordinate coord, float spooledDistance)
 : m_AnchorPosition(coord)
 , m_Pins(pinSetup)
 , m_SpooledDistance(spooledDistance)
 , m_SpoolingSpeed(1.0)
 , m_StepsTodo(0)
 , m_Direction(1)
{
  Log::logDebug("Creating anchor\n");
  configurePins();
}

HardwareAnchor *HardwareAnchor::get()
{
  return s_Instance;
}

HardwareAnchor::~HardwareAnchor()
{
  Log::logDebug("Deleting Anchor\n");
  delete(s_Instance);
  s_Instance = NULL;
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
    // Log::logDebug("No steps todo\n");
    return;
  }
  startStep();
  delay(STEP_DELAY / 1000);
  endStep();
  delay(STEP_DELAY / 1000);
  m_StepsTodo--;
  m_SpooledDistance += m_Direction * ( 1 / STEP_CM / MICROSTEPS);
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
