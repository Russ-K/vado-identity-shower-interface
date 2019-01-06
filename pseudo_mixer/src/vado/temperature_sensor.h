/*
  temperature_sensor.h - Library for retrieving a temperature reading
  Created by Russell Keane, January 6, 2019.
  Released into the public domain.

  Adapted from https://learn.adafruit.com/thermistor/using-a-thermistor
*/
#ifndef vado_temperature
#define vado_temperature

#include "Arduino.h"
#include "constants.h"
#include "thermistor_params.h"

class TemperatureSensor
{
  public:
    TemperatureSensor();
    const float GetCurrentTemp();

    void Init(ThermistorParams thermistorParams);

  private:
    const float GetCurrentThermistorTemp();

    ThermistorParams _thermistorParams;
};

#endif