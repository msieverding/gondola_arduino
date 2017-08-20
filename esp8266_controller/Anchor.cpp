#include "Anchor.hpp"
#include <Arduino.h>

Anchor::Anchor(int id, pins_t pinSetup, Coordinate coord)
 : m_ID(id)
 , m_AnchorPosition(coord)
 , m_Pins(pinSetup)
 , m_SpooledDistance(0.0f)
 , m_StepsTodo(0)
 , m_StepsDone(0)
 , m_StepsGoal(0)
{
  Serial.print("Creating anchor ");
  Serial.println(id);
  configurePins();
}

Coordinate Anchor::getPosition()
{
  return m_AnchorPosition;
}

long Anchor::missingSteps()
{
  return m_StepsTodo - m_StepsDone;
}

void Anchor::configurePins()
{
  pinMode(m_Pins.en, OUTPUT);
  pinMode(m_Pins.stp, OUTPUT);
  pinMode(m_Pins.dir, OUTPUT);
  digitalWrite(m_Pins.en, LOW);
}

void Anchor::setPosition(Coordinate coord, Coordinate gondola)
{
  m_AnchorPosition = coord;
  m_SpooledDistance = Coordinate::euclideanDistance(gondola, m_AnchorPosition);
}

void Anchor::prepareToSpool(Coordinate newPosition)
{
  float cm_todo, cm_todo_rounded, new_m_SpooledDistance, precision_distance;

  new_m_SpooledDistance = Coordinate::euclideanDistance(m_AnchorPosition, newPosition);

  cm_todo = new_m_SpooledDistance - m_SpooledDistance; // in cm

  if (DEBUG)
  {
    Serial.print("Spooled: ");
    Serial.print(m_SpooledDistance);
    Serial.print("cm, Delta: ");
    Serial.print(cm_todo);
  }

  if (cm_todo < 0)
  {
    // direction_todo = 1;
    digitalWrite(m_Pins.dir, HIGH);
  }
  else
  {
    // direction_todo = -1;
    digitalWrite(m_Pins.dir, LOW);
  }

  m_SpooledDistance += Coordinate::round_precision(cm_todo, MIN_PRECISION); // save new anchor spooled distance
  // m_SpooledDistance += cm_todo; // save new anchor spooled distance
  cm_todo = abs(cm_todo);
  cm_todo_rounded = Coordinate::round_precision(cm_todo, MIN_PRECISION);
  m_StepsTodo = (long)(cm_todo_rounded * STEP_CM); // here we do not make sure the number is not round!!!

  if (DEBUG)
  {
    Serial.print("cm, rounded to (");
    Serial.print(MIN_PRECISION);
    Serial.print("): ");
    Serial.print(cm_todo_rounded);
    Serial.print("cm, steps: ");
    Serial.print(m_StepsTodo); // 200 steps per cm
    Serial.print(", microsteps: ");
    Serial.println(m_StepsTodo * MICROSTEPS);
  }

  m_StepsTodo *= MICROSTEPS; // we need to account for all microsteps
  m_StepsDone = 0;
  m_StepsGoal = 0;
}

void Anchor::startStep(long start_time, float budget)
{
  m_StepsGoal = ceil(((float)((millis() - start_time) * m_StepsTodo)) / budget);
  if (m_StepsGoal > m_StepsTodo)
  {
    m_StepsGoal = m_StepsTodo;
  }
  if ((m_StepsGoal > m_StepsDone) && (m_StepsDone < m_StepsTodo))
  {
    digitalWrite(m_Pins.stp, HIGH);
  }
}

void Anchor::endStep()
{
  if ((m_StepsGoal > m_StepsDone) && (m_StepsDone < m_StepsTodo))
  {
    digitalWrite(m_Pins.stp, LOW); // stop step trigger
    m_StepsDone += 1;
  }
}
