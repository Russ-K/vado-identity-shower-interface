/*
  Morse.cpp - Library for flashing Morse code.
  Created by David A. Mellis, November 2, 2007.
  Released into the public domain.
*/

#include "Arduino.h"
#include "vado_controller.h"

Controller::Controller()
{
}

void Controller::parse(const byte data [MSG_LEN])
{
    if (parsePower(data[CONT_BYTE_POWER]) &&
            parseTemperature(data[CONT_BYTE_TEMP]) &&
            parseFlow(data[CONT_BYTE_FLOW]) &&
            parseOutlet(data[CONT_BYTE_OUTLET])) {
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
        case CONT_POWER_ON:
        case CONT_POWER_OFF:
        case CONT_POWER_PAUSED:
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
    bool valid = true;
    switch (value) {
        default:
            _temperature = 0;
            break;
    }
    return valid;
}

bool Controller::parseFlow(const byte value)
{
    bool valid = true;
    switch (value) {
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
        case CONT_OUTLET_DEFAULT:
        case CONT_OUTLET_ALTERNATIVE:
            _outlet = value;
            valid = true;
            break;
        default:
            _outlet = 0;
            break;
    }
    return valid;
}
