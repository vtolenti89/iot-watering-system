#include <avr/sleep.h>
#include <avr/wdt.h>

unsigned long previousMillis, currentMillis = 0;
const long interval = 1000;
const unsigned long watchdogInterval = 60*60*6 /8; // called every 8 seconds -> 60 * 60 * 6 = 6 hours;
long watchdogCounter = watchdogInterval;
long int lWaterLevel = 0;
long unsigned motorCounter = 0;
const long motorOnTime = 60 * 20; // twenty minutes on

enum waterLevelStates {
  WL_0 = 0, // water level is empty
  WL_20 = 20, // water level is 20%
  WL_40 = 40, // water level is 40%
  WL_60 = 60, // water level is 60%
  WL_80 = 80, // water level is 80%
  WL_100 = 100 // water level is 100%
};

//pin settings
const int motorPWM = PB1;
const int lWaterLevelPin = A0;
const int sWaterLevelPin = PC1;
const int led = PB5;

//motor and water level settings
enum waterLevelStates wlState;
enum waterLevelStates previouswlState;



void setup()
{
  //Setting up pin inputs and outputs
  DDRD = 0x00;
  DDRB = 0x00;
  DDRB |= (1 << motorPWM);
  DDRB |= (1 << led);

  setupPowerSave();
  watchdogSetup();
}

void loop()
{
  currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    PORTB |= (1 << motorPWM);
    //PORTB |= (1 << led);
    //watchdogCounter = 0;
    motorCounter++;
  }

  if (motorCounter > motorOnTime) {
    PORTB &= ~ (1 << motorPWM);
    //PORTB &= ~ (1 << led);
    motorCounter = 0;
    watchdogCounter = 0;
  }

  if (watchdogCounter < watchdogInterval) {
    // Keep device sleeping
    previousMillis = currentMillis;
    enablePowerSave();
  }

}


void enablePowerSave() {
  cli();
  sleep_enable();
  sleep_bod_disable();
  sei();
  sleep_cpu(); // sleep now
  sleep_disable();
  /*
    MCUCR |= (3 << 5); // set both BOD5 and BODSE at the same time
    MCUCR = (MCUCR & ~ (1 << 5) | (1 << 6)); //then set the BODS bit and clear the BODSE bit at the same time
    asm("SLEEP");
  */
}

void watchdogSetup() {
  cli();
  wdt_reset();
  WDTCSR |= (1 << WDCE) | (1 << WDE); // change enable and WBE - also resets
  WDTCSR = (1 << WDIE) | (1 << WDP3) | (1 << WDP0); // 8s / interrupt, no system reset
  sei();
}

void setupPowerSave () {
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  /*
    // Disable ADC
    ADCSRA &= ~(1 << 7);
    // Configure power down mode and enable it
    SMCR |= (1 << 2);
    SMCR |= (1 << 0);
  */
}

ISR(WDT_vect) { //put in additional code here
  watchdogCounter++;
}


waterLevelStates getWaterLevel() {
  lWaterLevel = analogRead(lWaterLevelPin);

  previouswlState = wlState;
  if (lWaterLevel >= 900) {
    return WL_0;
  } else if (lWaterLevel >= 800) {
    return WL_20;
  } else if (lWaterLevel >= 650) {
    return WL_40;
  } else if (lWaterLevel >= 600) {
    return WL_60;
  } else if (lWaterLevel >= 500) {
    return WL_80;
  } else {
    return WL_100;
  }
}
