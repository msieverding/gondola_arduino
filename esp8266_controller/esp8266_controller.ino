// Configuration
#include "Config.hpp"
// Own classes and libraries
#include "Gondola.hpp"
#include "Anchor.hpp"
#include "WiFiConnection.hpp"
#include "SerialConnection.hpp"
#include "APConnection.hpp"
#include "WebServer.hpp"
#include "CommandInterpreter.hpp"
#include "ConnectionMgr.hpp"
#include "Log.hpp"

IConnection *serial;

void setup()
{
  // Wait one second for hardware and serial console
  delay(1000);
  serial = SerialConnection::create(115200);

  Config::get()->readFromEEPROM();

  // hardware anchor
  Anchor::get();

  ConnectionMgr::get();

  wdt_enable(1000);
  // ESP.wdtEnable(1000);
}

void loop()
{
  wdt_reset();
  // ESP.wdtFeed();

  ConnectionMgr::get()->loop();
  Anchor::get()->loop();
  if (serial)
    serial->loop();
}
