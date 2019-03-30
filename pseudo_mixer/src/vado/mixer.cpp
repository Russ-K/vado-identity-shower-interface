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
const byte Mixer::MSG_ERROR[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; //use the intro message until a proper error message is known

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

bool Mixer::UpdateSystemState(const byte data [MSG_LEN])
{
  lastUpdate = millis();
  ControllerState requestedState = controller.parse(data);
  if (requestedState.isValid()) {
    if (controller.StateChanged(requestedState)) {
      controller.SetState(requestedState);
      PrintData(requestedState);
      UpdateSystemState(requestedState);
    }
    erroredReads = 0;
  }
  else
    ++erroredReads;

  _curMsg = CalcResponse(requestedState);
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
  if (IsInFailureTimeout())
  {
    required = ERROR;
    Serial.println("Failure - in failure timeout");
  }

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
    case ERROR:
      memcpy(setMsg, Mixer::MSG_ERROR, MSG_LEN);
      break;
    case INTRO:
    default:
      memcpy(setMsg, Mixer::MSG_INTRO, MSG_LEN);
      break;
  }
}

void Mixer::Process(byte setMsg[])
{
  if (ShouldFailsafe())
  {
    ControllerState controllerState(true, POWER_OFF, TEMP_MIN, FLOW_MIN, OUTLET_DEFAULT);
    UpdateSystemState(controllerState);
    _curMsg = ERROR;
    failureTimeout = millis();
    Serial.println("Failure - extending failure timeout");
  }

  SetMessage(_curMsg, setMsg);

  if (_isOn)
  {
    float smoothTemp = SmoothTemp(_temperatureSensor.GetCurrentTemp());
    Mixer::TempSuitability currentSuitability = EvaluateTempSuitability(_targetTemperature, smoothTemp);
    ChangeTemp(currentSuitability);
  }
}

bool Mixer::ShouldFailsafe()
{
  return (erroredReads > MAX_ERRORED_READS) || (millis() - lastUpdate >= MAX_FAILED_READ_TIME);
}

bool Mixer::IsInFailureTimeout()
{
  return millis() - failureTimeout <= FAILURE_BACKOFF_TIME;
}

float Mixer::SmoothTemp(float currentTemp)
{
  float smoothedTemp = 0;
  for (int i = 0; i < (SMOOTHING_ARR_SIZE - 1); ++i)
  {
    tempSmoothingArr[i] = tempSmoothingArr[i + 1];
    smoothedTemp += tempSmoothingArr[i];
  }
  tempSmoothingArr[SMOOTHING_ARR_SIZE - 1] = currentTemp;
  smoothedTemp += currentTemp;

  smoothedTemp /= SMOOTHING_ARR_SIZE;

  Serial.print("Smoothed Temperature "); 
  Serial.print(smoothedTemp);
  Serial.print(" *C");
  Serial.print(" - Real Temperature "); 
  Serial.print(currentTemp);
  Serial.println(" *C");

  return smoothedTemp;
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

void Mixer::PrintData(ControllerState& newState)
{
  Serial.println("State changed");
  Serial.print("Power is ");
  Serial.println(newState.isOn() ? "on" : newState.isPaused() ? "paused" : "off");
  
  Serial.print("Temp is ");
  Serial.println(newState.temp());
  Serial.print("Flow is ");
  Serial.println(newState.flow());
  Serial.print("Outlet is ");
  Serial.println(newState.isMainOutlet() ? "main" : "alternative");
  Serial.println("");
}