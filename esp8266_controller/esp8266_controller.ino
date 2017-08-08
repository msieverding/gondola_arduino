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

void contypeCommand(std::string &s)
{
  String arduinoS(s.c_str());
  Serial.println("-Contype command-");
  uint32_t pos1 = arduinoS.indexOf(' ');
  uint32_t pos2 = arduinoS.indexOf(' ', pos1 + 1);
  Serial.print("pos2 ");
  Serial.println(pos2);
  if (pos1 < 0 || pos2 < 0)
  {
    Serial.println("Invalid command.");
    return;
  }
  String subs = arduinoS.substring(pos1 + 1, pos2);
  Serial.print("first arg: ");
  Serial.println(subs.c_str());
  for (uint8_t i = 0; i < subs.length(); i++)
  {
    Serial.print(subs.c_str()[i], HEX);
    Serial.print(" ");
    Serial.println(subs.c_str()[i]);
  }
  Serial.println("end");
  if (subs.equals("AP"))
  {
    Serial.println("delete old connection");
    delete(connection);
    Serial.print("Address of connection");
    Serial.println((int)connection, HEX);
    connection = new APConnection(server, AP_Name, AP_Passphrase, AP_IPAddress, AP_Gateway, AP_Netmask, AP_URL);
  }
  else if (subs.equals("WIFI"))
  {
    Serial.println("delete old connection");
    delete(connection);
    Serial.print("Address of connection");
    Serial.println((int)connection, HEX);
    connection = new WiFiConnection(server, WC_SSID, WC_PASSPHRASE, WC_NAME);
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
