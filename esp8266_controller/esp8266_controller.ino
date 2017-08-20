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
#include "CommandInterpreter.hpp"
#include "ConnectionMgr.hpp"

// Gondola
Gondola *gondola;
long time_budget;

WebServer *server;
IConnection *serial;
ConnectionMgr *conMgr;

void setup()
{
  Config* config = Config::get();
  config->readFromEEPROM();

  Serial.begin(config->getSE_BAUDRATE());
  Serial.print("\n\n");

  gondola = new Gondola(gondolaStart);
  for (int i = 0; i < NUM_ANCHORS; i++)
  {
    gondola->addAnchor(i, {enable_pin[i], step_pin[i], dir_pin[i]});
  }

  serial = new SerialConnection(config->getSE_BAUDRATE(), gondola);

  server = WebServer::create(config->getWS_PORT(), gondola);

  conMgr = ConnectionMgr::get();
  conMgr->initConnection(server);
}

void loop()
{
  wdt_reset();
  if (serial)
    serial->loop();
  if (conMgr)
    conMgr->loop();
  gondola->move();
}
