#include "helper.h"
#include <Arduino.h>
#include<SoftwareSerial.h>

Helper::Helper(long counter = 10) {
    timerRefCounter = 0;
    timer10XCounter = 0;
    timer100XCounter = 0;
    timer1000XCounter = 0;
    timerMax = counter;
  };

Helper::~Helper() {
  
  };

void Helper::updateTimer () {
  timerRefFlag = false;
  timer10XFlag = false;
  timer100XFlag = false;
  timer1000XFlag = false;
  
  if(millis() - timerRefCounter > timerMax) {
    timerRefCounter = millis();
    timerRefClock = !timerRefClock;
    timerRefFlag = true;
    timer10XCounter++;
    if(timer10XCounter >= 10) {
      timer10XCounter = 0;
      timer10XClock = !timer10XClock;
      timer10XFlag = true;
      timer100XCounter++;

      if(timer100XCounter >= 10) {
        timer100XCounter = 0;
        timer100XClock = !timer100XClock;
        timer100XFlag = true;
        timer1000XCounter++;

        if(timer1000XCounter >= 30) {
          timer1000XCounter = 0;
          timer1000XClock = !timer1000XClock;
          timer1000XFlag = true;
        }
      }
    }
  }
};

bool Helper::getTimerFlag(int multiplier) {
    switch(multiplier) {
      case 1: return timerRefFlag;
      break;
      case 2: return timer10XFlag;
      break;
      case 3: return timer100XFlag;
      break;
      case 4: return timer1000XFlag;
      break;
      default: return timerRefFlag;
      break;
      }
  }

bool Helper::getTimerClock(int multiplier) {
    switch(multiplier) {
      case 1: return timerRefClock;
      break;
      case 2: return timer10XClock;
      break;
      case 3: return timer100XClock;
      break;
      case 4: return timer1000XClock;
      break;
      default: return timerRefClock;
      break;
      }
  }
