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
   * private constructor
   * @param  port           Port to use for the server
   * @param  startServer    constrcutor configures server if true
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

protected:
  // HTTP Handler
  virtual void handleRoot();
  virtual void handleSetup();
  virtual void handleNotFound();

  // Helper functions to build web pages
  virtual void prepareHeader(std::string &s);
  void prepareSetupPage(std::string &s);

  // Membervariables
  ESP8266WebServer          m_Server;
};
#endif /* _WEB_SERVER_HPP_ */
