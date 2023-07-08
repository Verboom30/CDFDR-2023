#ifndef TEST_TICKER_H
#define TEST_TICKER_H

#include "mbed.h"



class TestTicker : public Ticker
{
  
public:
  /** Create Stepper instance connected to pin clk
  * @param clk pin to connect at clk/step output
  */
  TestTicker(PinName clk);
  void run(void);
  void stop(void);

  /**Set Rotation Speed
  * @param speed [steps/sec]
  */
  void setSpeed(float speed);

protected:

private:
  DigitalOut   _clk;                      //output clock pin 
   float _spd;                            //Speed [step/s]
  unsigned int _n;                        //steps counters
  unsigned int _dtn;                      //current delay
  virtual void handler();
};
#endif