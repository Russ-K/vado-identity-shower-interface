long baudRate = 9600;
byte serialConfig = SERIAL_8N2;

#define TxControlMixer      3   //RS485 Direction control
#define RS485Transmit       HIGH
#define RS485Receive        LOW

const int nMsgLen = 7;
byte receivedData[nMsgLen];

const long unsigned DEDICATED_READ_TIME = 20;
long unsigned msReadEnd = 0;

void setup() {
  Serial.begin(baudRate, serialConfig);
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
