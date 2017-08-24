// Configuration
#include "Config.hpp"
// Own classes and libraries
#include "Gondola.hpp"
#include "HardwareAnchor.hpp"
#include "WiFiConnection.hpp"
#include "SerialConnection.hpp"
#include "APConnection.hpp"
#include "WebServer.hpp"
#include "WebServerMaster.hpp"
#include "WebServerSlave.hpp"
#include "CommandInterpreter.hpp"
#include "ConnectionMgr.hpp"
#include "Log.hpp"

IConnection *serial;
ConnectionMgr *conMgr;
HardwareAnchor *anchor;

void setup()
{
  Config* config = Config::get();
  config->readFromEEPROM();

  serial = SerialConnection::create(config->getSE_BAUDRATE());

  anchor = HardwareAnchor::create({enable_pin[0], step_pin[0], dir_pin[0]}, anchorPos[0], 0.0f);

  conMgr = ConnectionMgr::get();

  wdt_enable(1000);
  // ESP.wdtEnable(1000);
}

void loop()
{
  wdt_reset();
  // ESP.wdtFeed();
  if (serial)
    serial->loop();
  if (conMgr)
    conMgr->loop();
  if (anchor)
    anchor->move();
}
