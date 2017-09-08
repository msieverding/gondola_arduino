#ifndef _HARDWARE_ANCHOR_HPP_
#define _HARDWARE_ANCHOR_HPP_

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
class Anchor
{
public:

  /**
   * get the instance
   */
  static Anchor *get();

  /**
   * vitual Destructor
   */
  virtual ~Anchor();

  /**
   * Set the target of the anchor spooling
   * @param gondolaTargetPos target position of gondola
   * @param speed            allowed movement speed
   */
  void setTargetPosition(Coordinate gondolaTargetPos, float speed);

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
   */
  Anchor();

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
  static Anchor    *s_Instance;

  // membervariables
  Coordinate        m_AnchorPosition;             //!< Mounting position of anchor
  pins_t            m_Pins;                       //!< pin setup of this anchor
  float             m_CurrentSpooledDistance;     //!< Current spooled distance
  float             m_TargetSpooledDistance;      //!< Target spooled distance
  float             m_Speed;                      //!< Speed to spool the rope
  long              m_StepsTodo;                  //!< step todo to reach the targget position
  int8_t            m_Direction;                  //!< Direction of movement -1 or +1
};

#endif /*  */
