#include "anchor.hpp"
#include <Arduino.h>

Anchor::Anchor(int id)
{
  Serial.print("Creating anchor ");
  Serial.println(id);
}

Coordinate Anchor::get_position()
{
  return m_anchorPosition;
}

long Anchor::missing_steps()
{
  return m_stepsTodo - m_stepsDone;
}

void Anchor::set_pins(int _enable, int _step, int _direction)
{
  m_pins = {_enable, _step, _direction};
  pinMode(m_pins.en, OUTPUT);
  pinMode(m_pins.stp, OUTPUT);
  pinMode(m_pins.dir, OUTPUT);
  digitalWrite(m_pins.en, LOW);
}

void Anchor::set_position(float _x, float _y, float _z, Coordinate _gondola)
{
  m_anchorPosition = {_x, _y, _z};
  m_spooledDistance = Coordinate::euclidean_distance(_gondola, m_anchorPosition);
}

void Anchor::prepare_to_spool(Coordinate new_position)
{
  float cm_todo, cm_todo_rounded, new_m_spooledDistance, precision_distance;

  new_m_spooledDistance = Coordinate::euclidean_distance(m_anchorPosition, new_position);

  cm_todo = new_m_spooledDistance - m_spooledDistance; // in cm

  if (DEBUG)
  {
    Serial.print("Spooled: ");
    Serial.print(m_spooledDistance);
    Serial.print("cm, Delta: ");
    Serial.print(cm_todo);
  }

  if (cm_todo < 0)
  {
    // direction_todo = 1;
    digitalWrite(m_pins.dir, HIGH);
  }
  else
  {
    // direction_todo = -1;
    digitalWrite(m_pins.dir, LOW);
  }

  m_spooledDistance += Coordinate::round_precision(cm_todo, MIN_PRECISION); // save new anchor spooled distance
  // m_spooledDistance += cm_todo; // save new anchor spooled distance
  cm_todo = abs(cm_todo);
  cm_todo_rounded = Coordinate::round_precision(cm_todo, MIN_PRECISION);
  m_stepsTodo = (long)(cm_todo_rounded * STEP_CM); // here we do not make sure the number is not round!!!

  if (DEBUG)
  {
    Serial.print("cm, rounded to (");
    Serial.print(MIN_PRECISION);
    Serial.print("): ");
    Serial.print(cm_todo_rounded);
    Serial.print("cm, steps: ");
    Serial.print(m_stepsTodo); // 200 steps per cm
    Serial.print(", microsteps: ");
    Serial.println(m_stepsTodo * MICROSTEPS);
  }

  m_stepsTodo *= MICROSTEPS; // we need to account for all microsteps
  m_stepsDone = 0;
  m_stepsGoal = 0;
}

void Anchor::start_step(long start_time, float budget)
{
  m_stepsGoal = ceil(((float)((millis() - start_time) * m_stepsTodo)) / budget);
  if (m_stepsGoal > m_stepsTodo)
  {
    m_stepsGoal = m_stepsTodo;
  }
  if ((m_stepsGoal > m_stepsDone) && (m_stepsDone < m_stepsTodo))
  {
    digitalWrite(m_pins.stp, HIGH);
  }
}

void Anchor::end_step()
{
  if ((m_stepsGoal > m_stepsDone) && (m_stepsDone < m_stepsTodo))
  {
    digitalWrite(m_pins.stp, LOW); // stop step trigger
    m_stepsDone += 1;
  }
}
