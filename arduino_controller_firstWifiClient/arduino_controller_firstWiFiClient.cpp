

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
#include "gondola.hpp"
#include "anchor.hpp"
#include "WiFiConnection.hpp"
#include "WebServer.hpp"

// Gondola
Anchor *anchors[NUM_ANCHORS];
Gondola *gondola;
long time_budget;

WebServer *server;
WiFiConnection *connection;

void setup()
{
  Serial.begin(BAUDRATE);

  gondola = new Gondola(Coordinate(0.0, 0.0, 0.0));

  for (int a = 0; a < NUM_ANCHORS; a++)
  {
    anchors[a] = new Anchor(a);
    anchors[a]->set_pins(enable_pin[a], step_pin[a], dir_pin[a]);
    anchors[a]->set_position(x[a], y[a], z[a], gondola->get_position());
  }

  connection = new WiFiConnection(server, WC_SSID, WC_PASSPHRASE);
  server = WebServer::create(WC_PORT, gondola);

}

void loop()
{
  wdt_reset();
  server->loop();

  // wdt_reset();
  // // Check if a client has connected
  // WiFiClient client = webConnection.getClient();
  // if (!client)
  // {
  //   return;
  // }

 //  // Wait until the client sends some data
 //  Serial.println("new client");
 //  while (!client.available())
 //  {
 //    yield();
 //    delay(1);
 //  }
 //
 //  // Read the first line of the request
 //  String request = client.readStringUntil('\r');
 //  client.flush();
 //
 //  if (DEBUG)
 //  {
 //    Serial.print("request= ");
 //    Serial.println(request);
 //  }
 //
 //  // Return the response
 //  client.println("HTTP/1.1 200 OK");
 //  client.println("Content-Type: text/html");
 //  client.println(""); //  do not forget this one
 //  client.println("<!DOCTYPE HTML>");
 //  client.println("<html>");
 //
 //
 //  Coordinate Coord = gondola.get_position();
 //  client.println("Gondolas actual position: x=");
 //  client.println(Coord.x);
 //  client.println(" y=");
 //  client.println(Coord.y);
 //  client.println(" z=");
 //  client.println(Coord.z);
 //
 //
 //  client.println("<br><br>");
 //  client.println("New position:");
 //  client.println("<form>");
 //  client.println("<label for=\"x\">X:");
 //  client.println("<input type=\"text\" id=\"x\" name=\"x\">");
 //  client.println("</label>");
 //  client.println("<label for=\"y\">Y:");
 //  client.println("<input type=\"text\" id=\"y\" name=\"y\">");
 //  client.println("</label>");
 //  client.println("<label for=\"z\">Z:");
 //  client.println("<input type=\"text\" id=\"z\" name=\"z\">");
 //  client.println("</label>");
 //  client.println("<br><br>");
 //  client.println("<label for=\"speed\">Speed:");
 //  client.println("<input type=\"text\" id=\"speed\" name=\"speed\">");
 //  client.println("</label>");
 //  client.println("<br><br>");
 //  client.println("<button type=\"submit\">Move!</button>");
 //  client.println("</form>");
 //  client.println("</html>");
 //
 //  delay(1);
 //  Serial.println("Client disconnected");
 //  Serial.println("");
 //
 //  // Match the request
 //  Coordinate new_position;
 //  float travel_speed, travel_distance, travel_time;
 //  long start_time;
 //  String subs;
 //  uint8_t idx;
 //
 //  subs = request.substring(request.indexOf("x=") + 2, request.indexOf("&y"));
 //  new_position.x = subs.toFloat();
 //
 //  subs = request.substring(request.indexOf("y=") + 2, request.indexOf("&z"));
 //  new_position.y = subs.toFloat();
 //
 //  subs = request.substring(request.indexOf("z=") + 2, request.indexOf("&s"));
 //  new_position.z = subs.toFloat();
 //
 //  subs = request.substring(request.indexOf("speed=") + 6);
 //  travel_speed = subs.toFloat();
 //
 //  if (DEBUG)
 //  {
 //    Serial.print("New position is:= ");
 //    Serial.print(new_position.x);
 //    Serial.print(" ");
 //    Serial.print(new_position.y);
 //    Serial.print(" ");
 //    Serial.print(new_position.z);
 //    Serial.print(". Given speed is:= ");
 //    Serial.println(travel_speed);
 //  }
 //
 //  travel_distance = Coordinate::euclidean_distance(gondola.get_position(), new_position);
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
 //    anchors[a]->prepare_to_spool(new_position);
 //    max_steps = MAX(anchors[a]->missing_steps(), max_steps);
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
 //      anchors[a]->start_step(start_time, travel_time);
 //    }
 //    yield();
 //    delay(STEP_DELAY / 1000); // leave the pins up for abit in order to be detected
 //    for (int a = 0; a < NUM_ANCHORS; a++)
 //    {
 //      anchors[a]->end_step();
 //      steps_left += anchors[a]->missing_steps();
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
 //  gondola.set_position(new_position);
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
