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
    static const byte CONT_BYTE_1 = 0;
    static const byte CONT_BYTE_2 = 1;
    static const byte CONT_BYTE_POWER = 2;
    static const byte CONT_BYTE_TEMP = 3;
    static const byte CONT_BYTE_FLOW = 4;
    static const byte CONT_BYTE_OUTLET = 5;
    static const byte CONT_BYTE_END = 6;

    //Power definitions
    static const byte CONT_POWER_ON = 0x01;
    static const byte CONT_POWER_OFF = 0x02;
    static const byte CONT_POWER_PAUSED = 0x03;

    //Temperature definitions
    static const int TEMP_OFFSET = 16; //the value sent from the controller is 16 higher than the deg C value
    static const byte CONT_TEMP_MIN = 0x29;
    static const byte CONT_TEMP_MAX = 0x3E;

    //Flow definitions
    static const byte CONT_FLOW_MIN = 0x05;
    static const byte CONT_FLOW_LOW = 0x06;
    static const byte CONT_FLOW_MED = 0x07;
    static const byte CONT_FLOW_HIGH = 0x08;
    static const byte CONT_FLOW_MAX = 0x09;

    //Outlet definitions
    static const byte CONT_OUTLET_DEFAULT = 0x02;
    static const byte CONT_OUTLET_ALTERNATIVE = 0x01;
};

#endif