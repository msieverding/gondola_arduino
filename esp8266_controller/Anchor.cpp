#include <Arduino.h>
#include "Anchor.hpp"
#include "Log.hpp"


Anchor::Anchor(pins_t pinSetup, Coordinate anchorPos, Coordinate gondolaPos)
 : m_AnchorPosition(anchorPos)
 , m_Pins(pinSetup)
 , m_CurrentSpooledDistance(Coordinate::euclideanDistance(m_AnchorPosition, gondolaPos))
 , m_TargetSpooledDistance(m_CurrentSpooledDistance)
 , m_Speed(1.0f)
 , m_StepsTodo(0)
 , m_Direction(1)
{
  Log::logDebug("Creating anchor\n");
  configurePins();
}

Anchor::~Anchor()
{
  Log::logDebug("Deleting Anchor\n");
}

void Anchor::setTargetPosition(Coordinate gondolaTargetPos, float speed)
{
  if (speed == 0.0f)
    m_Speed = 1.0f;
  else
    m_Speed = speed;

  m_TargetSpooledDistance = Coordinate::euclideanDistance(m_AnchorPosition, gondolaTargetPos);

  float distanceTodo = m_TargetSpooledDistance - m_CurrentSpooledDistance;

  if (distanceTodo == 0)
    return;

  Log::logDebug("Spooled: ");
  Log::logDebug(m_CurrentSpooledDistance);
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
  m_CurrentSpooledDistance += m_Direction * ( 1 / STEP_CM / MICROSTEPS);
}

float Anchor::roundPrecision(float f, float precision)
{
  return round(f * (1.0f / precision)) / (1.0f / precision);
}
