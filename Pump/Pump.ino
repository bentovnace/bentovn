//Khai báo thư viện servo
#include <Servo.h>
  //Tạo biến servo
 Servo myservo;
 //khai báo các biến dữ liệu và chân cắm
 const int ldrpin = A0; //Chân đọc dữ liệu của cảm biến ánh sáng là chân analog số 0
 const int servopin = 5
 ; //Chân kích xung để hoạt động servo là chân digital 8 trên board arduino

 int ldrvalue; //Khai báo kiểu dữ liệu cho cảm biến ánh sáng
 int goc = 0; //Khai báo kiểu dữ liệu cho biến góc ( chỉ góc quay của Servo )
float d;
 void setup()
 {
  Serial.begin(9600); // Mở cổng kết bối với aruduino
 myservo.attach ( servopin ); // Cổng kết nối của servo
  //------------------------------------------------------------------
  pinMode(7,INPUT);//Chân Echo của ultrasonic Hc-sr04  
 pinMode(8,OUTPUT);//Chân Trig của ultrasonic Hc-sr04  
 pinMode(10,OUTPUT);// relay
 pinMode(9,OUTPUT);// buzzer(đèn led) 
 //------------------------------------------------------------------
 }
 int low=23;
int high=5;

void vol() //Tính toán khoảng cách cho HC Sr 04
{
 digitalWrite(8,HIGH);
 delayMicroseconds(8);
 digitalWrite(8,LOW);
 delayMicroseconds(2);
 d=pulseIn(7,HIGH);
 d=d/69;
}
//---------------------------------------------------------------
 void loop ()
{ //-----------------------------------------------------------------------------------------------
  
  ldrvalue = analogRead(ldrpin); // Đọc dữ liệu của cảm biến ánh sáng ở chân analog 0 
  goc = map ( ldrvalue, 0, 1023, 0, 180); //Giới hạn giá trị của cảm biến ánh sáng nằm trong khoảng từ 180 - 1023
  Serial.println(goc);
  myservo.write(goc);
  delay (15);

  
  //---------------------------------------------------------------------------------------------
  vol();
 while(1)
  {
   b:
   digitalWrite(10,HIGH);// Bật Bơm...
   delay(2000);
   vol();
   if(d<high) //Kiểm tra HIGH
    {
     digitalWrite(9,HIGH);// Bật buzzer.....
     delay(1000);
     digitalWrite(9,LOW);
     goto a;
    }
  }
 while(1)
  {
   a:
   digitalWrite(10,LOW);// Tắt bơm...
   delay(100);
   vol();
   if(d>low) //Kiểm tra LOW
    {
     digitalWrite(9,HIGH);//Bật led hoặc buzzer
     delay(1000);
     digitalWrite(9,LOW);
     delay(1000);
     digitalWrite(9,HIGH);
     delay(1000);
     digitalWrite(9,LOW);
     delay(1000);
     
     goto b;
    }
}
}
