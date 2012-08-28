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

static const long IRTEMP_TIMEOUT = 2000;  // milliseconds

static const long IRTEMP_DATA_AMBIENT = 0x66;
static const long IRTEMP_DATA_IR =      0x4C;
static const long IRTEMP_DATA_JUNK =    0x53;

class IRTemp {
  private:
//  byte _pinAcquire;
//  byte _pinClock;
//  byte _pinData;

//  volatile bool _sensorEnabled;

    float convertFarenheit(float celcius);
    float getTemperature(bool scale, byte dataType);
    float readTemperature(volatile byte data[]);
    void  resetData(void);
    void  sensorEnable(bool state);
    bool  validData(volatile byte data[]);

  public:
    IRTemp(byte pinAcquire, byte pinClock, byte pinData);
    float getAmbientTemperature(bool scale=false);
    float getIRTemperature(bool scale=false);
};
#endif
