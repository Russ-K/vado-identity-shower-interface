/*
  controller.h - Library for parsing and validating data packets from a Vado Identity Controller
  Created by Russell Keane, December 8, 2018.
  Released into the public domain.
*/
#ifndef vado_controller
#define vado_controller

#include "Arduino.h"
#include "constants.h"
#include "controllerState.h"

class Controller
{
  public:
    Controller();

    static ControllerState parse(const byte[MSG_LEN]);

    void SetState(const ControllerState& newState);
    bool StateChanged(const ControllerState& newState);
  private:
    ControllerState _controllerState;

    static bool parsePower(const byte, int& power);
    static bool parseTemperature(const byte, int& temperature);
    static bool parseFlow(const byte, int& flow);
    static bool parseOutlet(const byte, int& outlet);
};

#endif