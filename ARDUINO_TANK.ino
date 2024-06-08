
#include <Wire.h>
#include <LiquidCrystal.h>
#include <Keypad.h>

#define EEPROM_ID 0xA0 >> 1
//DEFINE DIGITAL INPUT PINS
//DEFINE POSITIONS
#define INLET 0x00
#define OUTLET 0x01
#define HEATER 0x02
#define COOLER 0x03
#define STIRRER 0x04
//DEFINE DIGITAL INPUT PINS
#define HIGHFLOATER 0x10
#define LOWFLOATER 0x11
#define TMIN 0x12
#define TMAX 0x13
//DEFINE DIGITAL INPUT PINS
#define MINALA 0x20
#define MAXALA 0x21
//DEFINE DIGITAL INPUT PINS
#define VOL 0x30
#define TEMP 0x31
//START OR STOP ACTION FOR EVERY PIN OF THE SERIAL REMOTE TANK
#define START 0x01
#define STOP 0x00

const int rs = 13, en = 12, d4 = 8, d5 = 9, d6 = 10, d7 = 11;  //LCD DISPLAY PINS
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);                     //LCD DISPLAY PINS

bool firstCharEntered = false;   //FIRST CHARACTER CHECK INITIALLIZATION AS A BOOLEAN FALSE...IS MEANT TO BE FOR THE SECOND STEP
bool secondCharEntered = false;  //SECOND CHARACTER CHECK INITIALLIZATION AS A BOOLEAN FALSE...IS MEANT TO BE FOR THE SECOND STEP
bool thirdCharEntered = false;   //THIRD CHARACTER CHECKD INITIALLIZATION AS A BOOLEAN FALSE...IS MEANT TO BE FOR THE SECOND STEP

char EnteredKey1, EnteredKey2, EnteredKey3, EnteredKey4;  //KEYS PRESSED FROM KEYPAD

const byte ROWS = 4;  //INITIALLIZING THE NUMBER OF ROWS AT KEYPAD
const byte COLS = 4;  //INITIALLIZING THE NUMBER OF COLS AT KEYPAD

char hexaKeys[ROWS][COLS] = {  //DECLARING THE KEYPAD CONTENTS
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};

byte rowPins[ROWS] = { A0, A1, A2, A3 };  //KEYPAD PINS OF ROWS
byte colPins[COLS] = { 7, 6, 5, 4 };      //KEYPAD PINS OF COLS

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);  //GET A CHARACTER

void setup() {  //CODE TO RUN ONE TIME

  int i;  //VARIABLE INITIALLIZATION

  lcd.begin(16, 2);              //INITIALIZATION OF THE LCD TO DECLARE THAT IT HAS TWO ROWS WITH 16 COLS EACH
  lcd.setCursor(0, 0);           //SET CURSOR TO WRITE AT 0,0
  lcd.print("NAME, 20..");    //PRINT THIS AT 0,0
  lcd.setCursor(0, 1);           //SET CURSOR TO WRITE AT 0,1
  lcd.print("ENTER VOL: ????");  //PRINT THIS AT 0,1

  Serial.begin(9600);  //SERIAL COMMUNICATION WITH BAUD RATE 9600
  Wire.begin();        //INITIALLIZE WIRE TO CONNECT I2C

  Wire.beginTransmission(EEPROM_ID);  //BEGIN TRANSMISSION TO EEPROM
  Wire.write(0);                      //START FROM POSITION 0
  for (i = 0; i < 30; i++) {          //LOOP TO WRITE AT EEPROM
    Wire.write(0xAA);                 //BEGIN TO WRITE AT EEPROM AA TILL WRITTEN 30 AA...HERE WE MAKE IT JUST FOR DISCREET REASONS
  }
  Wire.endTransmission();  //STOP TRANSIMISSION
}

void loop() {  //CODE TO EXCECUTE REPETEADLY
               //DECLARING VARIABLES
  unsigned WATERLEVEL = 0;
  unsigned TANKVOL = 0;
  unsigned TEMPERATURE = 0;
  unsigned char TEMPLOWBYTE, TEMPHIGHBYTE, TANKLOWBYTE, TANKHIGHBYTE;
  unsigned int FirstNumber, SecondNumber, ThirdNumber, FourthNumber;

  if (!firstCharEntered) {                //IF FIRST CHARACTER IS FALSE THEN ENTER THE CONDITION
    EnteredKey1 = customKeypad.getKey();  //READ A CHARACTER FROM KEYPAD

    if (EnteredKey1 != NO_KEY) {  //NO_KEY...IF THERE IS NO WAITING CHARACTER THE KEYPAD. getKey() WILL RETURN NO_KEY...WE HAVE A CHARACTER
      firstCharEntered = true;    //WE HAVE ENTERED THE FIRST CHARACTER SO MAKE IT BOOLEAN TRUE
      lcd.setCursor(11, 1);
      lcd.print(EnteredKey1);
    }
  } else if (!secondCharEntered) {        //IF SECOND CHARACTER IS FALSE THEN ENTER THE CONDITION
    EnteredKey2 = customKeypad.getKey();  //READ A CHARACTER FROM KEYPAD
    if (EnteredKey2 != NO_KEY) {          //THERE IS A WAITING CHARACTER SO ...ENTER THE CONDITION
      secondCharEntered = true;           //WE HAVE ENTERED THE SECOND CHARACTER SO MAKE IT BOOLEAN TRUE
      lcd.setCursor(12, 1);               //SET CURSOR TO WRITE AT 12,1
      lcd.print(EnteredKey2);             //PRINT THIS AT 12,1
    }
  } else if (!thirdCharEntered) {         //IF THIRD CHARACTER IS FALSE THEN ENTER THE CONDITION
    EnteredKey3 = customKeypad.getKey();  //READ A CHARACTER FROM KEYPAD
    if (EnteredKey3 != NO_KEY) {          //THERE IS A WAITING CHARACTER SO ...ENTER THE CONDITION
      thirdCharEntered = true;            //WE HAVE ENTERED THE FOURTH CHARACTER SO MAKE IT BOOLEAN TRUE
      lcd.setCursor(13, 1);               //SET CURSOR TO WRITE AT 13,1
      lcd.print(EnteredKey3);             //PRINT THIS AT 13,1
    }
  } else {                                //ALL CHARACTERS HAVE BEEN ENTERED...ENTER THE FOURTH
    EnteredKey4 = customKeypad.getKey();  //READ A CHARACTER FROM KEYPAD
    if (EnteredKey4 != NO_KEY) {          //THERE IS A WAITING CHARACTER SO ...ENTER THE CONDITION
      lcd.setCursor(14, 1);               //SET CURSOR TO WRITE AT 14,1
      lcd.print(EnteredKey4);             //PRINT THIS AT 14,1

      delay(1000);
      lcd.clear();  //CLEAR LCD SCREEN

      lcd.setCursor(0, 0);         //SET CURSOR TO WRITE AT 0,0
      lcd.print("SAVING VOLUME");  //PRINT THIS AT 0,0
      lcd.setCursor(0, 1);         //SET CURSOR TO WRITE AT 0,1
      lcd.print("TO EEPROM");      //PRINT THIS AT 0,1

      delay(1000);  //DELAY BETWEEN MESSAGES

      Wire.beginTransmission(EEPROM_ID);  //BEGIN TRANSMISSION
      Wire.write(0x10);                   //START FROM POSITION 10...SECOND LINE FIRST COLUMN
      Wire.write(EnteredKey1);            //WRITE FIRST NUMBER
      Wire.write(EnteredKey2);            //WRITE SECOND NUMBER
      Wire.write(EnteredKey3);            //WRITE THIRD NUMBER
      Wire.write(EnteredKey4);            //WRITE FOURTH NUMBER
      Wire.endTransmission();             //STOP TRANSMISSION

      FirstNumber = EnteredKey1 - '0';   // CONVERT THE CHARACTER REPRESENTATION OF THE DIGIT TO ITS INTEGER VALUE.
      SecondNumber = EnteredKey2 - '0';  // CONVERT THE CHARACTER REPRESENTATION OF THE DIGIT TO ITS INTEGER VALUE.
      ThirdNumber = EnteredKey3 - '0';   // CONVERT THE CHARACTER REPRESENTATION OF THE DIGIT TO ITS INTEGER VALUE.
      FourthNumber = EnteredKey4 - '0';  // CONVERT THE CHARACTER REPRESENTATION OF THE DIGIT TO ITS INTEGER VALUE.

      unsigned int desired_vol = (FirstNumber * 1000 + SecondNumber * 100 + ThirdNumber * 10 + FourthNumber);  //CREATE THE WHOLE NUMBER

      lcd.clear();  //CLEAR LCD SCREEN

      lcd.setCursor(0, 0);     //SET CURSOR TO WRITE AT 0,0
      lcd.print("VOLUME");     //PRINT THIS AT 0,0
      lcd.setCursor(10, 0);    //SET CURSOR TO WRITE AT 10,0
      lcd.print(desired_vol);  //PRINT THIS AT 10,0

      delay(1000);  //DELAY BETWEEN MESSAGES

      lcd.clear();  //CLEAR LCD SCREEN

      lcd.setCursor(0, 0);      //SET CURSOR TO WRITE AT 0,0
      lcd.print("OUTLET ON");   //PRINT THIS AT 0,0
      lcd.setCursor(0, 1);      //SET CURSOR TO WRITE AT 0,1
      lcd.print("INLET  OFF");  //PRINT THIS AT 0,1

      //THESE LINES ARE TO CHECK THE FUNCTIONALITY OF LOWFLOATER...SO AS NOT TO START FROM ZERO
      //Serial.write(INLET);
      //Serial.write(START);
      //delay(1000);
      //Serial.write(INLET);
      //Serial.write(STOP);

      Serial.write(INLET);
      Serial.write(STOP);

      Serial.write(OUTLET);  //SET OUTLET VALVE
      Serial.write(START);   // TURN ON OUTLET VALVE

      do {
        Serial.write(LOWFLOATER);  //SET LOWFLOATER VALVE
        while (!Serial.available()) {};
        WATERLEVEL = Serial.read();  //READ FROM LOWFLOATER DIGITAL OUTPUT
        if (WATERLEVEL == 0) {
          Serial.write(OUTLET);  //SET OUTLET VALVE
          Serial.write(STOP);    // TURN OFF OUTLET VALVE
        }
      } while (WATERLEVEL > 0);  //CHECK

      delay(1000);

      lcd.clear();  //CLEAR LCD SCREEN

      lcd.setCursor(0, 0);       //SET CURSOR TO WRITE AT 0,0
      lcd.print("OUTLET  OFF");  //PRINT THIS AT 0,0
      lcd.setCursor(0, 1);       //SET CURSOR TO WRITE AT 0,1
      lcd.print("INLET  ON");    //PRINT THIS AT 0,1

      Serial.write(INLET);  //SET INLET VALVE
      Serial.write(START);  // TURN ON INLET VALVE

      while (TANKVOL <= desired_vol) {  //CHECK CONTINUALLY
                                        //READ TWO BYTES
        Serial.read();
        Serial.read();
        Serial.write(VOL);  //SENT ONE BYTE AT VOL PTN
        while (!Serial.available()) {}
        TANKHIGHBYTE = Serial.read();  //HIGH BYTE READ
        while (!Serial.available()) {}
        TANKLOWBYTE = Serial.read();  //LOW BYTE READ

        TANKVOL = TANKHIGHBYTE;
        TANKVOL = (TANKVOL << 8) | (int)TANKLOWBYTE;

        lcd.setCursor(12, 1);  //SET CURSOR TO WRITE AT 12,1
        lcd.print(TANKVOL);    //PRINT THIS AT 12,1
      };

      Serial.write(INLET);  //SET INLET VALVE
      Serial.write(STOP);   // TURN OFF INLET VALVE

      delay(1000);  //DELAY BETWEEN MESSAGES

      lcd.clear();  //CLEAR LCD SCREEN
                    //READ TWO BYTES
      Serial.read();
      Serial.read();
      Serial.write(TEMP);  //SENT ONE BYTE AT TEMP PTN
      while (!Serial.available()) {}
      TEMPHIGHBYTE = Serial.read();  //HIGH BYTE READ
      while (!Serial.available()) {}
      TEMPLOWBYTE = Serial.read();  //LOW BYTE READ

      TEMPERATURE = TEMPHIGHBYTE;
      TEMPERATURE = (TEMPERATURE << 8) | (int)TEMPLOWBYTE;

      lcd.setCursor(0, 0);          //SET CURSOR TO WRITE AT 0,0
      lcd.print("TEMP");            //PRINT THIS AT 0,0
      lcd.setCursor(10, 0);         //SET CURSOR TO WRITE AT 10,0
      lcd.print(TEMPERATURE);       //PRINT THIS AT 10,0
      lcd.setCursor(0, 1);          //SET CURSOR TO WRITE AT 0,1
      lcd.print("STIR/HEATER ON");  //PRINT THIS AT 0,1

      delay(1000);  //DELAY BETWEEN MESSAGES
      lcd.clear();  //CLEAR LCD SCREEN

      Serial.write(HEATER);  //SET HEATER VALVE
      Serial.write(START);   // TURN ON HEATER VALVE

      Serial.write(STIRRER);  //SET STIRRER VALVE
      Serial.write(START);    // TURN ON STIRRER VALVE

      while (TEMPERATURE <= 26) {  //CHECK CONTINUALLY
        //READ TWO BYTES
        Serial.read();
        Serial.read();
        Serial.write(TEMP);  //SENT ONE BYTE AT TEMP PTN
        while (!Serial.available()) {}
        TEMPHIGHBYTE = Serial.read();  //HIGH BYTE READ
        while (!Serial.available()) {}
        TEMPLOWBYTE = Serial.read();  //LOW BYTE READ

        TEMPERATURE = TEMPHIGHBYTE;
        TEMPERATURE = (TEMPERATURE << 8) | (int)TEMPLOWBYTE;

        lcd.setCursor(12, 0);    //SET CURSOR TO WRITE AT 12,0
        lcd.print(TEMPERATURE);  //PRINT THIS AT 12,0
      };

      Serial.write(HEATER);  //SET HEATER VALVE
      Serial.write(STOP);    // TURN OFF HEATER VALVE

      Serial.write(STIRRER);  //SET STIRRER VALVE
      Serial.write(STOP);     // TURN OFF STIRRER VALVE

      delay(100);   //DELAY BETWEEN MESSAGES
      lcd.clear();  //CLEAR LCD SCREEN

      lcd.setCursor(0, 0);          //SET CURSOR TO WRITE AT 0,0
      lcd.print("TEMP");            //PRINT THIS AT 0,0
      lcd.setCursor(10, 0);         //SET CURSOR TO WRITE AT 10,0
      lcd.print(TEMPERATURE);       //PRINT THIS AT 10,0
      lcd.setCursor(0, 1);          //SET CURSOR TO WRITE AT 0,1
      lcd.print("CYCLE COMPLETE");  //PRINT THIS AT 0,1

      firstCharEntered = false;
      secondCharEntered = false;
      thirdCharEntered = false;
      EnteredKey1 = 0;
      EnteredKey2 = 0;
      EnteredKey3 = 0;
      EnteredKey4 = 0;
    }
  }
}
