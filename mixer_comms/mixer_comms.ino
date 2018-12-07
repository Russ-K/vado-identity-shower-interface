//Transmission config
long baudRate = 9600;
byte serialConfig = SERIAL_8N2;
//Transmission Details
/////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////
//Timing
long unsigned lastCom = 0;
long unsigned curMicros = 0;
const unsigned long comPeriod = 55240; //should 55240 - 55280 - Arduino is not perfect at timing so 55240 seems to equate to roughly 55260
const int bitLen = 10;

const long unsigned DEDICATED_READ_TIME = 20;
long unsigned msReadEnd = 0;
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
const byte MSG_STARTUP[MSG_LEN] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x41};
const byte MSG_PREPARING[MSG_LEN] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xB3};
const byte MSG_HEARTBEAT[MSG_LEN] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1D}; //it might be that this should be the current mixer water temperature
const byte MSG_READY[MSG_LEN] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xB1};
//Outgoing message types
const char INTRO = 0;
const char STARTUP = 1;
const char PREPARING = 2;
const char HEARTBEAT = 3;
const char READY = 4;
//Outgoing message type
char curMsg = INTRO;
//Outgoing register
byte message[MSG_LEN];
//Handshaking
bool handshakeRequired = true;
int handshakeStage = 0;
//Startup
bool startupRequired = true;
int startupStage = 0;
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
const char CONT_ALIVE = 2;
//Incoming register
byte receivedData[MSG_LEN];
int nCurByte = 0;
//Controller message details
/////////////////////////////////////////////////////////////////

void setup() {
  Serial.begin(baudRate, serialConfig);
  Serial1.begin(baudRate, serialConfig);

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
  if (receivedData[CONT_BYTE_OUTLET] == CONT_ALIVE) {
    if (handshakeRequired) {
      switch(handshakeStage++) {
        case 0:
        case 2:
        case 4:
          return STARTUP;
          break;
        case 5:
          handshakeRequired = false;
        case 1:
        case 3:
          return PREPARING;
          break;
        default:
          return INTRO;
          break;
      }
    }

    if (startupRequired) {
      switch(startupStage++) {
        case 0:
        case 2:
        case 4:
          return HEARTBEAT;
          break;
        case 5:
          startupRequired = false;
        case 1:
        case 3:
          return PREPARING;
          break;
        default:
          return INTRO;
          break;
      }
    }
    
    if (curMsg == HEARTBEAT) {
      //Serial.println("REST");
      return READY;
    }
    else {
      //Serial.println("HEARTBEAT");
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
    case STARTUP:
      memcpy(setMsg, MSG_STARTUP, MSG_LEN);
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
  while (Serial1.available() > 0) {
    receivedData[nCurByte++] = Serial1.read();

    if (nCurByte >= MSG_LEN) {
      nCurByte = 0;
      return true;
    }
  }
  return false;
}
