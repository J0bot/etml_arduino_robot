#include <Arduino.h>
#include <WiFiNINA.h>
#include <aREST.h>

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

aREST rest = aREST();

char ssid[] = "NETGEAR96";      // your network SSID (name)
char pass[] = "freshviolin032";   // your network password
int keyIndex = 0;                 // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;

int running = 1;

unsigned long currentMillis = millis();
unsigned long interval = 1000;
unsigned long previousMillis = 0;
int running1 = 1;
int running2 = 1;

WiFiServer restServer(80);

//Déclaration des fonctions
int getProxValue(int);
void runWithProx();
void justRun();
void setupAPI();
void runWithAPI();
void printWifiStatus();
int stopMotors(String);
int startMotors(String);
int startFunction(String);
int setRunning(String);


void setup() {
  Serial.begin(9600); // // Serial Communication is starting with 9600 of baudrate speed
  
  //Pins pour les capteurs
  pinMode(trigPinR, OUTPUT); 
  pinMode(echoPinR, INPUT); 
  pinMode(trigPinL, OUTPUT); 
  pinMode(echoPinL, INPUT); 

  digitalWrite(trigPinR, LOW);
  digitalWrite(trigPinL, LOW);

  // Mettre tous les pins qu'on aura besoin en output
  pinMode(MOT2CW,OUTPUT);
  pinMode(MOT2CCW,OUTPUT);
  pinMode(MOT1CW,OUTPUT);
  pinMode(MOT1CCW,OUTPUT);
  
  setupAPI();
}


void loop() {
  runWithAPI();
} 

void runWithProx()
{
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
}



void justRun()
{
  digitalWrite(MOT2CW,LOW);
  digitalWrite(MOT2CCW,HIGH);
  analogWrite(MOT2PWN,255);
  digitalWrite(MOT1CW,LOW);
  digitalWrite(MOT1CCW,HIGH);
  analogWrite(MOT1PWN,255);


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

  return duration;

}

void setupAPI()
{
  //Rest stuff
  rest.function("start",startMotors);
  rest.function("stop",stopMotors);
  rest.function("function",startFunction);
  rest.function("set_running",setRunning);
  rest.variable("running",&running);
  // Give name and ID to device
  rest.set_id("008");
  rest.set_name("arduino_machine");

  rest.function("set_running",setRunning);
  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue:
    while (true);
  }

  // attempt to connect to Wifi network:
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(5000);
  }
  printWifiStatus();

  // Start du serveur
  restServer.begin();
}



void runWithAPI()
{
  // Handle REST calls
  WiFiClient client = restServer.available();
  rest.handle(client);

  if(running==1){
    startFunction(" ");
  }

}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  IPAddress subnet = WiFi.subnetMask();
  Serial.print("Netmask: ");
  Serial.println(subnet);

  IPAddress gateway = WiFi.gatewayIP();
  Serial.print("Gateway: ");
  Serial.println(gateway);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

int stopMotors(String command){
  analogWrite(5, 0);
  analogWrite(4, 0);
  return 0;
}

int startMotors(String command){
  analogWrite(5, 255);
  analogWrite(4, 255);
  return 0;
}

int startFunction(String command)
{
  
  unsigned long currentMillis = millis();

  if(currentMillis - previousMillis >= interval) {
  // save the last time you blinked the LED
    previousMillis = currentMillis;

    if(running1 ==1 && running2 == 1)
    {
      digitalWrite(MOT2CW,LOW);
      digitalWrite(MOT2CCW,HIGH);
      digitalWrite(MOT1CW,LOW);
      digitalWrite(MOT1CCW,HIGH);
      running1 = 2;
      running2 = 2;
    }
    else{
      digitalWrite(MOT2CW,HIGH);
      digitalWrite(MOT2CCW,LOW);
      digitalWrite(MOT1CW,HIGH);
      digitalWrite(MOT1CCW,LOW);
      running1 = 1;
      running2 = 1;
    }
  }


  return 0;
}


int setRunning(String command){
  running = command.toInt();
  Serial.print("Value : ");
  Serial.println(command);
  Serial.println(running);
  return command.toInt();
}