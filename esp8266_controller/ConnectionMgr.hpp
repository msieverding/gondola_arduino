#ifndef _CONNECTION_MGR_HPP_
#define _CONNECTION_MGR_HPP

#include "IConnection.hpp"
#include "WebServer.hpp"

class ConnectionMgr
{
public:
  typedef enum {
    CON_ACCESS_POINT,
    CON_WIFI_CONNECTION,
    CON_NONE
  } conType_t;

  static ConnectionMgr *get();
  virtual ~ConnectionMgr();

  bool initConnection(WebServer *server);
  bool changeConnection(conType_t contype);
  void loop();
  void requestChangeConnection(conType_t contype);

private:
  ConnectionMgr();

  static void contypeCommand(std::string &s);

  static ConnectionMgr  *s_Instance;
  conType_t              m_ConType;
  bool                   m_ChangeRequest;
  conType_t              m_ChangeContype;
  IConnection           *m_Connection;
  WebServer             *m_WebServer;

};

#endif
