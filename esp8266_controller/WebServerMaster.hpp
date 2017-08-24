#ifndef _WEB_SERVER_MASTER_HPP_
#define _WEB_SERVER_MASTER_HPP_

#include "WebServer.hpp"
#include "Gondola.hpp"

/**
 * Class for a WebServer which acts as gondolas master
 */
class WebServerMaster : public WebServer
{
public:
  /**
   * Constrcutor
   * @param port Port to use
   */
  WebServerMaster(uint16_t port);

  /**
   * virtual destructor
   */
  virtual ~WebServerMaster();

protected:
  // HTTP handler
  virtual void handleGondolaMove();
  virtual void handleAddWebAnchor();
  virtual void handleReportSpoolingFinished();

  // Helper functions to build web pages
  virtual void prepareHeader(std::string &s);
  void prepareGondolaMovePage(std::string &s);

  //membervariables
  Gondola     *m_Gondola;                       //!< Pointer to gondola
};

#endif /* _WEB_SERVER_MASTER_HPP_ */
