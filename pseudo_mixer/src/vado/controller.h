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

    ControllerState parse(const byte[MSG_LEN]);
  private:

    bool parsePower(const byte, int& power);
    bool parseTemperature(const byte, int& temperature);
    bool parseFlow(const byte, int& flow);
    bool parseOutlet(const byte, int& outlet);
};

#endif