
#include <Servo.h>
Servo myservo;
int pos = 0;
int lightPin = A0;

void setup() {
  myservo.attach(8);
}


void loop()   {
  int lightLevel =analogRead(lightPin);
  lightLevel = map(lightLevel, 0, 1023, 0, 179);
  pos = constrain(lightLevel, 0, 179);

  myservo.write(pos);
  delay(100);


}
