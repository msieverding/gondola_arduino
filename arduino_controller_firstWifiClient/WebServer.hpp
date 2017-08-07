#ifndef _WEB_SERVER_HPP_
#define _WEB_SERVER_HPP_

#include <ESP8266WebServer.h>
#include "gondola.hpp"

class WebServer
{
public:
  static WebServer* create(uint16_t port, Gondola *gondola);
  virtual ~WebServer();

  void loop();

private:
  explicit WebServer(uint16_t port, Gondola *gondola);
  bool initialize();


  static void handleRoot();
  static void handleNotFound();
  static void prepareGondolaMainPage(std::string &s);
  // Variables
  static WebServer         *s_Instance;
  ESP8266WebServer          m_Server;
  Gondola                  *m_Gondola;
};
#endif /* _WEB_SERVER_HPP_ */
