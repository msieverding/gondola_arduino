#ifndef _I_WEB_SOCKET_HPP_
#define _I_WEB_SOCKET_HPP_

// TODO Doc
typedef enum webSocketCommand_e : byte {
  WSO_C_REGISTER,
  WSO_C_REPORT,
  WSO_S_MOVE
} webSocketCommand_t;

class IWebSocket
{
public:
  IWebSocket() {}
  virtual ~IWebSocket() {}
  virtual void loop() = 0;

protected:

};

#endif /* _I_WEB_SOCKET_HPP_ */
