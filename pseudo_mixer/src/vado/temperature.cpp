/*
  temperature.h - Library for retrieving a temperature reading
  Created by Russell Keane, January 6, 2019.
  Released into the public domain.

  Adapted from https://learn.adafruit.com/thermistor/using-a-thermistor
*/

#include "Arduino.h"
#include "temperature.h"

Temperature::Temperature(int temperaturePin, int betaValue, int seriesResistor, int thermistorNominal, int sampleCount, int sampleWait, int temperatureNominal)
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