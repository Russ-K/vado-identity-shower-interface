/*
  temperature.h - Library for retrieving a temperature reading
  Created by Russell Keane, January 6, 2019.
  Released into the public domain.

  Adapted from https://learn.adafruit.com/thermistor/using-a-thermistor
*/

#include "Arduino.h"
#include "temperature.h"

Temperature::Temperature(uint8_t temperaturePin, uint16_t betaValue, uint16_t seriesResistor, uint16_t thermistorNominal, uint8_t sampleCount, uint8_t sampleWait, int8_t temperatureNominal)
{
  _temperaturePin = temperaturePin;
  _betaValue = betaValue;
  _seriesResistor = seriesResistor;
  _thermistorNominal = thermistorNominal;
  _sampleCount = sampleCount;
  _sampleWait = sampleWait;
  _temperatureNominal = temperatureNominal;
}

const float Temperature::GetCurrentTemp()
{
}