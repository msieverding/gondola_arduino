/**
 *
 *   GONDOLA CONTROLLER FOR ARDUINO
 *
 *   self-contained code. just program the arduino and send positions via serial
 *
 *
 **/

//#include "config.h"
#include "config_wemos.h"
#include "gondola.h"

Anchor *anchors[NUM_ANCHORS];
Gondola *gondola;
long time_budget;

void setup()
{
  Serial.begin(BAUDRATE);

  gondola = new Gondola((coordinate){0.0, 0.0, 0.0});

  for (int a = 0; a < NUM_ANCHORS; a++)
  {
    anchors[a] = new Anchor(a);
    anchors[a]->set_pins(enable_pin[a], step_pin[a], dir_pin[a]);
    anchors[a]->set_position(x[a], y[a], z[a], gondola->get_position());
  }
}

void loop()
{
  if (Serial.available() > 0)
  {
    char command[255];
    coordinate new_position;
    float travel_speed, travel_distance, travel_time;
    long start_time;

    // read a line from serial
    Serial.readBytesUntil('\n', command, 255);
    // parse string on serial (later change it with command interpreter)
    // we expect 4 float: x, y, z, speed in cm/s
    char *cmd = strtok(command, TOKENS);
    // TODO: handle situation where input < 4 floats!!
    new_position.x = atof(cmd);
    cmd = strtok(NULL, TOKENS); // in cm
    new_position.y = atof(cmd);
    cmd = strtok(NULL, TOKENS); // in cm
    new_position.z = atof(cmd);
    cmd = strtok(NULL, TOKENS); // in cm
    travel_speed = atof(cmd);   // in cm/s
    cmd = strtok(NULL, TOKENS);

    travel_distance = euclidean_distance(gondola->get_position(), new_position);
    travel_time = travel_distance / travel_speed;

    if (travel_distance == 0)
    {
      Serial.println("Travel distance = 0. Nothing to do");
      return;
    }

    // ACTUATE THE STEPPER MOTORS
    long max_steps = 0;

    for (int a = 0; a < NUM_ANCHORS; a++)
    {
      anchors[a]->prepare_to_spool(new_position);
      max_steps = MAX(anchors[a]->missing_steps(), max_steps);
    }

    if (DEBUG)
    {
      Serial.print("Budget ");
      Serial.print(travel_time);
      Serial.print("s, Minimum ");
      Serial.print(max_steps / 2000.0); // each microsteps takes 0.5 ms
      Serial.println("s");
    }

    travel_time = MAX(travel_time, max_steps / 2000.0);

    start_time = millis();
    travel_time *= 1000; // convert budget time in milliseconds

    boolean steps_left = 1;

    wdt_disable(); // we disable the watchdog, since the next loop can take tens of seconds
    while ((millis() < (start_time + travel_time)) || steps_left > 0)
    {
      steps_left = 0;
      for (int a = 0; a < NUM_ANCHORS; a++) {
        anchors[a]->start_step(start_time, travel_time);
      }
      delayMicroseconds(STEP_DELAY); // leave the pins up for abit in order to be detected
      for (int a = 0; a < NUM_ANCHORS; a++)
      {
        anchors[a]->end_step();
        steps_left += anchors[a]->missing_steps();
      }
    }
    wdt_enable(1000); // re-enable watchdog

    gondola->set_position(new_position);
    Serial.println("Position reached");

    if (DEBUG)
    {
      Serial.print("Spooling time ");
      Serial.print((millis() - start_time) / 1000.0);
      Serial.print(", missing steps ");
      Serial.println(steps_left);
    }
  }
}
