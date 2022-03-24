#include <Arduino.h>

//pin du capteur ultrasons
#define echoPinR 11
#define trigPinR 10

#define echoPinL 0
#define trigPinL 1

//Pins des moteurs
#define MOT2CW 6
#define MOT2CCW 7
#define MOT2PWN 4

#define MOT1CW 8
#define MOT1CCW 9
#define MOT1PWN 5


//Déclaration des fonctions
int getProxValue(int);

void setup() {
  //Serial.begin(9600); // // Serial Communication is starting with 9600 of baudrate speed
  
  //Pins pour les capteurs
  pinMode(trigPinR, OUTPUT); 
  pinMode(echoPinR, INPUT); 
  pinMode(trigPinL, OUTPUT); 
  pinMode(echoPinL, INPUT); 

  digitalWrite(trigPinR, LOW);
  digitalWrite(trigPinL, LOW);


  // Mettre tous les pins qu'on aura besoin en output
  // Pas besoin de définir le mode des pins PWN
  pinMode(MOT2CW,OUTPUT);
  pinMode(MOT2CCW,OUTPUT);
  pinMode(MOT1CW,OUTPUT);
  pinMode(MOT1CCW,OUTPUT);

  // Allumer le premier moteur et lui donner une vitesse avec le PWN
  digitalWrite(MOT2CW,LOW);
  digitalWrite(MOT2CCW,HIGH);
  analogWrite(MOT2PWN,255);
  
  // désactiver le deuxième moteur
  digitalWrite(MOT1CW,HIGH);
  digitalWrite(MOT1CCW,LOW);
  analogWrite(MOT1PWN,255);

}


void loop() {
  /*
  Serial.println(getProxValue(1));
  Serial.println(getProxValue(2));
  delay(100);
  */

  if(getProxValue(1)>500)
  {
    digitalWrite(MOT1CW,LOW);
    digitalWrite(MOT1CCW,HIGH);
    analogWrite(MOT1PWN,255);
  }
  else
  {
    digitalWrite(MOT1CW,HIGH);
    digitalWrite(MOT1CCW,LOW);
    analogWrite(MOT1PWN,255);
  }

  if(getProxValue(2)>500)
  {
    digitalWrite(MOT2CW,LOW);
    digitalWrite(MOT2CCW,HIGH);
    analogWrite(MOT2PWN,255);
  }
  else
  {
    digitalWrite(MOT2CW,HIGH);
    digitalWrite(MOT2CCW,LOW);
    analogWrite(MOT2PWN,255);
  }
  //delay(100);
} 

// For right prox c'est 1 et pour le gauche c'est 2
int getProxValue(int prox)
{
  int trigPin;
  int echoPin;
  int duration;

  if(prox == 1)
  {
    trigPin = trigPinR;
    echoPin = echoPinR;
  }
  else if(prox == 2)
  {
    trigPin = trigPinL;
    echoPin = echoPinL;
  }
  // Clears the trigPin condition
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  /*
  Serial.print("Duration for ");
  Serial.print(prox);
  Serial.print(" : ");
  Serial.println(duration);
  */
  return duration;

}