# 1 "c:\\users\\marvin\\appdata\\local\\temp\\tmpx9uwlg"
#include <Arduino.h>
# 1 "C:/Users/Marvin/Desktop/Projektarbeit/gondola/arduino_controller_firstWifiClient/arduino_controller_firstWiFiClient.ino"
# 21 "C:/Users/Marvin/Desktop/Projektarbeit/gondola/arduino_controller_firstWifiClient/arduino_controller_firstWiFiClient.ino"
#include "config.hpp"







#include "Gondola.hpp"

#include "Anchor.hpp"

#include "WiFiConnection.hpp"

#include "SerialConnection.hpp"

#include "APConnection.hpp"

#include "WebServer.hpp"





Gondola *gondola;

long time_budget;



WebServer *server;

IConnection *connection;
void setup();
void loop();
#line 57 "C:/Users/Marvin/Desktop/Projektarbeit/gondola/arduino_controller_firstWifiClient/arduino_controller_firstWiFiClient.ino"
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