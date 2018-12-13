/*
  controllerState.h - Library for packaging controller state
  Created by Russell Keane, December 13, 2018.
  Released into the public domain.
*/

#include "Arduino.h"
#include "controllerState.h"

ControllerState::ControllerState()
{
}

ControllerState::ControllerState(bool valid, int power, int temp, int flow, int isMainOutlet)
{
    _isValid = valid;

    _power = power;
    _temperature = temp;
    _flow = flow;
    _outlet = isMainOutlet;
}

ControllerState::ControllerState(const ControllerState& toCopy)
{
    _isValid = toCopy._isValid;

    _power = toCopy._power;
    _temperature = toCopy._temperature;
    _flow = toCopy._flow;
    _outlet = toCopy._outlet;
}