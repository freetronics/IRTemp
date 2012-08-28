/*
 * File:    IRTemp.cpp
 * Version: 1.0
 * Author:  Andy Gelme (@geekscape)
 * License: GPLv3
 *
 * See www.freetronics.com/irtemp for more information.
 *
 * ToDo
 * ~~~~
 * - Fix-up the half C++ and half C approach, due to the C interrupt handler.
 *
 * - Ensure Clock pin is either pin 2 or pin 3.
 *
 * - Cache previously read Ambient or IR temperature in case get*Temperature()
 *   is called more often than 0.1 seconds (minimum time between updates).
 *
 * - Create non-interrupt version of the library.
 */

#ifndef IRTEMP_cpp
#define IRTEMP_cpp

#include "IRTemp.h"

extern void irTempInterrupt(void);

static const byte IRTEMP_DATA_SIZE = 5;

volatile byte irTempData[IRTEMP_DATA_SIZE];
volatile byte irTempDataBit;
volatile byte irTempDataIndex;

bool irTempSensorEnabled;

byte irTempPinAcquire;
byte irTempPinClock;
byte irTempPinData;

IRTemp::IRTemp(
  byte pinAcquire,
  byte pinClock,
  byte pinData) {

  irTempPinAcquire = pinAcquire;
  irTempPinClock =   pinClock;
  irTempPinData =    pinData;

  pinMode(irTempPinAcquire, OUTPUT);
  pinMode(irTempPinClock,   INPUT);
  pinMode(irTempPinData,    INPUT);

  digitalWrite(irTempPinAcquire, HIGH);
  digitalWrite(irTempPinClock,   HIGH);
  digitalWrite(irTempPinData,    HIGH);

  sensorEnable(false);

  byte interruptNumber = (pinClock == 2)  ?  0  :  1;
  attachInterrupt(interruptNumber, irTempInterrupt, FALLING);
}

// SCALE: Celcius: false, Farenheit: true

float IRTemp::getAmbientTemperature(
  bool scale) {

  return(getTemperature(scale, IRTEMP_DATA_AMBIENT));
}

float IRTemp::getIRTemperature(
  bool scale) {

  return(getTemperature(scale, IRTEMP_DATA_IR));
}

float IRTemp::getTemperature(
  bool scale,
  byte dataType) {

  float temperature = NAN;
  long  timeStart = millis();

  sensorEnable(true);

  while (isnan(temperature)) {
    resetData();

    while (irTempDataIndex < IRTEMP_DATA_SIZE) {
      if ((millis() - timeStart) > IRTEMP_TIMEOUT) {
        sensorEnable(false);
        return(NAN);
      }
    }

    if (irTempData[0] == dataType) {
      if (validData(irTempData)) temperature = readTemperature(irTempData);
    }
  }

  sensorEnable(false);

  if (scale) temperature = convertFarenheit(temperature);

  return(temperature);
}

float IRTemp::convertFarenheit(
  float celcius) {

  return(celcius * 9 / 5 + 32);
}

float IRTemp::readTemperature(
  volatile byte data[]) {

  int msb = data[1] << 8;
  int lsb = data[2];

  return((msb + lsb) / 16.0 - 273.15);
}

void IRTemp::resetData(void) {
  for (byte i = 0;  i < IRTEMP_DATA_SIZE;  i ++) irTempData[i] = 0;

  irTempDataBit = 8;
  irTempDataIndex = 0;
}

void IRTemp::sensorEnable(
  bool state) {

  irTempSensorEnabled = state;

  digitalWrite(irTempPinAcquire, ! irTempSensorEnabled);
}

void irTempInterrupt(void) {
  if (irTempSensorEnabled) {
    if (irTempDataIndex < IRTEMP_DATA_SIZE) {
      irTempDataBit --;
      irTempData[irTempDataIndex] |=
        digitalRead(irTempPinData) << irTempDataBit;

      if (irTempDataBit == 0) {
        irTempDataBit = 8;
        irTempDataIndex ++;
      }
    }
  }
}

bool IRTemp::validData(
  volatile byte data[]) {

  byte checksum = (data[0] + data[1] + data[2]) & 0xff;

  return(data[3] == checksum  &&  data[4] == 0x0d);
}

#endif
