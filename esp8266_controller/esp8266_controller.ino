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

// Gondola
Gondola *gondola;
long time_budget;

WebServer *server;
IConnection *serial;
IConnection *connection;

void contypeCommand1Arg(std::string &arg)
{
  if (arg.compare("WIFI") == 0)
  {
    Serial.println("delete old connection");
    delete(connection);
    connection = new WiFiConnection(server, WC_SSID, WC_PASSPHRASE, WC_NAME);
  }
  else if (arg.compare("AP") == 0)
  {
    Serial.println("delete old connection");
    delete(connection);
    connection = new APConnection(server, AP_Name, AP_Passphrase, AP_IPAddress, AP_Gateway, AP_Netmask, AP_URL);
  }
  else
  {
    Serial.println("Unsopported.");
  }
}

void contypeCommand(std::string &s)
{
  CommandInterpreter *CI = CommandInterpreter::get();
  String arduinoS(s.c_str());
  Serial.println("-Contype command-");
  std::string arg;

  uint8_t args = CI->getNumArgument(s);
  for (uint8_t i = 0; i < args; i++)
  {
    CI->getArgument(s, arg, i);
    Serial.print("Arg ");
    Serial.print(i);
    Serial.print(" ");
    Serial.println(arg.c_str());
  }

  switch(args)
  {
    case 0:
      Serial.println("Unsupported.");
      break; // TODO make help
    case 1:
      CI->getArgument(s, arg, 0);
      contypeCommand1Arg(arg);
      break;
  }
}



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
  connection = new WiFiConnection(server, WC_SSID, WC_PASSPHRASE, WC_NAME);
#elif WIFI_MODE == WIFI_ACCESS_POINT
  connection = new APConnection(server, AP_Name, AP_Passphrase, AP_IPAddress, AP_Gateway, AP_Netmask, AP_URL);
#endif

  CommandInterpreter::get()->addCommand(std::string("contype"), contypeCommand);
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
