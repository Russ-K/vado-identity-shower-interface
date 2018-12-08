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
//Outgoing register
byte message[MSG_LEN];
//Outgoing message details
/////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////
//Controller message details
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
  }
}

void PrintData() {
  for(int i = 0; i < MSG_LEN; i++)
  {
    Serial.print(receivedData[i], HEX);
    Serial.print(" ");
  }
  Serial.println("");
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
