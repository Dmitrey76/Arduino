const int _moveDetectLevel = 160;
const int _mainDelayDefault = 50;
const unsigned long _delayMotionTime = 1000;
const unsigned long _delayActionTime = 3000;
const int _pinMotion = 1;

unsigned long detectMotionTime = 0;
unsigned long detectAction;

int mainDelay = 50;


void setup()
{

	pinMode(1, OUTPUT);
	pinMode(2, INPUT);
	Serial.begin(9600);
	detectAction = millis();
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

void loop()
{

	checkDelay(_pinMotion, &detectMotionTime, checkAnalogState(_pinMotion));

	if ((millis() > detectAction)&((millis() - detectAction) > _delayActionTime)) {
		
		if (mainDelay < 1000) {
			mainDelay = int((millis() - detectAction) / _delayActionTime) * _mainDelayDefault;
		}

	}
	else {
		mainDelay = _mainDelayDefault;
	}
	
	Serial.println(mainDelay);

	delay(mainDelay);

}

