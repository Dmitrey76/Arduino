int _gaugePins[10] = {10,9,8,7,6,5,4,3,2,1};
const int _gaugeSize = 10;

const int _relayPin = 0;
const int _sensorPin = 0;
const int _deltaPreasure = 10;

const unsigned long _delayShowTime = 3000;
const unsigned long _workTime =  30000;
const unsigned long _delayTime = 15000;

unsigned long startWork;
unsigned long showTime;

int preasureValue = 0; 
int prevPreasureValue = 0; 

int minPreasure = 100;
int maxPreasure = 135; //  из 1023

bool workInProgress = false;
bool repeatWork = false;

void setup() {

  int a;

  Serial.begin(9600);

  for (a = 0; a < _gaugeSize; a++) {
    pinMode (_gaugePins[a], OUTPUT);
    digitalWrite (_gaugePins[a], LOW);
  }

  pinMode (_relayPin, OUTPUT);
  digitalWrite (_relayPin, HIGH);

  startWork = 0;
  workInProgress = false;
  repeatWork = false;

}

void ShowPreasure (int value) {

  int sizeG = (maxPreasure - minPreasure);
  int stepG = sizeG / 10;
  int a;

  for (a = 0; a < _gaugeSize; a++) {
    
    digitalWrite (_gaugePins[a], ! (minPreasure + (a * stepG) >= value) );
    Serial.print ("Шкала: ");
    Serial.println (minPreasure + a * stepG);
  }

  showTime = millis ();
  
}

void loop() {

  prevPreasureValue = preasureValue;
  preasureValue = analogRead (_sensorPin);
  
//  Serial.print("Давление: ");
//  Serial.println(preasureValue);

  if ( abs(preasureValue - prevPreasureValue) > _deltaPreasure ) {
    ShowPreasure (preasureValue);
  };

  if ( ((preasureValue < minPreasure) || repeatWork) & !workInProgress & ((millis () - startWork) > _delayTime) ){
    digitalWrite (_relayPin, LOW);
    workInProgress = true;
    startWork = millis ();
    repeatWork = false;
    Serial.println ("on");
    Serial.println (preasureValue);
    Serial.println (millis () - startWork);
  };

  if (workInProgress & ((millis () - startWork) > _workTime)) {
    digitalWrite (_relayPin, HIGH);
    workInProgress = false;
    repeatWork = true;
    startWork = millis ();
    Serial.println (preasureValue);
    Serial.println ((millis () - startWork));
    Serial.println ("off - TO");
  };

  if (workInProgress & (preasureValue > maxPreasure)) {
    digitalWrite (_relayPin, HIGH);
    workInProgress = false;
    Serial.println (preasureValue);
    Serial.println ("off");
  };
  
  if (!workInProgress & !repeatWork & (millis() - showTime > _delayShowTime)) {
    ShowPreasure (0);
  };
  
  delay (1000);
  
}
