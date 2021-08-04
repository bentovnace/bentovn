// include
#include <LiquidCrystal.h>
#include <Keypad.h>
#include <Servo.h>
#include <SPI.h>
#include <RFID.h>
 
// instance
LiquidCrystal lcd(42, 41, 35, 34, 33, 32); // LCD
Keypad myKeypad= Keypad(makeKeymap(keymap), rowPins, colPins, numRows, numCols); // Clavier
Servo myservo; // Serrure
RFID rfid(53, 5); //RFID
 
 
// variables
 
// Code bonne cle : 84, 6B, 49, 2E, 88
int serNum0=0x84;
int serNum1=0x6B;
int serNum2=0x49;
int serNum3=0x2E;
int serNum4=0x88;
 
// Clavier
const byte numRows= 4; 
const byte numCols= 4; 
char keymap[numRows][numCols]=
{
{'1', '4', '7', '*'},
{'2', '5', '8', '0'},
{'3', '6', '9', '#'},
{'A', 'B', 'C', 'D'}
};
byte colPins[numRows] = {29,28,27,26}; //Rows 0 to 3
byte rowPins[numCols]= {25,24,23,22}; //Columns 0 to 3
 
// Temoin
int rouge = 8;
int vert = 9;
 
// ----
int index = 0;
String code;
String motdepasse;
bool testCle, testCode;
 
// setup
void setup() {
  lcd.begin(16, 2);
  SPI.begin(); 
  rfid.init();
 
  code = String("0000");
  motdepasse = String("1235");
  
  pinMode(rouge, OUTPUT);
  pinMode(vert, OUTPUT);
  
  lcd.clear();
  
  myservo.attach(10);
  myservo.write (20);
  lcd.write("CLE");
  testCle = false;
  testCode = false;
}
 
// loop
void loop() {
  digitalWrite(rouge,1);
  char keypressed = myKeypad.getKey();
 
// TEST bonne cle ET bon code
  if (testCode && testCle){
    
    // affichage LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("CODE : ");
    lcd.print(code);
    lcd.setCursor(0, 1);
    lcd.print("CORRECT");
    
    // temoin OK
    digitalWrite(rouge,0);
    digitalWrite(vert,1);
    
    // ouverture serrure
    myservo.write (100);
    delay(5000); // attend 5 secondes
    
    // temoin PAS OK
    digitalWrite(rouge,1);
    digitalWrite(vert,0);
    
    // fermeture serrure
    myservo.write (20);
    testCle = false;
    testCode = false;
    lcd.clear();
  }
 
// TEST bonne cle ET mauvais code  
  if(testCle && !testCode){      
      lcd.setCursor(0, 0);
      lcd.print("CODE ?");
      digitalWrite (rouge,0);
      delay (200);
      digitalWrite (rouge,1);
      if (keypressed != NO_KEY) {
        lcd.setCursor(index, 1);
        lcd.print(keypressed);
        code[index] = keypressed;  
        index++;
      }
      if (index == 4) {
        index = 0;
        if (code == motdepasse) {
          // code OK
          testCode = true;
          }
         else {
          // code PAS OK
          lcd.setCursor(0, 0);
          lcd.print("CODE : ");
          lcd.print(code);
          lcd.setCursor(0, 1);
          lcd.print("INCORRECT");       
          testCode = false;
          testCle = false;
          }
        lcd.clear();
      }
  }
  
// TEST mauvaise cle  
  if (!testCle) { 
  lcd.setCursor(0,0);
  lcd.print("CLE");
  if (rfid.isCard()) {
        if (rfid.readCardSerial()) {
            if (rfid.serNum[0] != serNum0
                && rfid.serNum[1] != serNum1
                && rfid.serNum[2] != serNum2
                && rfid.serNum[3] != serNum3
                && rfid.serNum[4] != serNum4
            ) {
                // cle PAS OK
                lcd.clear();
                lcd.write("CLE INCORRECT");
                delay (2000);
                lcd.clear ();
                lcd.write ("CLE");
             } else {
               // cle OK
               lcd.clear();
               testCle = true;
               }
             }
          }
 
    rfid.halt();
  }
 
}
