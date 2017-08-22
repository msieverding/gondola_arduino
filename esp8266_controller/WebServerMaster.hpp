#ifndef _WEB_SERVER_MASTER_HPP_
#define _WEB_SERVER_MASTER_HPP_

#include "WebServer.hpp"
#include "Gondola.hpp"

class WebServerMaster : public WebServer
{
public:
  WebServerMaster(uint16_t port);
  ~WebServerMaster();
  // virtual void loop();

protected:
  // HTTP handler
  virtual void handleGondolaMove();
  virtual void handleAddWebAnchor();
  virtual void handleReportSpoolingFinished();
  // Helper functions to build web pages
  virtual void prepareHeader(std::string &s);
  void prepareGondolaMovePage(std::string &s);

  Gondola     *m_Gondola;
};

#endif /* _WEB_SERVER_MASTER_HPP_ */
