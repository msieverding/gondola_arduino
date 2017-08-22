#ifndef _WEB_SERVER_SLAVE_HPP_
#define _WEB_SERVER_SLAVE_HPP_

#include "WebServer.hpp"
#include "Gondola.hpp"

class WebServerSlave : public WebServer
{
public:
  WebServerSlave(uint16_t port);
  ~WebServerSlave();
  virtual void loop();

protected:
  // HTTP handler
  virtual void handleSetAnchorTargetPos();

  // Helper functions to build web pages
  virtual void prepareHeader(std::string &s);

  void registerAtMaster();

  HardwareAnchor    *m_Anchor;
  bool               m_RegisteredAtMAster;
};

#endif /* _WEB_SERVER_SLAVE_HPP_ */
