#include <LiquidCrystal_I2C.h>

int RelaysPin[16] = {36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 03, 49, 03, 48};
int KeysPin[14] =   {22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35};
int States[16] = {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH};
unsigned long Delays[16];
unsigned long showActionDelay;

const int _moveDetectLevel = 160;
const int _mainDelayDefault = 50;
const unsigned long _delayMotionTime = 5000;
const unsigned long _delayActionTime = 600000;

const int _pinMotion = 1;
const int _pinMotionRelay = 36; 

unsigned long detectMotionTime = 0;
unsigned long detectAction;

int mainDelay = 50; 

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

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {

  int a;
  int cnt;

  Serial.begin(9600);

  detectAction = millis();

  lcd.init();
  lcd.backlight();
  lcd.clear ();
  lcd.print ("Готов");

  lcd.createChar(0, lightOff);
  lcd.createChar(1, lightOn);

  cnt = sizeof(RelaysPin) / sizeof (int);

  for (a = 0; a < cnt; a++) {
    pinMode (RelaysPin[a], OUTPUT);
    digitalWrite (RelaysPin[a], HIGH);
    Serial.print(a);
    Serial.print(" ");
  }

  cnt = sizeof(KeysPin) / sizeof (int);

  for (a = 0; a < cnt; a++) {
    pinMode (KeysPin[a], INPUT);
    Serial.print(a);
    Serial.print(" ");
  }

  cnt = sizeof(Delays) / sizeof (unsigned long);
  showActionDelay = millis();

  for (a = 0; a < cnt; a++) {
    Delays[a] = millis();
  }

  lcd.noBacklight();

}

void checkDelay(int pinNo, unsigned long* detectTime, bool isHigh = false)
{
	if (isHigh) {
		*detectTime = millis() + _delayMotionTime;
		digitalWrite(pinNo, HIGH);
	}
	else
	{
		if (millis() > *detectTime) {
			digitalWrite(pinNo, LOW);
		}
	}

}

bool checkAnalogState(int pinRead, int analogLevel = _moveDetectLevel) {



	bool result;

	result = bool(analogRead(pinRead) > analogLevel);

	if (result) {
		detectAction = millis();
	}

	return (result);

};

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

  }
}

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
	  detectAction = millis();
      setKeyState (RelaysPin[a], &States[a], &Delays[a]);
    };
  };
  
  checkDelay(_pinMotionRelay, &detectMotionTime, checkAnalogState(_pinMotion));

  if ((millis() > detectAction)&((millis() - detectAction) > _delayActionTime)) {
	  if (mainDelay < 1000) {
		  mainDelay = int((millis() - detectAction) / _delayActionTime) * _mainDelayDefault;
	  }
  }
  else {
	  mainDelay = _mainDelayDefault;
  }

  delay(mainDelay);

}
