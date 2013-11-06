IRTemp Arduino Library v1.0
===========================
Infrared non-contact thermometer library for the Arduino.

This library supports the IRTEMP IR Temperature Sensor Module from
Freetronics, based on the TN9 sensor. For more information see:

      www.freetronics.com/irtemp

The TN9 measures:

- Infrared temperature in conical zone in front of the sensor
- Ambient temperature

The library needs to be initialized with the Arduino pin configurations.

      #include "IRTemp.h"
      static const byte PIN_DATA    = 2;
      static const byte PIN_CLOCK   = 3;
      static const byte PIN_ACQUIRE = 4;
      IRTemp irTemp(PIN_ACQUIRE, PIN_CLOCK, PIN_DATA);

Any 3 unused pins can be chosen.

Once the library has been initialized, the two different temperature values
can be obtained as follows:

      float irTemperature = irTemp.getIRTemperature();
      float amTemperature = irTemp.getAmbientTemperature();

By default, these functions return the temperature value as a floating-point
number using the Celcius scale.

Optionally, the temperature may be returned
using the Farenheit scale as follows:

      float irTemperature = irTemp.getIRTemperature(FAHRENHEIT);
      float amTemperature = irTemp.getAmbientTemperature(FAHRENHEIT);

Choose File -> Examples -> IRTemp -> readTemperature in the Arduino
IDE to see an example, including error handling.

This library also works with Arduino IDE versions prior to 1.0.
However, the examples will need to be renamed from *.ino to *.pde.

# Using Multiple Sensors

It is possible to connect multiple IRTemp sensors to a single
Arduino. However you cannot share clock and data pins between sensors,
as data is sent continously (see below.)

# Two Wire Operation

It is possible to use the IRTemp without the "Acquire" pin. When used
in two wire mode, the Acquire pin on the IRTemp sensor should be
connected directly to ground. Readings for IR & Ambient temp will be
continuously output by the sensor.

If the acquire pin is left floating or pulled up to HIGH, readings for
Ambient temp but not IR temp will be continuously output by the
sensor.

Note that the time to take a reading is increased in two-wire mode
(approximately one sample every 500ms, alternating IR & Ambient.)
