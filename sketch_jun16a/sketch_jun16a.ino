#include <Wire.h>

#include <Blynk.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
 


char auth[] = "ecvqWSNGw-7edDuq4b-luN_uCbqbkxJu";       
char ssid[] = "Luan";                      
char pass[] = "79797979";
 
#define inSensor 14 //D5
#define outSensor 12 //D6
 
int inStatus;
int outStatus;
 
int countin = 0;
int countout = 0;
 
int in;
int out;
int now;
 
#define relay 0 //D3
WidgetLED light(V0);
 
void setup()
{
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  delay(1000);  
  
  pinMode(inSensor, INPUT);
  pinMode(outSensor, INPUT);
  pinMode(relay, OUTPUT);
  digitalWrite(relay, HIGH);

  Serial.println("Visitor Counter Demo");
  
}
 
void loop()
{
  Blynk.run(); // Initiates Blynk
  inStatus =  digitalRead(inSensor);
  outStatus = digitalRead(outSensor);
  if (inStatus == 0)
  {
    in = countin++;
  }
 
  if (outStatus == 0)
  {
    out = countout++;
  }
 
  now = in - out;
 
  if (now <= 0)
  {
    digitalWrite(relay, HIGH);
    light.off();
    
  
  }
  else
  {
    digitalWrite(relay, LOW);
    light.on();
 
  
  }
  Blynk.virtualWrite(V0, relay);
  Blynk.virtualWrite(V7, in); // Visitors In
  Blynk.virtualWrite(V8, out); // Visitors Out
  Blynk.virtualWrite(V9, now); // Current Visitors
  delay(1000);
}
