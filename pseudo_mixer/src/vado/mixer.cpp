/*
  mixer.h - Library for constructing data packets for a Vado Identity Mixer
  Created by Russell Keane, December 8, 2018.
  Released into the public domain.
*/

#include "Arduino.h"
#include "mixer.h"


const byte Mixer::MSG_INTRO[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
const byte Mixer::MSG_PREPARING[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xB3};
const byte Mixer::MSG_HEARTBEAT[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1D}; //it might be that this should be the current mixer water temperature
const byte Mixer::MSG_READY[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xB1};

Mixer::Mixer(int powerPin, ThermistorParams thermistorParams)
{
  _powerPin = powerPin;
  pinMode(_powerPin, OUTPUT);

  _temperatureSensor.Init(thermistorParams);
}

bool Mixer::GetResponse(ControllerState& controllerState, byte setMsg[])
{
    _curMsg = CalcResponse(controllerState);
    SetMessage(_curMsg, setMsg);
}

void Mixer::UpdateSystemState(ControllerState& controllerState)
{
  _isOn = controllerState.isOn();
  digitalWrite(_powerPin, _isOn ? HIGH : LOW);
}

char Mixer::CalcResponse(ControllerState& controllerState)
{
  if (controllerState.isValid()) {
    if (_curMsg == HEARTBEAT) {
      return READY;
    }
    else {
      return HEARTBEAT;
    }
  }

  return INTRO;
}

const void Mixer::SetMessage(char required, byte setMsg[])
{
  switch(required) {
    case HEARTBEAT:
      memcpy(setMsg, Mixer::MSG_HEARTBEAT, MSG_LEN);
      break;
    case READY:
      memcpy(setMsg, Mixer::MSG_READY, MSG_LEN);
      break;
    case PREPARING:
      memcpy(setMsg, Mixer::MSG_PREPARING, MSG_LEN);
      break;
    case INTRO:
    default:
      memcpy(setMsg, Mixer::MSG_INTRO, MSG_LEN);
      break;
  }
}

void Mixer::Process()
{
  if (_isOn)
  {
    _temperatureSensor.GetCurrentTemp();
  }
}