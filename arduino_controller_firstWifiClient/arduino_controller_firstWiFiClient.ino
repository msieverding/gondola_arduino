/**
 *
 *   GONDOLA CONTROLLER FOR ARDUINO
 *
 *   self-contained code. just program the arduino and send positions via serial
 *
 *
 **/

// Configuration
#include "config.hpp"
// ESP Libraries

// Own classes and libraries
#include "Gondola.hpp"
#include "Anchor.hpp"
#include "WiFiConnection.hpp"
#include "WebServer.hpp"

// Gondola
Gondola *gondola;
long time_budget;

WebServer *server;
IConnection *connection;

void setup()
{
  Serial.begin(BAUDRATE);

  gondola = new Gondola(gondolaStart);
  for (int i = 0; i < NUM_ANCHORS; i++)
  {
    gondola->addAnchor(i, {enable_pin[i], step_pin[i], dir_pin[i]});
  }

  connection = new WiFiConnection(server, WC_SSID, WC_PASSPHRASE);
  server = WebServer::create(WC_PORT, gondola);
}

void loop()
{
  wdt_reset();
  server->loop();
  gondola->move();
 //
 //  travel_distance = Coordinate::euclideanDistance(gondola.getCurrentPosition(), newPosition);
 //  travel_time = travel_distance / travel_speed;
 //
 //  if (travel_distance == 0)
 //  {
 //    Serial.println("Travel distance = 0. Nothing to do");
 //    return;
 //  }
 //
 //  // ACTUATE THE STEPPER MOTORS
 //  long max_steps = 0;
 //
 //  for (int a = 0; a < NUM_ANCHORS; a++)
 //  {
 //    anchors[a]->prepareToSpool(newPosition);
 //    max_steps = MAX(anchors[a]->missingSteps(), max_steps);
 //  }
 //
 //  if (DEBUG)
 //  {
 //    Serial.print("Budget ");
 //    Serial.print(travel_time);
 //    Serial.print("s, Minimum ");
 //    Serial.print(max_steps / 2000.0); // each microsteps takes 0.5 ms
 //    Serial.println("s");
 //  }
 //
 //  travel_time = MAX(travel_time, max_steps / 2000.0);
 //
 //  start_time = millis();
 //  travel_time *= 1000; // convert budget time in milliseconds
 //
 //  boolean steps_left = 1;
 //
 //  Serial.println("begin");
 //  // wdt_disable(); // we disable the watchdog, since the next loop can take tens
 //                 // of seconds
 // int i = 0;
 //  while ((millis() < (start_time + travel_time)) || steps_left > 0)
 //  {
 //    yield();
 //    steps_left = 0;
 //    for (int a = 0; a < NUM_ANCHORS; a++)
 //    {
 //      anchors[a]->startStep(start_time, travel_time);
 //    }
 //    yield();
 //    delay(STEP_DELAY / 1000); // leave the pins up for abit in order to be detected
 //    for (int a = 0; a < NUM_ANCHORS; a++)
 //    {
 //      anchors[a]->endStep();
 //      steps_left += anchors[a]->missingSteps();
 //    }
 //    yield();
 //
 //    Serial.print(i++);
 //    Serial.print(" millis= ");
 //    Serial.print(millis());
 //    Serial.print(" steps left= ");
 //    Serial.print(steps_left);
 //    Serial.println("");
 //    yield();
 //  }
 //  Serial.println("end");
 //  // wdt_reset();
 //  // wdt_enable(2000); // re-enable watchdog
 //  Serial.println("end2");
 //
 //  gondola.setPosition(newPosition);
 //  Serial.println("Position reached");
 //
 //  if (DEBUG)
 //  {
 //    Serial.print("Spooling time ");
 //    Serial.print((millis() - start_time) / 1000.0);
 //    Serial.print(", missing steps ");
 //    Serial.println(steps_left);
 //  }

}
