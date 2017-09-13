#ifndef _HARDWARE_ANCHOR_HPP_
#define _HARDWARE_ANCHOR_HPP_

#include "Coordinate.hpp"
#include "Config.hpp"
#include <functional>

// TODO check Doc
//
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
  // TODO Doc
  typedef std::function<void(void)> readyCallback;
  /**
   * get the instance
   */
  static Anchor *get();

  /**
   * vitual Destructor
   */
  virtual ~Anchor();

  // TODO Doc
  void setTargetSpooledDistance(float targetDistance, float speed);

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

   // TODO Doc
   float getCurrentSpooledDistance();

   // TODO Doc
   float getTargetSpooledDistance();

   // TODO Doc
   int getStepsTodo();

   // TODO Doc
   Coordinate getAnchorPos();

   // TODO Doc
   void registerReadyCallback(readyCallback cb);

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
  readyCallback     m_ReadyCallback; // TODO Doc
};

#endif /*  */
