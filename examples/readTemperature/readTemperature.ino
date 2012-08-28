/*
 * File:    readTemperature.ino
 * Version: 1.0
 * Author:  Andy Gelme (@geekscape)
 * License: GPLv3
 *
 * For more information see www.freetronics.com/irtemp
 *
 * IRTemp library uses an Arduino interrupt:
 *   If PIN_CLOCK = 2, then Arduino interrupt 0 is used
 *   If PIN_CLOCK = 3, then Arduino interrupt 1 is used
 */

#include "IRTemp.h"

static const byte PIN_DATA    = 2;
static const byte PIN_CLOCK   = 3;  // Must be either pin 2 or pin 3
static const byte PIN_ACQUIRE = 4;

static const bool SCALE=false;  // Celcius: false, Farenheit: true

IRTemp irTemp(PIN_ACQUIRE, PIN_CLOCK, PIN_DATA);

void setup(void) {
  Serial.begin(38400);
  Serial.println("IRTemp example");
  Serial.println("~~~~~~~~~~~~~~");
}

void loop(void) {
  float irTemperature = irTemp.getIRTemperature(SCALE);
  printTemperature("IR", irTemperature);

  float ambientTemperature = irTemp.getAmbientTemperature(SCALE);
  printTemperature("Ambient", ambientTemperature);

  delay(1000);
}

void printTemperature(
  char  *type,
  float  temperature) {

  Serial.print(type);
  Serial.print(" temperature: ");

  if (isnan(temperature)) {
    Serial.println("Failed");
  }
  else {
    Serial.print(temperature);
    Serial.println(SCALE  ?  " F"  :  " C");
  }
}
