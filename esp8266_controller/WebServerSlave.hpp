#ifndef _WEB_SERVER_SLAVE_HPP_
#define _WEB_SERVER_SLAVE_HPP_

#include "WebServer.hpp"
#include "Gondola.hpp"

/**
 * WebServer which acts like a slave of gondola
 */
class WebServerSlave : public WebServer
{
public:
  /**
   * Constrcutor
   * @param port Port to use for the WebServer
   */
  WebServerSlave(uint16_t port);

  /**
   * virtual destrcutor
   */
  virtual ~WebServerSlave();

  /**
   * Loop to call periodically to handle some function
   * -> Register at master
   * -> Report when spooling is finished
   */
  virtual void loop();

protected:
  // HTTP handler
  virtual void handleSetAnchorTargetPos();

  // Helper functions to build web pages
  virtual void prepareHeader(std::string &s);

  /**
   * Register the HardwareAnchor as a WebAnchor an the Master
   */
  void registerAtMaster();

  /**
   * Report the Master that spooling is finished
   */
  void reportSpoolingFinished();

  // Membervariables
  HardwareAnchor    *m_Anchor;                  //!< pointer to the HardwareAnchor
  bool               m_RegisteredAtMAster;      //!< Indicates if this slave is registered at a master
};

#endif /* _WEB_SERVER_SLAVE_HPP_ */
