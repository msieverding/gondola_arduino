#ifndef _I_MQTT_SERVICE_HPP_
#define _I_MQTT_SERVICE_HPP_

class IMQTTService
{
public:
  virtual ~IMQTTService() {}
  virtual void loop() {}
  
protected:
  IMQTTService() {}
};

#endif /* _I_MQTT_SERVICE_HPP_ */
