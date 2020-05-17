#ifndef Wifi_h
#define Wifi_h
#include <Arduino.h>

class Helper {
  private:
    long timerMax;
    bool timerRefFlag;
    bool timerRefClock;
    long timerRefCounter;
    bool timer10XFlag;
    bool timer10XClock;
    long timer10XCounter;
    bool timer100XFlag;
    bool timer100XClock;
    long timer100XCounter;
    bool timer1000XFlag;
    bool timer1000XClock;
    long timer1000XCounter;
  
  public:
    Helper(long counter = 10);
    ~Helper();
    void updateTimer();
    bool getTimerFlag(int multiplier);
    bool getTimerClock(int multiplier);
 
};

#endif
