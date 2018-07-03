#include <LiquidCrystal_I2C.h>

const int pinLightSensor = 1;
const int _moveDetectLevel = 500;

int RelaysPin[16] = {36,37,38,39,40,41,42,43,44,45,46,47,03,49,03,48};
int KeysPin[14] =   {22,23,24,25,26,27,28,29,30,31,32,33,34,35};
int States[16] = {HIGH,HIGH,HIGH,HIGH,HIGH,HIGH,HIGH,HIGH,HIGH,HIGH,HIGH,HIGH,HIGH,HIGH,HIGH,HIGH};
unsigned long Delays[16];
unsigned long showActionDelay;

int stateSensor_Light = HIGH;

unsigned long delaySensor_Light = millis();

uint8_t lightOn[8] = {
	0b11111,
	0b01110,
	0b00000,
	0b01010,
	0b00100,
	0b01010,
	0b00000,
	0b11111
};
uint8_t lightOff[8] = {
	0b11111,
	0b01110,
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b11111
};

LiquidCrystal_I2C lcd(0x27,16,2);
=======
/*byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
  char server[] = "192.168.1.39";
  byte bufTCP[3];

  IPAddress ip (192, 168, 1, 177);
  IPAddress serverIp (192, 168, 1, 39);

  EthernetServer ethServer(738);
  EthernetClient ethClient;*/

struct TSendData {
  byte btOperation;
  byte btDestination;
  byte btState;
};

LiquidCrystalRus_I2C lcd(0x27, 16, 2);
>>>>>>> efc6a0ceae168408ae5d91f918200aff270d1aae

void setup() {

  int a;
  int cnt;

  Serial.begin(9600);

  lcd.init();
  lcd.backlight();
  lcd.clear ();
  lcd.print ("Готов");

  lcd.createChar(0, lightOff);
  lcd.createChar(1, lightOn);

  cnt = sizeof(RelaysPin) / sizeof (int);

  Serial.print("Инициализация реле: ");

  for (a = 0; a < cnt; a++) {
    pinMode (RelaysPin[a], OUTPUT);
    digitalWrite (RelaysPin[a], HIGH);
    Serial.print(a);
    Serial.print(" ");
  }

  Serial.println(" OK");

  Serial.print("Инициализация выключателей: ");

  cnt = sizeof(KeysPin) / sizeof (int);

  for (a = 0; a < cnt; a++) {
    pinMode (KeysPin[a], INPUT);
    Serial.print(a);
    Serial.print(" ");
  }

  Serial.println(" OK");

  cnt = sizeof(Delays) / sizeof (unsigned long);
  showActionDelay = millis();

  for (a = 0; a < cnt; a++) {
    Delays[a] = millis();
  }


<<<<<<< HEAD

  lcd.noBacklight();


=======
  /*lcd.createChar(3, heart);  // создаём символ «сердце» в 3 ячейке памяти
    lcd.printByte(3); // печатаем символ «сердце», находящийся в 3-ей ячейке*/

  lcd.noBacklight();

  /*  Ethernet.begin(mac, ip);
    ethServer.begin();

    ethClient.connect (serverIp, 738);

    if (ethClient.connected ()) {
    }

    ethClient.write(bufTCP, 3);

    Serial.println(Ethernet.localIP());*/
>>>>>>> efc6a0ceae168408ae5d91f918200aff270d1aae

}

void ShowState (int keyNo = -1) {

  if (showActionDelay > millis()) {
    return;
  }


  lcd.clear ();
  char messageText[16];

  if (keyNo >= 0) {

    switch (keyNo)
    {
      case 36 : strcpy(messageText, "Кухня");
      case 37 : strcpy(messageText, "Гостинная");
      case 38 : strcpy(messageText, "Тамбур");
      case 39 : strcpy(messageText, "Сени");
      case 40 : strcpy(messageText, "Туалет");
      case 41 : strcpy(messageText, "Коридор 1эт.");
      case 42 : strcpy(messageText, "Лестница");
      case 43 : strcpy(messageText, "Коридор 2эт.");
      case 44 : strcpy(messageText, "Кабинет");
      case 45 : strcpy(messageText, "Спальня");
      case 46 : strcpy(messageText, "Детская");
      case 47 : strcpy(messageText, "Балкон");
      case 03 : strcpy(messageText, "Гардероб");
      case 49 : strcpy(messageText, "1?");
      case 48 : strcpy(messageText, "2?");

      default:
        break;
    }

    lcd.setCursor(0, 1);
    lcd.print(messageText);
    lcd.setCursor(0, 0);
    lcd.print("Переключение:");

    showActionDelay = millis() + 5000;
    lcd.backlight();

  }
  else {

    int a;
    int cnt;
    int onCount = 0;

    lcd.noBacklight();

    cnt = sizeof(States) / sizeof(int);

    for (a = 0; a < cnt; a++) {
      lcd.setCursor(a, 0);
      lcd.write(1);
      if (States[a] == LOW) {
      }
      else {
        lcd.write(1);
        onCount++;
      }

    }

    lcd.setCursor(0, 0);
    lcd.print("Потребителей: ");
    lcd.print(onCount);

<<<<<<< HEAD
=======
    /*lcd.setCursor (0,1);
      lcd.print ("Temp");
      lcd.setCursor (5,1);
      lcd.print ("21.4");
      lcd.setCursor (11,1);
      lcd.print ("15:44");*/
>>>>>>> efc6a0ceae168408ae5d91f918200aff270d1aae
  }
}

bool checkAnalogState (int pinRead, unsigned long keyDelay, int analogLevel = _moveDetectLevel) {

  int curRead = 0;
  bool timeOut = false;

  curRead = analogRead (pinRead);
  timeOut = ((millis() - keyDelay) > 5000);

  return (bool(curRead > analogLevel)&timeOut);

};

bool checkKeyState (int pinRead, unsigned long keyDelay) {

  int curRead = 0;
  bool timeOut = false;

  curRead = digitalRead (pinRead);
  timeOut = ((millis() - keyDelay) > 1000);

  return (bool(curRead)&timeOut);

};

void setKeyState (int pinWrite, int* stateKey, unsigned long* keyDelay) {

  *stateKey = !(*stateKey);
  digitalWrite(pinWrite, *stateKey);
  *keyDelay = millis ();
  ShowState ();

}

void loop() {

  int a;
  int cnt;

  cnt = sizeof(KeysPin) / sizeof (int);

  for (a = 0; a < cnt; a++) {
    if (checkKeyState (KeysPin[a], Delays[a]) == true) {
      setKeyState (RelaysPin[a], &States[a], &Delays[a]);
    };
  };

  /*Serial.println(analogRead(1));

    if (checkKeyState (pinKey_Kitcher, delayKey_Kitcher) == true) {
      Serial.println(stateKey_Kitcher);
      setKeyState (pinRelay_Kitcher, &stateKey_Kitcher, &delayKey_Kitcher);
      Serial.println(stateKey_Kitcher);
      Serial.println("key");
    }

    if (checkAnalogState (pinLightSensor, delaySensor_Light) == true) {
      setKeyState (pinRelay_Kitcher, &stateSensor_Light, &delaySensor_Light);
      stateSensor_Light = HIGH;
      stateKey_Kitcher = LOW;
      Serial.println("sensor");
    }

    EthernetClient client = ethServer.available();

    if (client) {

    Serial.println("4");

      Serial.println("new client: ");

      while (client.connected()) {

    Serial.println("5");

        if (client.available() == sizeof (TSendData)) {

    Serial.println("6");

          int cnt = client.read(bufTCP, sizeof (TSendData));
          Serial.println(bufTCP[0], DEC);
          Serial.println(bufTCP[1], DEC);
          Serial.println(bufTCP[2], DEC);

        };

      };

    };*/

  delay (30);

}
