#ifndef _HARDWARE_ANCHOR_HPP_
#define _HARDWARE_ANCHOR_HPP_

#include "Coordinate.hpp"
#include "Config.hpp"
#include <functional>
#include <Ticker.h>

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
   * Type for the callback, that should be executed, when movement is finished
   */
  typedef std::function<void(void)> readyCallback;

  /**
   * Get the instance of Anchor
   */
  static Anchor *get();

  /**
   * vitual Destructor
   */
  virtual ~Anchor();

  /**
   * Set the spooled distance initially
   * @param spooledDistance spooled distance to set
   */
  void setInitialSpooledDistance(float spooledDistance);

  /**
   * Set the new spooling targetDistance.
   * Anchor will start to move with an ISR driven function that
   * toggles the pin for the stepper driver
   * @see move()
   * @param targetDistance Distance to spool to
   * @param travelTime     Time that should be needed
   */
  void setTargetSpooledDistance(float targetDistance, uint32_t travelTime);

  /**
   * start a step with the hardware
   */
  void startStep();

  /**
   * end a step with the hardware
   */
  void endStep();

  /**
   * Get the Mounting position of the anchor
   * @return Coordinate, where the anchor is mounted
   */
  Coordinate getAnchorPos();

   /**
    * Register a callback, that should be executed, when the movement is finished
    * @param cb callback to execute
    */
  void registerReadyCallback(readyCallback cb);

   /**
    * Loop that should be executed with high frequency in the main arduino loop
    *
    * Makes the movemetn of the stepper motor when requestes
    */
  void loop();

  /**
   * Round a float to a given precision
   * @param f             float to round
   * @param precision     precision of rounding
   * @return  rounded f
  */
  static float roundPrecision(float f, float precision);

private:

  /**
   * constructor
   */
  Anchor();

  /**
   * Callback for timer to toggle pins of stepper
   */
  static void move();

  /**
   * Configure the hardware pins
   */
  void configurePins();

  // instance
  static Anchor    *s_Instance;                   //!< Instance of the anchor

  // membervariables
  Coordinate        m_AnchorPosition;             //!< Mounting position
  pins_t            m_Pins;                       //!< pin setup
  float             m_SpooledDistance;            //!< Current spooled distance
  float             m_TargetSpooledDistance;      //!< Target spooled distance
  uint32_t          m_StepsTodo;                  //!< necessary steps to reach the target position
  uint32_t          m_StepsDone;                  //!< steps already done
  int8_t            m_Direction;                  //!< Direction of movement -1 or +1
  readyCallback     m_ReadyCallback;              //!< Callback that should be executed to propagate the finished movement
  uint32_t          m_MoveStartTime;              //!< Time where movement starts
  uint32_t          m_TravelTime;                 //!< Time budget for movement
  bool              m_MovementFinished;           //!< Flag that indicates that the movement is finished. @see move() @see loop()
  Ticker            m_Timer;                      //!< ISR based timer for movement
};

#endif /*  */
