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

byte message[nMsgLen];

int nMsg = 0;

void setup() {
  Serial1.begin(baudRate, serialConfig);

  // Init Transceiver
  pinMode(TxControlMixer, OUTPUT);
  digitalWrite(TxControlMixer, RS485Receive);
}

void loop()
{
  curMicros = micros();
  if ((curMicros > (lastCom + comPeriod)) || (curMicros < lastCom)) {
    switch (nMsg) {
      case 0:
        memcpy(message, messageIntro, nMsgLen);
        break;
      case 1:
        memcpy(message, messageStartup1, nMsgLen);
        break;
      case 2:
        memcpy(message, messageStartup2, nMsgLen);
        break;
      case 3:
        memcpy(message, messageHearbeat, nMsgLen);
        break;
      case 4:
        memcpy(message, messageRest, nMsgLen);
        break;
      default:
        memcpy(message, messageIntro, nMsgLen);
        break;
    }

    ++nMsg;
    if (nMsg > 4) {
      nMsg = 0;
    }

    lastCom = curMicros;
    digitalWrite(TxControlMixer, RS485Transmit);  // Enable RS485 Transmit
    for (int i = 0; i < sizeof(message); ++i) {
      Serial1.write(message[i]);
      //delayMicroseconds(bitLen);
    }
    //delayMicroseconds(bitLen);
    //digitalWrite(TxControlMixer, RS485Receive);  // Enable RS485 Transmit
  }
}
