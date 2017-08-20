#ifndef _WEB_SERVER_HPP_
#define _WEB_SERVER_HPP_

#include <ESP8266WebServer.h>
#include "Gondola.hpp"

/**
 * Webserver provides HTTP server with a website to access all functions
 * of gondola through a webserver
 */
class WebServer
{
public:
  /**
   * Create instance of WebServer
   * @param  port    Port to use for the server
   * @param  gondola Gondola to move when a request comes in
   * @return         instnce of WebServer
   */
  static WebServer* create(uint16_t port, Gondola *gondola);

  /**
   * Virtual desctructor
   */
  virtual ~WebServer();

  /**
   * Call this loop in periodically to handle WebServer and DNS requests
   */
  void loop();

private:
  /**
   * private constructor
   * @param  port    Port to use for the server
   * @param  gondola Gondola to move when a request comes in
   */
  WebServer(uint16_t port, Gondola *gondola);
  // initialize WebServer
  bool initialize();
  // HTTP Handler
  static void handleRoot();
  static void handleInitGondola();
  static void handleSetupWiFi();
  static void handleNotFound();
  // Helper functions for building web pages
  static void prepareHeader(std::string &s);
  static void prepareMainPage(std::string &s);
  static void prepareGondolaMovePage(std::string &s, Coordinate &coord, float &speed);
  static void prepareGondolaInitPage(std::string &s, Gondola &gondola);
  static void readOutMoveArgs(ESP8266WebServer &server, Coordinate &coord, float &speed);
  static void prepareWiFiSetupPage(std::string &s);

  // instance
  static WebServer         *s_Instance;
  // Membervariables
  ESP8266WebServer          m_Server;
  Gondola                  *m_Gondola;
};
#endif /* _WEB_SERVER_HPP_ */
