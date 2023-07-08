#include "develop_ticker.hpp"

//***********************************/************************************
//                         Constructors                                 //
//***********************************/************************************
TestTicker::TestTicker(PinName clk): _clk(clk)
{
    _clk = 1;
    _n   = 0;
    _spd = 1000;
    //_dtn = 1000000/_spd;
}
//***********************************/************************************
//                                Get Set                               //
//***********************************/************************************
void TestTicker::setSpeed(float speed)
{
    //stop();   
    _spd = (speed<0) ? -speed : speed;  //speed must be unsigned
    if(_spd)_dtn = 1000000/_spd;      //fin min delay (max spd)
    //handler();  
}
//***********************************/************************************
//                             Public Methods                           //
//***********************************/************************************
void TestTicker::run(void)
{
  handler();          //start thread
}

void TestTicker::stop(void)
{   
  remove();           //stop timer
}
//***********************************/************************************
//                          Protected Methods                           //
//***********************************/************************************

void TestTicker::handler(void)
{
  _clk = 0;
  if(!_n)insert(_dtn + us_ticker_read());   //start timer @ first delay
  else insert(event.timestamp+(unsigned int)_dtn);  
  _n = 1;
  _clk = 1;
}