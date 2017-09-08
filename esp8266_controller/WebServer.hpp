#ifndef _WEB_SERVER_HPP_
#define _WEB_SERVER_HPP_

#include <ESP8266WebServer.h>
#include "MQTTServer.hpp"

/**
 * Webserver provides HTTP server with a website to access all functions
 * of gondola through a webserver
 */
class WebServer
{
public:
  /**
   * private constructor
   * @param  port               Port to use for the server
   * @param  configureServer    constrcutor configures server if true
   */
  WebServer(uint16_t port, bool configureServer = true);

  /**
   * Virtual desctructor
   */
  virtual ~WebServer();

  /**
   * Call this loop periodically to handle WebServer and DNS requests
   */
  virtual void loop();

  /**
   * Set a MqTT Server, wich acts as gondola
   * @param gondola pointer to gondola
   */
  virtual void setGondolaMQTTServer(MQTTServer * gondolaMQTTServer);

protected:
  // HTTP Handler
  virtual void handleRoot();
  virtual void handleSetupWiFi();
  virtual void handleSetupSystem();
  virtual void handleNotFound();

  // Helper functions to build web pages
  virtual void prepareHeader(std::string &s);
  void prepareSetupWiFiPage(std::string &s);
  void prepareSetupSystemPage(std::string &s);
  void prepareGondolaMovePage(std::string &s);

  // Membervariables
  ESP8266WebServer          m_Server;
  MQTTServer               *m_GondolaMQTTServer;
};
#endif /* _WEB_SERVER_HPP_ */
