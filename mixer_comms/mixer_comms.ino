long baudRate = 8752;
byte serialConfig = SERIAL_8N1;

#define TxControlMixer      3   //RS485 Direction control
#define RS485Transmit       HIGH
#define RS485Receive        LOW

long unsigned lastCom = 0;
long unsigned curMicros = 0;
const unsigned long comPeriod = 55000;
const int bitLen = 10;

const int MSG_LEN = 7;
const byte MSG_INTRO[MSG_LEN] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
const byte MSG_STARTUP_A[MSG_LEN] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x21};
const byte MSG_STARTUP_B[MSG_LEN] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x53};
const byte MSG_HEARTBEAT[MSG_LEN] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x16};
const byte MSG_REST[MSG_LEN] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x51};
byte message[MSG_LEN];
byte receivedData[MSG_LEN];

const long unsigned DEDICATED_READ_TIME = 20;
long unsigned msReadEnd = 0;

const char INTRO = 0;
const char STARTUP_A = 1;
const char STARTUP_B = 2;
const char HEARTBEAT = 3;
const char REST = 4;

char curMsg = INTRO;

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
    PrintData(receivedData);
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
  return INTRO;
}

void SetMessage(char required, byte setMsg[]) {
  switch(required) {
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
    for (int i = 0; i < MSG_LEN; ++i) {
      Serial1.write(sendMsg[i]);
    }
    //delayMicroseconds(bitLen);
    //digitalWrite(TxControlMixer, RS485Receive);  // Enable RS485 Transmit
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
