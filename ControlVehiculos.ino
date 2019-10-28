// Incluimos librerías
#include <SPI.h>   
#include <MFRC522.h> 
#include <LiquidCrystal.h>

// Se definen los pines RESET y SS del lector RFID y los pines de la pantalla LCD
#define SS_PIN 53 
#define RST_PIN 6 
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// Variables a utilizar
int numero_serie[4];
const int buttonPin = 8; //Pin del botón 
int contador = 1;
int estado_actual = 0;
const int ledPin = 13; //Pin del LED indicador del motor
int usuario[3][4]={ {227,93,65,197},{182,48,0,73},{134,249,190,50} }; // En hex: E3 5D 41 C5; B6 30 00 49 (el que entra); 86 F9 BE 32   
MFRC522 rfid(SS_PIN, RST_PIN); // Instancia de esta clase
MFRC522::MIFARE_Key key;
byte nuidPICC[4];
byte customChar[8] = { //í
    B00000,
    B00010,
    B00100,
    B00000,
    B00100,
    B00100,
    B00100,
    B00000
};
byte customChar2[8] = { //á
    B00010,
    B00100,
    B01100,
    B00010,
    B01110,
    B10010,
    B01110,
    B00000
};

// Se inicia la tasa de comunicación y se inicializan pines de entrada, salida y librerías 
void setup()  {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  lcd.begin(16, 2);
  SPI.begin(); 
  rfid.PCD_Init();  
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }
}

// Se repite en bucle la función "Tarjetas"
void loop(){
    Tarjetas();             
  }
  
void Tarjetas(){ 
    if ( ! rfid.PICC_IsNewCardPresent())
    return;
  if ( ! rfid.PICC_ReadCardSerial())
    return;
  Serial.print(F("PICC type: "));
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  Serial.println(rfid.PICC_GetTypeName(piccType));

  // Comprobar si el TAG es del tipo MFRC522
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&  
    piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
    piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    Serial.println(F("Tu tag no es de tipo MIFARE Clasica."));
    return;
  }

  if (rfid.uid.uidByte[0] != nuidPICC[0] || 
    rfid.uid.uidByte[1] != nuidPICC[1] || 
    rfid.uid.uidByte[2] != nuidPICC[2] || 
    rfid.uid.uidByte[3] != nuidPICC[3] ) {
    Serial.println(F("Un nuevo tag ha sido detectado."));

    // Se almacenan los ID dentro del array nuidPICC
    for (byte i = 0; i < 4; i++) {
      nuidPICC[i] = rfid.uid.uidByte[i];
    }
   
    Serial.println(F("El Tag es:"));
    Serial.print(F("En hexadecimal: "));
    printDec(rfid.uid.uidByte, rfid.uid.size);
    Serial.println();
  }
    
   if (((( rfid.uid.uidByte[0] ) == 182)) && ((( rfid.uid.uidByte[1] ) == 48)) && ((( rfid.uid.uidByte[2] ) == 0)) && ((( rfid.uid.uidByte[3] ) == 73)))
   {
        lcd.setCursor(2,0);
        lcd.print("Bienvenido.");
        lcd.setCursor(2,1);
        lcd.print("Su ID est");
        lcd.createChar(1,customChar2);
        lcd.setCursor(11,1);
        lcd.write((byte)1);
        delay(3000);
        lcd.clear();
        lcd.setCursor(2,0);
        lcd.print("registrado y"); 
        lcd.setCursor(0,1);
        lcd.print("puede acceder a");      
        delay(3000);
        lcd.clear();
        lcd.setCursor(1,0);
        lcd.print("conducir este");
        lcd.setCursor(3,1);
        lcd.print("veh");
        lcd.createChar(1,customChar);
        lcd.setCursor(6,1);
        lcd.write((byte)1);
        lcd.setCursor(7,1);
        lcd.print("culo.");
        delay(3000);
        lcd.clear();
        estado_actual = digitalRead(buttonPin);
        if(estado_actual == HIGH && contador == 1){
          digitalWrite(ledPin, HIGH); //Led encendido
          lcd.print("Motor encendido.");
          lcd.setCursor(2,1);
          lcd.print("Buen viaje.");
          delay(3000);
          lcd.clear();
          contador--;
          }
         else if(estado_actual == HIGH && contador == 0){
          digitalWrite(ledPin, LOW); //Led apagado
          lcd.setCursor(1,0);
          lcd.print("Motor apagado.");
          lcd.setCursor(0,1);
          lcd.print("Salga del coche.");
          delay(3000);
          lcd.clear();
          contador++;
          }}  
    else {
        lcd.setCursor(2,0);
        lcd.print("Lo sentimos.");
        lcd.setCursor(1,1);
        lcd.print("Su ID no est");
        lcd.createChar(1,customChar2);
        lcd.setCursor(13,1);
        lcd.write((byte)1);
        delay(3000);
        lcd.clear();
        lcd.setCursor(1,0);
        lcd.print("registrado y no"); 
        lcd.setCursor(1,1);
        lcd.print("tiene acceso a"); 
        delay(3000);     
        lcd.clear();
        lcd.setCursor(1,0);
        lcd.print("conducir este");
        lcd.setCursor(3,1);
        lcd.print("veh");
        lcd.createChar(1,customChar);
        lcd.setCursor(6,1);
        lcd.write((byte)1);
        lcd.setCursor(7,1);
        lcd.print("culo.");
        delay(3000);
        lcd.clear();
   Serial.println("");
  }
rfid.PICC_HaltA();
rfid.PCD_StopCrypto1();
}

// Función para sacar los TAGS en decimal de las  tarjetas RFID
void printDec(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], DEC);
  }
}
