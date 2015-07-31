#include <CapTouch.h>

#define HUMIDITY P2_1 // pin sur lequel on mesure l'humidité de la terre
#define WATERLEVEL P2_3 // pin sur lequel on va mesurer le niveau d'eau
#define WATERLEVELREF P2_2 // pin sur lequel on va mesurer la reference pour le calcul de niveau d'eau

#define LED1 P2_4
#define LED2 P2_5

#define PUMP P2_0

#define DEBUG // à commenter si on ne veut pas débuguer

#define TIME_PUMP 5000 // durée des "injections" d'eau
#define H_THRESHOLD 30000 // limite d'humidité, à adapté à votre montage

// instanciation des capteurs touch
CapTouch humidity = CapTouch(HUMIDITY, TOUCH_PROXIMITY);
CapTouch waterLevel = CapTouch(WATERLEVEL, TOUCH_PROXIMITY);
CapTouch waterLevelRef = CapTouch(WATERLEVELREF, TOUCH_PROXIMITY);

// initialisation du compteur
char count=0;

void setup()
{

#ifdef DEBUG
  delay(1000);
  Serial.begin(9600);
  Serial.println("Hello");
#endif

  pinMode(LED1,OUTPUT);
  pinMode(LED2,OUTPUT);
  pinMode(PUMP,OUTPUT);
  digitalWrite(LED1,HIGH);
  digitalWrite(LED2,HIGH);
  delay(1000)
  digitalWrite(LED1,LOW);
  digitalWrite(LED2,LOW);
  digitalWrite(PUMP,LOW);
}

void loop()
{
  // mesure des différentes capacités
  long soilH=getTouch(&humidity);
  long waterRef=getTouch(&waterLevelRef);
  long water=getTouch(&waterLevel);
  
  byte batLvl=getBatteryLevel(); // mesure du niveau de la batterie
  
  int waterRatio=water*100/waterRef;
  
  // si l'humidité est trop faible on incrémente le compteur
  if(soilH>H_THRESHOLD) {
    if(++count>100) count=100;
  } else {
    if(--count<0) count=0;
  }
  
  //si mon compteur dépasse 50, que j'ai de l'eau dans le 
  //réservoir et que le niveau de batterie est suffisant, j'injecte de l'eau
  if(count>50 && waterRatio<60 && batLvl>20) {
   count=0;
  digitalWrite(PUMP,HIGH);
  sleep(TIME_PUMP);
  digitalWrite(PUMP,LOW);
  }
  
  // pas assez d'eau, ça clignote
  if(waterRatio>60){
    flashLed(0);
  }
   // pas assez de batterie, ça clignote
  if(batLvl<20){
    flashLed(1);
  }
  
#ifdef DEBUG
  Serial.print("X,");
  Serial.print(soilH);
  Serial.print(",");
  Serial.print(water);
  Serial.print(",");
  Serial.print(waterRef);
  Serial.print(",");
  Serial.print(waterRatio);
  Serial.print(",");
  Serial.flush();
  Serial.print(count,DEC);
  Serial.print(",");
  Serial.flush();
  Serial.println(batLvl);
  Serial.flush();
#endif


}

long getTouch(CapTouch* toMeasure) {

  long measured=0;

// moyenne sur 10 mesures
  for(int i=0;i<10;i++)  {
    toMeasure->isTouched(); 
    measured+= toMeasure->getMeasured();
  }

  return measured/10;
}

byte getBatteryLevel() {

// on bascule sur la ref interne à 2,5V
  analogReference(INTERNAL2V5);
  sleep(10);
  // sur energia, ajouter 128 permet de choisir directement l'entrée du multiplexeur. Sur
  // ce µC, le canal 11 est pour VCC/2
  analogRead(128 + 11);
  sleep(10);
  // grosse fonction batarde tout en un, moyenne de deux mesure et 
  // mise à échelle de 0 à 100% pour une batterie au lithium
  return  constrain(((((analogRead(128 + 11)+analogRead(128 + 11))/2.0)*2.5/1024.0*2)-3.4)/(4.1-3.4)*100,0,100);

}

void flashLed(byte LED){
 
  digitalWrite(LED?LED1:LED2,HIGH);
  sleep(50);
  digitalWrite(LED?LED1:LED2,LOW);
  
}
