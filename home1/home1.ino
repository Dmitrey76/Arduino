#include <LiquidCrystal_I2C.h>

int RelaysPin[16] = {36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 03, 49, 03, 48};
int KeysPin[14] =   {22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35};
int _MotionPinNo = 12;

int States[16] = {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH};
unsigned long Delays[16];
unsigned long showActionDelay;

const int _moveDetectLevel = 850;
const int _mainDelayDefault = 50;
const unsigned long _delayMotionTime = 5000;
const unsigned long _delayActionTime = 600000;

const int _pinMotion = 1;
const int _pinMotionRelay = 3; 

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
  lcd.print ("Ready");

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

  lcd.backlight();

}

void ShowState (int keyNo = -1) {

  lcd.clear ();
  lcd.setCursor(0, 1);
  lcd.print("Swth: ");
  lcd.print(keyNo);

  showActionDelay = millis() + 5000;
  lcd.backlight();

  int a;
  int cnt;
  int onCount = 0;

  cnt = sizeof(States) / sizeof(int);

  for (a = 0; a < cnt; a++) {
    lcd.setCursor(a, 0);

    if (States[a] == HIGH) {
      lcd.write(0);
    }
    else {
      lcd.write(1);
      onCount++;
    }
  }

  lcd.setCursor(9, 1);
  lcd.print("Dvs: ");
  lcd.print(onCount);

}

void checkDelay(int pinNo, unsigned long* detectTime, bool isHigh = false)
{

  int state = States[_MotionPinNo];
   
	if (isHigh) {
		*detectTime = millis() + _delayMotionTime;
		digitalWrite(pinNo, LOW);
    States[_MotionPinNo] = LOW;
    ShowState (pinNo);
    States[_MotionPinNo] = state;

	}
	else
	{
		if (millis() > *detectTime) {
			if (States[_MotionPinNo] == HIGH) {
       	digitalWrite(pinNo, HIGH);
			}  
		}
	}

}

bool checkAnalogState(int pinRead, int analogLevel = _moveDetectLevel) {

	bool result;

	result = bool(analogRead(pinRead) > analogLevel);
	if (States[_MotionPinNo] == LOW) {
	  result = false;
	}

	if (result) {
		detectAction = millis();
	}

	return (result);

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
  ShowState (pinWrite);

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
  
  if (checkAnalogState(_pinMotion)) {
    checkDelay(_pinMotionRelay, &detectMotionTime, true);
  }
  else
    checkDelay(_pinMotionRelay, &detectMotionTime);

  if ((millis() > detectAction)&((millis() - detectAction) > _delayActionTime)) {
	  if (mainDelay < 1000) {
		  mainDelay = int((millis() - detectAction) / _delayActionTime) * _mainDelayDefault;
	  }
  }
  else {
	  mainDelay = _mainDelayDefault;
  }

  if (showActionDelay < millis()) {
    lcd.noBacklight();
  }

  delay(mainDelay);

}
