/*
  temperature.h - Library for retrieving a temperature reading
  Created by Russell Keane, January 6, 2019.
  Released into the public domain.

  Adapted from https://learn.adafruit.com/thermistor/using-a-thermistor
*/
#ifndef vado_temperature
#define vado_temperature

#include "Arduino.h"
#include "constants.h"

class Temperature
{
  public:
    Temperature(int temperaturePin, int betaValue, int seriesResistor = 10000, int thermistorNominal = 10000, int sampleCount = 5, int sampleWait = 1, int temperatureNominal = 25);
    const float GetCurrentTemp();

  private:
    // which analog pin to connect
    int _temperaturePin = 0;
    // resistance at 25 degrees C
    int _thermistorNominal = 0;
    // temp. for nominal resistance (almost always 25 C)
    int _temperatureNominal = 0;
    // how many samples to take and average, more takes longer
    // but is more 'smooth'
    int _sampleCount = 0;
    // how long to wait between samples
    int _sampleWait = 0;
    // The beta coefficient of the thermistor (usually 3000-4000)
    int _betaValue = 0;
    // the value of the 'other' resistor
    int _seriesResistor = 0;
};

#endif