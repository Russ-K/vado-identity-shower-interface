/*
  controller.h - Library for parsing and validating data packets from a Vado Identity Controller
  Created by Russell Keane, December 8, 2018.
  Released into the public domain.
*/
#ifndef vado_controller
#define vado_controller

#include "Arduino.h"
#include "constants.h"

class Controller
{
  public:
    Controller();
    void parse(const byte[MSG_LEN]);
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
    static const byte BYTE_1 = 0;
    static const byte BYTE_2 = 1;
    static const byte BYTE_POWER = 2;
    static const byte BYTE_TEMP = 3;
    static const byte BYTE_FLOW = 4;
    static const byte BYTE_OUTLET = 5;
    static const byte BYTE_END = 6;

    //Power definitions
    static const byte POWER_ON = 0x01;
    static const byte POWER_OFF = 0x02;
    static const byte POWER_PAUSED = 0x03;

    //Temperature definitions
    static const int TEMP_OFFSET = 16; //the value sent from the controller is 16 higher than the deg C value
    //Signal MIN and MAX
    static const byte TEMP_MIN = 0x29;
    static const byte TEMP_MAX = 0x3E;
    //Converted MIN and MAX
    static const byte TEMP_MIN_C = 25;
    static const byte TEMP_MAX_C = 46;

    //Flow definition
    static const byte FLOW_MIN = 0x05;
    static const byte FLOW_LOW = 0x06;
    static const byte FLOW_MED = 0x07;
    static const byte FLOW_HIGH = 0x08;
    static const byte FLOW_MAX = 0x09;

    //Outlet definitions
    static const byte OUTLET_DEFAULT = 0x02;
    static const byte OUTLET_ALTERNATIVE = 0x01;
};

#endif