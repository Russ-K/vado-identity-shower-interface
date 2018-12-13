/*
  controller.h - Library for parsing and validating data packets from a Vado Identity Controller
  Created by Russell Keane, December 8, 2018.
  Released into the public domain.
*/

#include "Arduino.h"
#include "controller.h"

Controller::Controller()
{
}

void Controller::SetState(const ControllerState& newState)
{
    _controllerState = newState;
}

bool Controller::StateChanged(const ControllerState& newState)
{
    return !(_controllerState == newState);
}

ControllerState Controller::parse(const byte data [MSG_LEN])
{
    bool _isValid = false;
    int _power = 0;
    int _temperature = 0;
    int _flow = 0;
    int _outlet = 0;

    if (parsePower(data[BYTE_POWER], _power) &&
            parseTemperature(data[BYTE_TEMP], _temperature) &&
            parseFlow(data[BYTE_FLOW], _flow) &&
            parseOutlet(data[BYTE_OUTLET], _outlet)) {
        _isValid = true;
    }
    else {
        _isValid = false;
    }

    ControllerState controllerState(_isValid, _power, _temperature, _flow, _outlet);
    return controllerState;
}

bool Controller::parsePower(const byte value, int& power)
{
    bool valid = false;
    switch (value) {
        case POWER_ON:
        case POWER_OFF:
        case POWER_PAUSED:
            power = value;
            valid = true;
            break;
        default:
            power = 0;
            break;
    }
    return valid;
}

bool Controller::parseTemperature(const byte value, int& temperature)
{
    bool valid = false;
    if (value >= TEMP_MIN && value <= TEMP_MAX) {
        temperature = value - TEMP_OFFSET;
        valid = true;
    }
    return valid;
}

bool Controller::parseFlow(const byte value, int& flow)
{
    bool valid = false;
    switch (value) {
        case FLOW_MIN :
        case FLOW_LOW :
        case FLOW_MED :
        case FLOW_HIGH :
        case FLOW_MAX :
            flow = value;
            valid = true;
            break;
        default:
            flow = 0;
            break;
    }
    return valid;
}

bool Controller::parseOutlet(const byte value, int& outlet)
{
    bool valid = false;
    switch (value) {
        case OUTLET_DEFAULT:
        case OUTLET_ALTERNATIVE:
            outlet = value;
            valid = true;
            break;
        default:
            outlet = 0;
            break;
    }
    return valid;
}
