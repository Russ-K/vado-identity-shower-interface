
/////////////////////////////////////////////////////////////////
//Transmission Details
//RS485 Control
#define TxControlMixer      3   //RS485 Direction control
#define RS485Transmit       HIGH
#define RS485Receive        LOW

//Transmission config
long baudRate = 9600;
byte serialConfig = SERIAL_8O1;
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
const byte MSG_STARTUP_A[MSG_LEN] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x21};
const byte MSG_STARTUP_B[MSG_LEN] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x53};
const byte MSG_HEARTBEAT[MSG_LEN] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x16};
const byte MSG_REST[MSG_LEN] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x51};
//Outgoing message types
const char INTRO = 0;
const char STARTUP_A = 1;
const char STARTUP_B = 2;
const char HEARTBEAT = 3;
const char REST = 4;
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
const char CONT_BYTE_3 = 2;
const char CONT_BYTE_4 = 3;
const char CONT_BYTE_5 = 4;
const char CONT_BYTE_LIVE = 5;
const char CONT_BYTE_END = 6;
//Message meanings
const char CONT_ALIVE = 2;
//Incoming register
byte receivedData[MSG_LEN];
//Controller message details
/////////////////////////////////////////////////////////////////

void setup() {
  Serial.begin(9600, serialConfig);
  Serial1.begin(baudRate, serialConfig);

  memcpy(message, MSG_INTRO, MSG_LEN);

  // Init Transceiver
  pinMode(TxControlMixer, OUTPUT);
  digitalWrite(TxControlMixer, RS485Receive);
}

void loop()
{
  SendData(message);

  if (ReadData(receivedData)) {
    //PrintData(receivedData);
    curMsg = CalcResponse(receivedData);
  }

  SetMessage(curMsg, message);
}

void PrintData(byte readMsg[]) {
  for(int i = 0; i < MSG_LEN; i++)
  {
    Serial.print(readMsg[i], HEX);
    Serial.print(" ");
  }
  Serial.println("");
}

char CalcResponse(byte readMsg[]) {
  if (readMsg[CONT_BYTE_LIVE] == CONT_ALIVE) {
    if (curMsg == HEARTBEAT) {
      //Serial.println("REST");
      return REST;
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
    case REST:
      memcpy(setMsg, MSG_REST, MSG_LEN);
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

    digitalWrite(TxControlMixer, RS485Transmit);  // Enable RS485 Transmit
    Serial1.write(sendMsg, MSG_LEN);
    digitalWrite(TxControlMixer, RS485Receive);  // Enable RS485 Receive
  }
}

bool ReadData(byte readMsg[]) {
  bool foundData = false;
  if (Serial1.available() > 0) {
    msReadEnd = millis() + DEDICATED_READ_TIME;
  
    int nCurByte = 0;
    while (msReadEnd > millis()) {
      if (Serial1.available() > 0) {
        // read the incoming byte:
        readMsg[nCurByte++] = Serial1.read();
      }

      if (nCurByte >= MSG_LEN) {
        nCurByte = 0;
        foundData = true;
      }
    }
  }
  return foundData;
}
