#include "src/vado/controller.h"
#include "src/vado/mixer.h"

/////////////////////////////////////////////////////////////////
//Transmission Details
//Serial config
long baudRate = 9600;
byte serialConfig = SERIAL_8N2;

//max expected time for reading data
const int MAX_READ_TIME = 10;
//Transmission Details
/////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////
//Timing
//Vars used to ensure signals are sent at the correct times to the controller
long unsigned lastCom = 0;
long unsigned curMicros = 0;
const unsigned long comPeriod = 55240; //should 55240 - 55280 - Arduino is not perfect at timing so 55240 seems to equate to roughly 55260
//Timing
/////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////
//Outgoing message details
//Outgoing messages
const byte MSG_INTRO[MSG_LEN] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
const byte MSG_PREPARING[MSG_LEN] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xB3};
const byte MSG_HEARTBEAT[MSG_LEN] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1D}; //it might be that this should be the current mixer water temperature
const byte MSG_READY[MSG_LEN] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xB1};
//Outgoing message types
const char INTRO = 0;
const char PREPARING = 2;
const char HEARTBEAT = 3;
const char READY = 4;
//Outgoing message type
char curMsg = INTRO;
//Outgoing register
byte message[MSG_LEN];
//Outgoing message details
/////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////
//Incoming register
byte receivedData[MSG_LEN];
int nCurByte = 0;
//Controller message details
/////////////////////////////////////////////////////////////////

Controller controller;

void setup() {
  Serial.begin(baudRate, serialConfig);
  Serial1.begin(baudRate, serialConfig);

  Serial.setTimeout(MAX_READ_TIME);

  memcpy(message, MSG_INTRO, MSG_LEN);
}

void loop()
{
  SendData(message);

  if (ReadData()) {
    controller.parse(receivedData);
    PrintData();
    curMsg = CalcResponse();
  }

  SetMessage(curMsg, message);
}

void PrintData() {
  static bool bOn = false;
  static bool bPaused = false;
  static int temp = 0;
  static int flow = 0;
  static int outlet = 0;

  bool stateChanged = false;
  if (controller.isOn() != bOn) {
    bOn = controller.isOn();
    stateChanged = true;
  }
  if (controller.isPaused() != bPaused) {
    bPaused = controller.isPaused();
    stateChanged = true;
  }
  if (controller.temp() != temp) {
    temp = controller.temp();
    stateChanged = true;
  }
  if (controller.flow() != flow) {
    flow = controller.flow();
    stateChanged = true;
  }
  if (controller.isMainOutlet() != outlet) {
    outlet = controller.isMainOutlet();
    stateChanged = true;
  }

  if (stateChanged) {
    Serial.println("State changed");
    Serial.print("Power is ");
    Serial.println(bOn ? "on" : bPaused ? "paused" : "off");
    
    Serial.print("Temp is ");
    Serial.println(temp);
    Serial.print("Flow is ");
    Serial.println(flow);
    Serial.print("Outlet is ");
    Serial.println(outlet);
    Serial.println("");
  }
}

char CalcResponse() {
  if (controller.isValid()) {
    if (curMsg == HEARTBEAT) {
      return READY;
    }
    else {
      return HEARTBEAT;
    }
  }

  return INTRO;
}

void SetMessage(char required, byte setMsg[]) {
  switch(required) {
    case HEARTBEAT:
      memcpy(setMsg, MSG_HEARTBEAT, MSG_LEN);
      break;
    case READY:
      memcpy(setMsg, MSG_READY, MSG_LEN);
      break;
    case PREPARING:
      memcpy(setMsg, MSG_PREPARING, MSG_LEN);
      break;
    case INTRO:
    default:
      memcpy(setMsg, MSG_INTRO, MSG_LEN);
      break;
  }
}

void SendData(byte sendMsg[]) {
  curMicros = micros();
  if ((curMicros > (lastCom + comPeriod)) || (curMicros < lastCom)) {
    lastCom = curMicros;
    Serial1.write(sendMsg, MSG_LEN);
  }
}

bool ReadData() {
  if (Serial1.available() > 0) {
    return MSG_LEN == Serial1.readBytes(receivedData, MSG_LEN);
  }
  return false;
}
