
 #include<Servo.h>
Servo s;
#define OutputDO 8
int Ldr = A1;   
int ldr;
int en1= 6; 
int en2= 5; 
int in1= 2;  
int in2= 12; 
int in3= 11;  
int in4= 10; 

int OutputAO=A0; 
int Led=13; 


int val;
float sensor; 
// Cài đặt các đầu ra 
void setup() {
  s.attach(9);
   pinMode(en1, OUTPUT); 
  pinMode(en2, OUTPUT); 
  pinMode(in1, OUTPUT); 
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT); 
  pinMode(in4, OUTPUT); 

pinMode (Led, OUTPUT) ; 
pinMode (OutputDO, INPUT) ; 
pinMode (OutputAO, INPUT) ; 
pinMode (ldr, INPUT);


Serial.begin(9600);
  
}
// Trường hợp Mưa
void rain(){
  digitalWrite (Led, HIGH);

  analogWrite(en1,125);
  digitalWrite(in1,LOW);
  digitalWrite(in2,HIGH);

  analogWrite(en2,125);
  digitalWrite(in3,LOW);
  digitalWrite(in4,HIGH);

  Serial.println("Rain");
  
}
void cloud(){
  digitalWrite (Led, HIGH);

  analogWrite(en1,125);
  digitalWrite(in1,LOW);
  digitalWrite(in2,HIGH);

  analogWrite(en2,125);
  digitalWrite(in3,LOW);
  digitalWrite(in4,HIGH);

  Serial.println("Rain");
}
void light(){
  digitalWrite (Led, LOW);

  analogWrite(en1,125);
  digitalWrite(in1,HIGH);
  digitalWrite(in2,LOW);

  analogWrite(en2,125);
  digitalWrite(in3,HIGH);
  digitalWrite(in4,LOW);

  Serial.println("light");
}
void energy(){
  analogWrite(en1,0);
  digitalWrite(in1,LOW);
  digitalWrite(in2,LOW);

  analogWrite(en2,0);
  digitalWrite(in3,LOW);
  digitalWrite(in4,LOW);

  Serial.println("energy");
  delay(5000);
  
}


void loop() {
  
  int value= analogRead(A0);
  if(value<395){
    s.write(180);
  }
  else{
    s.write(0);
     





sensor = analogRead(OutputAO);
Serial.println(sensor);
ldr = analogRead(A1);
Serial.print("ldr : ");
Serial.println(ldr);

val = digitalRead (OutputDO);


 if (ldr <= 250 )  
 {
  if (sensor <=500)
   
   
    rain(); delay(500);
    energy(); delay(5000);
   
 }

 else if(ldr <= 250 ){
  if (sensor > 500){
   cloud(); delay(400);
   energy(); delay(5000);
 }
 }
 
else if(ldr >= 250 ){
  if (sensor > 500){
   light(); delay(1000);
   energy(); delay(5000);
 }
 }
}
}
