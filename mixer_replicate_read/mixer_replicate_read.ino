long baudRate = 8752;
byte serialConfig = SERIAL_8N1;

#define TxControlMixer      3   //RS485 Direction control
#define RS485Transmit       HIGH
#define RS485Receive        LOW

long unsigned lastCom = 0;
long unsigned curMicros = 0;
const unsigned long comPeriod = 55000;
const int bitLen = 10;

const int nMsgLen = 7;
const byte messageIntro[nMsgLen] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
const byte messageStartup1[nMsgLen] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x21};
const byte messageStartup2[nMsgLen] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x53};
const byte messageHearbeat[nMsgLen] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x16};
const byte messageRest[nMsgLen] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x51};

byte receivedData[nMsgLen];

byte message[nMsgLen];

int nMsg = 0;

const long unsigned DEDICATED_READ_TIME = 20;
long unsigned msReadEnd = 0;

void setup() {
  Serial.begin(9600, serialConfig);
  Serial1.begin(baudRate, serialConfig);

  // Init Transceiver
  pinMode(TxControlMixer, OUTPUT);  
  digitalWrite(TxControlMixer, RS485Receive);
}

void loop()
{
  if (ReadData(receivedData)) {
    for(int i = 0; i < sizeof(receivedData); i++)
    {
      Serial.print(receivedData[i], HEX);
      Serial.print(" ");
    }
    Serial.println("");
  }
}

bool ReadData(byte pData[]) {
  bool foundData = false;
  if (Serial1.available() > 0) {
    msReadEnd = millis() + DEDICATED_READ_TIME;
  
    int nCurByte = 0;
    while (msReadEnd > millis()) {
      if (Serial1.available() > 0) {
        // read the incoming byte:
        pData[nCurByte++] = Serial1.read();
      }

      if (nCurByte >= nMsgLen) {
        nCurByte = 0;
        foundData = true;
      }
    }
  }
  return foundData;
}
