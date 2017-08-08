/**
 *
 *   GONDOLA CONTROLLER FOR ESP8266
 *
 *   self-contained code. just program the arduino and send positions via serial
 *
 *
 **/

// Configuration
#include "Config.hpp"
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
IConnection *serial;
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

  serial = new SerialConnection(BAUDRATE, gondola);

  server = WebServer::create(WC_PORT, gondola);
#if WIFI_MODE == WIFI_CONNECTION
  connection = new WiFiConnection(server, WC_SSID, WC_PASSPHRASE);
#elif WIFI_MODE == WIFI_ACCESS_POINT
  connection = new APConnection(server, AP_Name, AP_Passphrase, AP_IPAddress, AP_Gateway, AP_Netmask);
#endif
}

void loop()
{
  wdt_reset();
  if (serial)
    serial->loop();
  if (connection)
    connection->loop();
  gondola->move();
}
