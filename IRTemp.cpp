/*
 * File:    IRTemp.cpp
 * Version: 1.0
 * Author:  Andy Gelme (@geekscape) & Angus Gratton <angus at freetronics. com>
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
 */

#ifndef IRTEMP_cpp
#define IRTEMP_cpp

#include "IRTemp.h"

static const byte IRTEMP_DATA_SIZE = 5;

byte irTempData[IRTEMP_DATA_SIZE];

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

  long timeout = millis() + IRTEMP_TIMEOUT;

  sensorEnable(true);

  while(1) {
    memset(irTempData, 0, IRTEMP_DATA_SIZE);
    for(uint8_t data_byte = 0; data_byte < IRTEMP_DATA_SIZE; data_byte++) {
      for(int8_t data_bit = 7; data_bit >= 0; data_bit--) {
        // Clock idles high, data changes on falling edge sample on rising edge
        while(digitalRead(irTempPinClock) == HIGH && millis() < timeout) { } // Wait for falling edge
        while(digitalRead(irTempPinClock) == LOW && millis() < timeout) { } // Wait for rising edge to sample
        if(digitalRead(irTempPinData))
          irTempData[data_byte] |= 1<<data_bit;
      }
    }
    if(millis() >= timeout) {
      sensorEnable(false);
      return NAN;
    }

    if (irTempData[0] == dataType && validData(irTempData)) {
      sensorEnable(false);
      float temperature = readTemperature(irTempData);
      if (scale) temperature = convertFarenheit(temperature);
      return temperature;
    }
  }
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

void IRTemp::sensorEnable(
  bool state) {

  irTempSensorEnabled = state;

  digitalWrite(irTempPinAcquire, ! irTempSensorEnabled);
}

bool IRTemp::validData(
  byte data[]) {

  byte checksum = (data[0] + data[1] + data[2]) & 0xff;

  return(data[3] == checksum  &&  data[4] == '\r');
}

#endif
