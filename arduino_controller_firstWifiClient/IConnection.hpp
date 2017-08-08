#ifndef _I_CONNECTION_HPP_
#define _I_CONNECTION_HPP_

class IConnection
{
public:
  virtual void loop() = 0;
protected:
  IConnection() {}
  virtual ~IConnection() {}
};

#endif /* _I_CONNECTION_HPP_ */
