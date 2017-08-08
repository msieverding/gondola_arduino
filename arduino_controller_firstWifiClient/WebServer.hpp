#ifndef _WEB_SERVER_HPP_
#define _WEB_SERVER_HPP_

#include <ESP8266WebServer.h>
#include "Gondola.hpp"

class WebServer
{
public:
  // create function for singleton
  static WebServer* create(uint16_t port, Gondola *gondola);
  virtual ~WebServer();

  // call this loop function periodically in main loop()
  void loop();

private:
  // private constructor for singleton
  explicit WebServer(uint16_t port, Gondola *gondola);
  // initialize
  bool initialize();

  // HTTP Handler
  static void handleRoot();
  static void handleNotFound();

  // Helper functions for building web pages
  static void prepareGondolaMainPage(std::string &s);
  static void prepareGondolaMovePage(std::string &s, Coordinate &coord, float &speed);
  static void readOutArgs(ESP8266WebServer &server, Coordinate &coord, float &speed);

  // Variables
  static WebServer         *s_Instance;
  ESP8266WebServer          m_Server;
  Gondola                  *m_Gondola;
};
#endif /* _WEB_SERVER_HPP_ */
