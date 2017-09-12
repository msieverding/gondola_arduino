#ifndef _I_WEB_SOCKET_HPP_
#define _I_WEB_SOCKET_HPP_

class IWebSocket
{
public:
  IWebSocket() {}
  virtual ~IWebSocket() {}
  virtual void loop() = 0;

protected:

};

#endif /* _I_WEB_SOCKET_HPP_ */
