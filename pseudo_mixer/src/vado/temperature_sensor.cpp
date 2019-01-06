/*
  temperature_sensor.cpp - Library for retrieving a temperature reading
  Created by Russell Keane, January 6, 2019.
  Released into the public domain.

  Adapted from https://learn.adafruit.com/thermistor/using-a-thermistor
*/

#include "Arduino.h"
#include "temperature_sensor.h"

TemperatureSensor::TemperatureSensor()
{
}

void TemperatureSensor::Init(ThermistorParams thermistorParams)
{
    _thermistorParams = thermistorParams;
    if (_thermistorParams._temperaturePowerPin > 0) 
        pinMode(_thermistorParams._temperaturePowerPin, OUTPUT);
}

const float TemperatureSensor::GetCurrentTemp()
{
    return GetCurrentThermistorTemp();
}

const float TemperatureSensor::GetCurrentThermistorTemp()
{
    if (_thermistorParams._temperaturePowerPin > 0)
        digitalWrite(_thermistorParams._temperaturePowerPin, HIGH);

    uint16_t sampleTotal = 0;
    // take N samples in a row, with a slight delay
    for (uint8_t i = 0; i < _thermistorParams._sampleCount; i++) {
        sampleTotal += analogRead(_thermistorParams._temperaturePin);
        delay(_thermistorParams._sampleWait);
    }

    float sampleAverage = sampleTotal / _thermistorParams._sampleCount;
    // Serial.print("Average analog reading "); 
    // Serial.println(sampleAverage);

    // convert the value to resistance
    sampleAverage = 1023 / sampleAverage - 1;
    sampleAverage = _thermistorParams._seriesResistor / sampleAverage;
    // Serial.print("Thermistor resistance "); 
    // Serial.println(sampleAverage);

    float steinhart;
    steinhart = sampleAverage / _thermistorParams._thermistorNominal;     // (R/Ro)
    steinhart = log(steinhart);                         // ln(R/Ro)
    steinhart /= _thermistorParams._betaValue;                            // 1/B * ln(R/Ro)
    steinhart += 1.0 / (_thermistorParams._temperatureNominal + 273.15);  // + (1/To)
    steinhart = 1.0 / steinhart;                        // Invert
    steinhart -= 273.15;                                // convert to C
    
    // Serial.print("Temperature "); 
    // Serial.print(steinhart);
    // Serial.println(" *C");

    if (_thermistorParams._temperaturePowerPin > 0)
        digitalWrite(_thermistorParams._temperaturePowerPin, LOW);

    return steinhart;
}