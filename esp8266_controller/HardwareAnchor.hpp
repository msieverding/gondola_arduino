#ifndef _HARDWARE_ANCHOR_HPP_
#define _HARDWARE_ANCHOR_HPP_

#include "IAnchor.hpp"
#include "Coordinate.hpp"
#include "Config.hpp"

/** Struct for pin setup of ESP8266 */
typedef struct {
  uint8_t en;     //!< enable pin
  uint8_t stp;    //!< step pin
  uint8_t dir;    //!< direction pin
} pins_t;

/**
 * Class for an Anchor of a gondola
 */
class HardwareAnchor : public IAnchor
{
public:
  /**
   * create instance of anchor
   * @param pinSetup    pinsetup for stepper driver
   * @param anchorPos   coordinate of anchor
   */
  static HardwareAnchor *create(pins_t pinSetup, Coordinate anchorPos, float spooledDistance);

  /**
   * get pointer to the instance
   * @return pointer to HardwareAnchor instance
   */
  static HardwareAnchor *get();

  /**
   * vitual Destructor
   */
  virtual ~HardwareAnchor();

  /**
   * Set the target of the anchor spooling
   * @param targetDistance  target distance
   * @param speed           allowed movement speed
   */
  virtual void setTargetSpooledDistance(float targetDistance, float speed);

  /**
   * start a step with the hardware
   */
  void startStep();

  /**
   * end a step with the hardware
   */
  void endStep();

  /**
   * Returns how many steps are missed
   * @return number of missed stepps
   */
  long missingSteps();

  /**
   * Call this function frequently to spool the anchor. Timing is considered within this function
   */
   void move();

private:
  /**
   * constructor
   * @param pinSetup        pinsetup for stepper driver
   * @param anchorPos       coordinate of anchor
   * @param spooledDistance currently spooled distance
   */
  HardwareAnchor(pins_t pinSetup, Coordinate anchorPos, float spooledDistance);

  /**
   * Configure the hardware pins
   */
  void configurePins();

  /**
   * Round a float to a given precision
   * @param f             float to round
   * @param precision     precision of rounding
   * @return  rounded f
   */
  float roundPrecision(float f, float precision);

  /**
   * calculate all details for movement
   */
  void calculate(void);

  // instance
  static HardwareAnchor  *s_Instance;           //!< instance of singleton
  // membervariables
  Coordinate              m_AnchorPosition;     //!< Position of the anchor
  pins_t                  m_Pins;               //!< pin setup of this anchor
  long                    m_StepsTodo;          //!< step todo to reach the targget position
  int8_t                  m_Direction;          //!< Direction of movement -1 or +1
};

#endif /* _ANCHOR_HPP_ */
