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
    ControllerState(bool valid, int power, int temp, int flow, int isMainOutlet);
    bool isValid() { return _isValid; }

    //Power
    bool isOn() { return isValid() && _power == POWER_ON; }
    bool isPaused() { return isValid() && _power == POWER_PAUSED; }
    //Temp
    int temp() { return isValid() && _temperature <= TEMP_MAX_C ? _temperature : TEMP_MIN_C; }
    //Flow
    int flow() { return isValid() ? _flow : FLOW_MIN; }
    //Outlet
    int isMainOutlet() { return isValid() ? _outlet : OUTLET_DEFAULT; }
    private:
    bool _isValid = false;

    int _power = 0;
    int _temperature = 0;
    int _flow = 0;
    int _outlet = 0;
};

#endif