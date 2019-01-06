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
    uint16_t sampleTotal = 0;
    // take N samples in a row, with a slight delay
    for (uint8_t i = 0; i < _sampleCount; i++) {
        sampleTotal += analogRead(_temperaturePin);
        delay(_sampleWait);
    }

    float sampleAverage = sampleTotal / _sampleCount;
    Serial.print("Average analog reading "); 
    Serial.println(sampleAverage);

      // convert the value to resistance
    sampleAverage = 1023 / sampleAverage - 1;
    sampleAverage = _seriesResistor / sampleAverage;
    Serial.print("Thermistor resistance "); 
    Serial.println(sampleAverage);

    float steinhart;
    steinhart = sampleAverage / _thermistorNominal;     // (R/Ro)
    steinhart = log(steinhart);                         // ln(R/Ro)
    steinhart /= _betaValue;                            // 1/B * ln(R/Ro)
    steinhart += 1.0 / (_temperatureNominal + 273.15);  // + (1/To)
    steinhart = 1.0 / steinhart;                        // Invert
    steinhart -= 273.15;                                // convert to C
    
    Serial.print("Temperature "); 
    Serial.print(steinhart);
    Serial.println(" *C");

    return steinhart;
}