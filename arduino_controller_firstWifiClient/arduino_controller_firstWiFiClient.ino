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
#include "SerialConnection.hpp"
#include "APConnection.hpp"
#include "WebServer.hpp"

// Gondola
Gondola *gondola;
long time_budget;

WebServer *server;
IConnection *connection;

void setup()
{
  Serial.begin(BAUDRATE);
  Serial.print("\n\n");

  gondola = new Gondola(gondolaStart);
  for (int i = 0; i < NUM_ANCHORS; i++)
  {
    gondola->addAnchor(i, {enable_pin[i], step_pin[i], dir_pin[i]});
  }

  server = WebServer::create(WC_PORT, gondola);
#if CONNECTION == CON_WIFI
  connection = new WiFiConnection(server, WC_SSID, WC_PASSPHRASE);
#elif CONNECTION == CON_SERIAL
  connection = new SerialConnection(BAUDRATE, gondola);
#elif CONNECTION == CON_AP
  connection = new APConnection(server, AP_Name, AP_Passphrase, AP_IPAddress, AP_Gateway, AP_Netmask);
#endif
}

void loop()
{
  wdt_reset();
  connection->loop();
  gondola->move();
}
