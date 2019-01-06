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
    Temperature(uint8_t temperaturePin, uint16_t betaValue, uint16_t seriesResistor = 10000, uint16_t thermistorNominal = 10000, uint8_t sampleCount = 5, uint8_t sampleWait = 1, int8_t temperatureNominal = 25);
    const float GetCurrentTemp();

  private:
    // which analog pin to connect
    uint8_t _temperaturePin = 0;
    // resistance at 25 degrees C
    uint16_t _thermistorNominal = 0;
    // temp. for nominal resistance (almost always 25 C)
    int8_t _temperatureNominal = 0;
    // how many samples to take and average, more takes longer
    // but is more 'smooth'
    uint8_t _sampleCount = 0;
    // how long to wait between samples
    uint8_t _sampleWait = 0;
    // The beta coefficient of the thermistor (usually 3000-4000)
    uint16_t _betaValue = 0;
    // the value of the 'other' resistor
    uint16_t _seriesResistor = 0;
};

#endif