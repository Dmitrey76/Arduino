#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 10 // номер пина к которому подключен DS18B20
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void setup()
{
  sensors.begin(); 
  Serial.begin(9600);
}
void loop()
{
sensors.requestTemperatures();
Serial.print (sensors.getTempCByIndex(0));
delay(100);


}
