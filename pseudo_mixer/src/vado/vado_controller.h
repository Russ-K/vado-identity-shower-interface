/*
  vado_controller.h - Library for parsing and validating data packets from a Vado Identity Controller
  Created by Russell Keane, December 8, 2018.
  Released into the public domain.
*/
#ifndef vado_controller
#define vado_controller

#include "Arduino.h"
#include "vado_constants.h"

class Controller
{
  public:
    Controller();
    void parse(const byte[MSG_LEN]);
    bool isValid() {return _isValid;}
  private:
    bool _isValid;

    int _power = 0;
    int _temperature = 0;
    int _flow = 0;
    int _outlet = 0;

    bool parsePower(const byte);
    bool parseTemperature(const byte);
    bool parseFlow(const byte);
    bool parseOutlet(const byte);

    //Controller message details
    //Message byte positions
    const char CONT_BYTE_1 = 0;
    const char CONT_BYTE_2 = 1;
    const char CONT_BYTE_POWER = 2;
    const char CONT_BYTE_TEMP = 3;
    const char CONT_BYTE_FLOW = 4;
    const char CONT_BYTE_OUTLET = 5;
    const char CONT_BYTE_END = 6;

    //Power definitions
    const byte CONT_POWER_ON = 0x01;
    const char CONT_POWER_OFF = 0x02;
    const char CONT_POWER_PAUSED = 0x03;

    //Outlet definitions
    const byte CONT_OUTLET_DEFAULT = 0x02;
    const char CONT_OUTLET_ALTERNATIVE = 0x01;
};

#endif