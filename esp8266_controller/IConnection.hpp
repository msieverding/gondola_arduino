#ifndef _I_CONNECTION_HPP_
#define _I_CONNECTION_HPP_

/**
 * Interface for connections.
 * Used for WiFiConnection, APConnection and SerialConnection for polymorphie
 */
class IConnection
{
public:
  virtual void loop() = 0;
  virtual ~IConnection() {}
protected:
  IConnection() {}
};

#endif /* _I_CONNECTION_HPP_ */
