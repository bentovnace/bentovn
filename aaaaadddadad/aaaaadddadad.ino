#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); 
#define sensorPin1 26
#define sensorPin2 27
#define relay 7

int sensorState1 = 0;
int sensorState2 = 0;
int count=0;


//----------------------------------------------------------------------------------------------------------------------------------------------------




//Khai báo thư viện cho servo và khai báo các chân cắm 
#include <Servo.h>

Servo myservo;
int goc = 0;
int lightPin = A0;


//-----------------------------------------------------------------------------------------------------------------------------------------------------


void setup()
{


  //----------------------------------------------------------------------------------------------------


 myservo.attach(9);


//--------------------------------------------------------------------------------------------------



  pinMode (sensorPin1, INPUT_PULLUP);
  pinMode (sensorPin2, INPUT_PULLUP);
  pinMode(relay, OUTPUT);
  
  lcd.begin(16,2);
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("THPT MacDinhChi");
   lcd.setCursor(0,1);
  lcd.print("Khong Co Nguoi   ");
  delay(200);
}

void loop()
{  
  sensorState1 = digitalRead(sensorPin1);
  sensorState2 = digitalRead(sensorPin2);

  if(sensorState1 == LOW){
    count++; 
    delay(500);
  }

  if(sensorState2 == LOW){
    count--; 
    delay(500);
  }
  

   if(count<=0)
  {
    digitalWrite(relay, HIGH); 
    lcd.setCursor(0,1);
    lcd.print("Khong Co Nguoi   ");
  }
  else if (count>0 && count<10){
    digitalWrite(relay, LOW);
    lcd.setCursor(0,1);
    lcd.print("So Nguoi:   ");
    lcd.setCursor(12,1);
    lcd.print(count);
    lcd.setCursor(13,1);
    lcd.print("  "); 
    
  }
  else {
    digitalWrite(relay, LOW);
    lcd.setCursor(0,1);
    lcd.print("So Nguoi:   ");
    lcd.setCursor(12,1);
    lcd.print(count);  
  }





   //----------------------------------------------------------------------------------------------------
  
   
  int lightLevel =analogRead(lightPin);
  lightLevel = map(lightLevel, 0, 1023, 0, 179);
  goc = constrain(lightLevel, 0, 179);
 // Serial.println(goc);
  myservo.write(goc);
  delay(100);


//-------------------------------------------------------------------------------------------------------
  
}
