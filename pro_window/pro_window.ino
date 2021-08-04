#include <Servo.h>
Servo myservo;
void setup()  {
  myservo.attach(9);
  Serial.begin  (9600);
  pinMode (12, OUTPUT);
}
void loop() {
  int sensorValue = analogRead(A0);
  Serial.print (sensorValue);
  delay(100);
  if (sensorValue <500){
    Serial.println(" = LEDUP");
    digitalWrite(12, LOW);
    myservo.write(0);
  }
  else{
    Serial.println(" = LIGHT");
    digitalWrite(12, HIGH);
    myservo.write (180);
  }
}
