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
//Message Length
const int MSG_LEN = 7;
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
//Controller message details
//Message byte positions
const char CONT_BYTE_1 = 0;
const char CONT_BYTE_2 = 1;
const char CONT_BYTE_POWER = 2;
const char CONT_BYTE_TEMP = 3;
const char CONT_BYTE_FLOW = 4;
const char CONT_BYTE_OUTLET = 5;
const char CONT_BYTE_END = 6;
//Message meanings
const byte CONT_OUTLET_DEFAULT = 0x02;
const char CONT_OUTLET_ALTERNATIVE = 0x01;
//Incoming register
byte receivedData[MSG_LEN];
int nCurByte = 0;
//Controller message details
/////////////////////////////////////////////////////////////////

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
    PrintData();
    curMsg = CalcResponse();
  }

  SetMessage(curMsg, message);
}

void PrintData() {
  for(int i = 0; i < MSG_LEN; i++)
  {
    Serial.print(receivedData[i], HEX);
    Serial.print(" ");
  }
  Serial.println("");
}

char CalcResponse() {
  if (receivedData[CONT_BYTE_OUTLET] == CONT_OUTLET_DEFAULT || receivedData[CONT_BYTE_OUTLET] == CONT_OUTLET_ALTERNATIVE) {
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
