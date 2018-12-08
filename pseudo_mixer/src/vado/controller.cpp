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

void Controller::parse(const byte data [MSG_LEN])
{
    if (parsePower(data[BYTE_POWER]) &&
            parseTemperature(data[BYTE_TEMP]) &&
            parseFlow(data[BYTE_FLOW]) &&
            parseOutlet(data[BYTE_OUTLET])) {
        _isValid = true;
    }
    else {
        _isValid = false;
    }
}

bool Controller::parsePower(const byte value)
{
    bool valid = false;
    switch (value) {
        case POWER_ON:
        case POWER_OFF:
        case POWER_PAUSED:
            _power = value;
            valid = true;
            break;
        default:
            _power = 0;
            break;
    }
    return valid;
}

bool Controller::parseTemperature(const byte value)
{
    bool valid = false;
    if (value >= TEMP_MIN && value <= TEMP_MAX) {
        _temperature = value - TEMP_OFFSET;
        valid = true;
    }
    return valid;
}

bool Controller::parseFlow(const byte value)
{
    bool valid = false;
    switch (value) {
        case FLOW_MIN :
        case FLOW_LOW :
        case FLOW_MED :
        case FLOW_HIGH :
        case FLOW_MAX :
            _flow = value;
            valid = true;
            break;
        default:
            _flow = 0;
            break;
    }
    return valid;
}

bool Controller::parseOutlet(const byte value)
{
    bool valid = false;
    switch (value) {
        case OUTLET_DEFAULT:
        case OUTLET_ALTERNATIVE:
            _outlet = value;
            valid = true;
            break;
        default:
            _outlet = 0;
            break;
    }
    return valid;
}
