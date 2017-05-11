//RFID
#include "SPI.h"
#include "MFRC522.h"

#define SS_PIN 10
#define RST_PIN 9
#define SP_PIN 8
MFRC522 rfid(SS_PIN, RST_PIN);

MFRC522::MIFARE_Key key;


const int LED_PIN = A0;
const int zatvoreno = 4;
const int otvoreno = 2;
const int IN1 = 3;
const int IN2 = 5;
const int ENA = 6;
unsigned long trenutnovrijeme = 0;
boolean stanje = false; //false = zatvoreno, true = otvoreno
//int zatvoreno = 0;    //closed
//int otvoreno = 0;     //open

void setup() {
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
  pinMode(ENA,OUTPUT);
  pinMode(LED_PIN,OUTPUT);
  analogWrite(ENA,0);
  pinMode (otvoreno,INPUT);
  pinMode (zatvoreno,INPUT);
  //zatvoreno = digitalRead(PREKIDAC); //check if "closed" switch is pressed
  //otvoreno = digitalRead (PREKIDAC2); //check if "open" switch is pressed
  
}

void loop() {
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial())
    return;

  // Serial.print(F("PICC type: ")); ispisuje koji je PICC type
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  // Serial.println(rfid.PICC_GetTypeName(piccType));

  // Check if the PICC of Classic MIFARE type- Služi samo za ispitivanje vrste kartice, ispisuje ako nije vrsta MIFARE classic
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&
    piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
    piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    Serial.println(F("Your tag is not of type MIFARE Classic."));
    return;
  }
  
  String strID = "";
  for (byte i = 0; i < 4; i++) {
    strID +=
    (rfid.uid.uidByte[i] < 0x10 ? "0" : "") +
    String(rfid.uid.uidByte[i], HEX) +
    (i!=3 ? ":" : "");
  }
  strID.toUpperCase(); //slova iz stringa u CAPS LOCK (velika slova)

  // kada ova varijabla strID zadrži UID od oznake (tag)
  // može se koristiti kao input u IF (It can be used as input to IF to the tube)
  // LED je ugašena ili upaljena (LED illuminated or extinguished)
  Serial.print("Tap card key: ");
  Serial.println(strID);
 

 int ca = strID.indexOf ("75:AA:25:A6");
//zatvoreno = digitalRead(PREKIDAC);
//otvoreno = digitalRead (PREKIDAC2);

 //OTVARANJE I ZATVARANJE SA RFID-om
 
  if (ca >=0 && stanje == false){ //ako očita karticu i ako je "zatvoreni" prekidač pritisnut otvara vrata
    digitalWrite(IN1,HIGH);      //CCW rotation
    digitalWrite(IN2,LOW);
    analogWrite(ENA,80);
    while(1){
      if (digitalRead (otvoreno) == HIGH){
        stanje = true;
        analogWrite(ENA, 40);
        delay(3000);
        digitalWrite(LED_PIN, LOW); //turn off LED
        analogWrite(ENA, 0); //disable h-bridge
        digitalWrite(IN1, LOW); //reset h-bridge config
        
      }break;
      if (millis()> trenutnovrijeme + 500){
        digitalWrite(LED_PIN,HIGH);
        delay(500);
        trenutnovrijeme = millis();
      }
      else {
        digitalWrite (LED_PIN, LOW);
      }
    }
  }
 

 if (ca >=0 && stanje == true){ //ako očita karticu i ako je "zatvoreni" prekidač pritisnut otvara vrata
    digitalWrite(IN1,LOW);      //CW rotation
    digitalWrite(IN2,HIGH);
    analogWrite(ENA,80);
    while(1){
      if (digitalRead (zatvoreno) == HIGH){
        stanje = false;
        analogWrite(ENA, 40);
        delay(3000);
        digitalWrite(LED_PIN, LOW); //turn off LED
        analogWrite(ENA, 0); //disable h-bridge
        digitalWrite(IN2, LOW); //reset h-bridge config
        
      }break;
      if (millis()> trenutnovrijeme + 500){
        digitalWrite(LED_PIN,HIGH);
        delay(500);
        trenutnovrijeme = millis();
      }
      else {
        digitalWrite (LED_PIN, LOW);
      }
    }
  }
  }
   
   
