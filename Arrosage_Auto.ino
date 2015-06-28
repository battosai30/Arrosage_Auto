#include <CapTouch.h>


#define HUMIDITY P2_1
#define WATERLEVEL P2_3
#define WATERLEVELREF P2_2
#define LED1 P2_4
#define LED2 P2_5
#define PUMP P2_0

CapTouch humidity = CapTouch(HUMIDITY, TOUCH_PROXIMITY);
CapTouch waterLevel = CapTouch(WATERLEVEL, TOUCH_PROXIMITY);
CapTouch waterLevelRef = CapTouch(WATERLEVELREF, TOUCH_PROXIMITY);



void setup()
{
  Serial.begin(9600);
 // Serial.println("Hello");
  pinMode(LED1,OUTPUT);
  pinMode(LED2,OUTPUT);
  pinMode(PUMP,OUTPUT);
  digitalWrite(LED1,LOW);
  digitalWrite(LED2,LOW);
  digitalWrite(PUMP,HIGH);
  delay(4000);
  digitalWrite(PUMP,LOW);
}

void loop()
{
//  Serial.print("Humidity = ");
//  Serial.print(getTouch(&humidity));
//  Serial.print(" Waterlevel = ");
//  Serial.print(getTouch(&waterLevel));
//  Serial.print(" WaterlevelRef = ");
//  Serial.println(getTouch(&waterLevelRef));
Serial.print("X,");
Serial.print(getTouch(&humidity));
Serial.print(",");
Serial.print(getTouch(&waterLevel));
Serial.print(",");
Serial.print(getTouch(&waterLevelRef));
Serial.print(",");
Serial.flush();
Serial.println(getBatteryLevel());
}

long getTouch(CapTouch* toMeasure) {
  
  long measured=0;
  
  for(int i=0;i<10;i++)  {
  toMeasure->isTouched(); 
measured+= toMeasure->getMeasured();
}
// return 1000-measured/50;
return measured;
}

byte getBatteryLevel() {
  
  analogReference(INTERNAL2V5);
  sleep(10);
  analogRead(128 + 11);
  sleep(10);
  return  constrain(((((analogRead(128 + 11)+analogRead(128 + 11))/2.0)*2.5/1024.0*2)-3.4)/(4.1-3.4)*100,0,100);
  
}
