/*
  controllerState.h - Library for packaging controller state
  Created by Russell Keane, December 13, 2018.
  Released into the public domain.
*/
#ifndef vado_controller_state
#define vado_controller_state

#include "Arduino.h"
#include "constants.h"


class ControllerState
{
  public:
    ControllerState();
    ControllerState(const ControllerState& toCopy);
    ControllerState(bool valid, uint8_t power, int8_t temp, uint8_t flow, uint8_t outlet);

    const bool operator == (const ControllerState& rhs);

    const bool isValid() { return _isValid; }

    //Power
    const bool isOn() { return isValid() && _power == POWER_ON; }
    const bool isPaused() { return isValid() && _power == POWER_PAUSED; }
    //Temp
    const int8_t temp() { return isValid() && _temperature <= TEMP_MAX_C ? _temperature : TEMP_MIN_C; }
    //Flow
    const uint8_t flow() { return isValid() ? _flow : FLOW_MIN; }
    //Outlet
    const bool isMainOutlet() { return isValid() ? _outlet == OUTLET_DEFAULT : true; }
  private:
    bool _isValid = false;

    uint8_t _power = 0;
    int8_t _temperature = 0;
    uint8_t _flow = 0;
    uint8_t _outlet = 0;
};

#endif