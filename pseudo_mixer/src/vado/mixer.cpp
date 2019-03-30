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

const float Mixer::ALLOWABLE_TEMP_DIFFERENTIAL = 0.5;

Mixer::Mixer(int powerPin, int solenoidSelectionPin, int proportioningValvePowerPin, int proportioningValveDirectionPin, ThermistorParams thermistorParams)
{
  _powerPin = powerPin;
  pinMode(_powerPin, OUTPUT);
  digitalWrite(_powerPin, RELAY_LOW); //default power off

  _solenoidSelectionPin = solenoidSelectionPin;
  pinMode(_solenoidSelectionPin, OUTPUT);
  digitalWrite(_solenoidSelectionPin, RELAY_LOW);

  _proportioningValvePowerPin = proportioningValvePowerPin;
  pinMode(_proportioningValvePowerPin, OUTPUT);
  digitalWrite(_proportioningValvePowerPin, RELAY_LOW); //default power off

  _proportioningValveDirectionPin = proportioningValveDirectionPin;
  pinMode(_proportioningValveDirectionPin, OUTPUT);
  digitalWrite(_proportioningValveDirectionPin, RELAY_LOW);

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
  digitalWrite(_powerPin, _isOn ? RELAY_HIGH : RELAY_LOW);

  if (_isOn)
  {
    digitalWrite(_solenoidSelectionPin, controllerState.isMainOutlet() ? RELAY_LOW : RELAY_HIGH);
    _targetTemperature = controllerState.temp();
  }
  else
  {
    digitalWrite(_solenoidSelectionPin, RELAY_LOW);
    digitalWrite(_proportioningValvePowerPin, RELAY_LOW);
    digitalWrite(_proportioningValveDirectionPin, RELAY_LOW);
  }
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
    Mixer::TempSuitability currentSuitability = EvaluateTempSuitability(_targetTemperature, _temperatureSensor.GetCurrentTemp());
    ChangeTemp(currentSuitability);
  }
}

Mixer::TempSuitability Mixer::EvaluateTempSuitability(int requiredTemp, float currentTemp)
{
  if (currentTemp < (requiredTemp - ALLOWABLE_TEMP_DIFFERENTIAL))
  {
    return TempSuitability::TooCold;
  }
  else if (currentTemp > (requiredTemp + ALLOWABLE_TEMP_DIFFERENTIAL))
  {
    return TempSuitability::TooHot;
  }

  return TempSuitability::JustRight;
}

void Mixer::ChangeTemp(TempSuitability currentSuitability)
{
  if (currentSuitability == TempSuitability::JustRight)
  {
    digitalWrite(_proportioningValvePowerPin, RELAY_LOW);
    digitalWrite(_proportioningValveDirectionPin, RELAY_LOW);
  }
  else
  {
    if (currentSuitability == TempSuitability::TooCold)
    {
      digitalWrite(_proportioningValveDirectionPin, RELAY_LOW);
    }
    else
    {
      digitalWrite(_proportioningValveDirectionPin, RELAY_HIGH);
    }

    digitalWrite(_proportioningValvePowerPin, RELAY_HIGH);
  }
}