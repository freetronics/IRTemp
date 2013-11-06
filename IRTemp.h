/*
 * File:    IRTemp.h
 * Version: 1.0
 * Author:  Andy Gelme (@geekscape)
 * License: GPLv3
 *
 * See www.freetronics.com/irtemp for more information.
 */

#ifndef IRTEMP_h
#define IRTEMP_h

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

class IRTemp {
 private:
  byte _pinAcquire;
  byte _pinClock;
  byte _pinData;

  float convertFarenheit(float celcius);
  float getTemperature(bool scale, byte dataType);
  float readTemperature(volatile byte data[]);
  void  sensorEnable(bool state);
  bool  validData(byte data[]);

 public:
  IRTemp(byte pinAcquire, byte pinClock, byte pinData);
  float getAmbientTemperature(bool scale=false);
  float getIRTemperature(bool scale=false);
};
#endif
