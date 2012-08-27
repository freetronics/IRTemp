IRTemp
======
Infrared non-contact thermometer library for the Arduino.

This library supports the ZyTemp TN9 IR Temperature module.

The TN9 measures the ...

- Infrared temperature (conical zone in front of the sensor)
- Ambient temperature

This library needs to be initialized with the Arduino pin configurations ...

      #include "IRTemp.h"
      static const byte PIN_DATA    = 2;
      static const byte PIN_CLOCK   = 3;  // Must be either pin 2 or pin 3
      static const byte PIN_ACQUIRE = 4;
      IRTemp irTemp(PIN_ACQUIRE, PIN_CLOCK, PIN_DATA);

Once the library has been initialized, the two different temperature values
can be obtained as follows ...

      float irTemperature = irTemp.getIRTemperature();
      float amTemperature = irTemp.getAmbientTemperature();

By default, these functions return the temperature value as a floating-point
number using the Celcius scale.

Optionally, the temperature may be returned
using the Farenheit scale as follows ...

      float irTemperature = irTemp.getIRTemperature(true);
      float amTemperature = irTemp.getAmbientTemperature(true);

The temperature module _clock pin_ must be connected to either Arduino
pin 2 or pin 3.  If pin 2 is used, then Arduino interrupt 0 is used by
the library.  Otherwise, if pin 3 is used, then Arduino interrupt 1 is
used by the library.

See examples/readTemperature/readTemperature.ino for working source code.

This library works with Arduino IDE versions prior to 1.0.
However, the examples will need to be renamed from *.ino to *.pde.
