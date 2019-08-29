// Import the Liquid Crystal library
#include <LiquidCrystal.h>
//Initialise the LCD with the arduino. LiquidCrystal(rs, enable, d4, d5, d6, d7)
LiquidCrystal lcd(12, 11, 7, 6, 5, 4);

volatile int player = 1;
char timeline[16];
int mins1 = 1;
int sec1 = 0;
int mins2 = 1;
int sec2 = 0;
int buttonmode1 = 0;
int buttonmode2 = 8;
const int butpin1 = 2;
const int butpin2 = 3;
int coup1 = 0;
int coup2 = 0;
int stepp = 0;
int etape = 0;
int potPin = A1;
int tempsVal;
int temps;


void setup() {
  pinMode(potPin, INPUT);
  pinMode(butpin1, INPUT_PULLUP);
  pinMode(butpin2, INPUT_PULLUP);
  lcd.begin(16, 2);
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(butpin1), turn , RISING);

}
void turn() {

  delay(1);

  if (etape <= 1) {
    etape++;
  }
  else {
    if (player % 2 == 1) {
      coup1++;
    }
    else {
      coup2++;
    }

    player++;

    player = player % 2 ;

    if (etape <= 1) {
      etape++;
      lcd.clear();
    }
  }
}

void loop() {

  while (etape == 0) {
    tempsVal = analogRead(potPin);
    Serial.println(tempsVal);
    temps = tempsVal / 51;
    lcd.setCursor(0, 0);
    lcd.print("Temps de jeu");
    lcd.setCursor(0, 1);
    sprintf(timeline, "%0.1d mins chaque", temps);
    lcd.print(timeline);
    mins1 = mins2 = temps;
    delay(250);

  }


  buttonmode1 = digitalRead(butpin1);
  buttonmode2 = digitalRead(butpin2);

  stepp = 1;
  Serial.print(stepp);






  // Loop Temps***********************************************************

  switch (player) {
      stepp = 2;
      Serial.print(stepp);

    case 1:

      Serial.print(" player1 ");


      stepp = 3;
      Serial.print(stepp);

      if (sec1 == 0) {
        stepp = 5;
        Serial.print(stepp);
        if (mins1 != 0) {
          stepp = 6;
          Serial.print(stepp);
          mins1--;
          sec1 = 60;
          Serial.print("Fin1");
        }
      }

      if (mins1 == 0 && sec1 == 0) {

        lcd.clear();
        Serial.print("Fin2");
        etape = 3;
        lcd.print("Fin");         // Fin
        break;

      }
      else {



        stepp = 4;
        Serial.print(stepp);
        sec1--;
        delay(1000);
        lcd.setCursor(0, 0); //player 1
        sprintf(timeline, "%0.2d : %0.2d ", mins1, sec1);
        lcd.print(timeline);
        lcd.print(" ");
        lcd.print(coup1);
        lcd.print(" ");
        lcd.print("Coups");
        lcd.setCursor(0, 1); //player 2
        sprintf(timeline, "%0.2d : %0.2d ", mins2, sec2);
        lcd.print(timeline);
        lcd.print(" ");
        lcd.print(coup2);
        lcd.print(" ");
        lcd.print("Coups");
        break;


      }

      if ( sec1 != 0) {
        break;
      }

      break;




    case 0 :

      Serial.print(" player2 ");
      Serial.print(player);

      if (sec2 == 0) {
        stepp = 5;
        Serial.print(stepp);
        if (mins2 != 0) {
          stepp = 6;
          Serial.print(stepp);
          mins2--;
          sec2 = 60;
          Serial.print("Fin1");
        }
      }



      if (mins2 == 0 && sec2 == 0) {

        lcd.clear();
        Serial.print("Fin4");
        etape = 4;
        break;

      }
      else {
        sec2--;
        delay(1000);
        lcd.setCursor(0, 0); //player 1
        sprintf(timeline, "%0.2d : %0.2d ", mins1, sec1);
        lcd.print(timeline);
        lcd.print(" ");
        lcd.print(coup1);
        lcd.print(" ");
        lcd.print("Coups");
        lcd.setCursor(0, 1); //player 2
        sprintf(timeline, "%0.2d : %0.2d ", mins2, sec2);
        lcd.print(timeline);
        lcd.print(" ");
        lcd.print(coup2);
        lcd.print(" ");
        lcd.print("Coups");



      }

      if ( sec2 != 0) {
        break;
      }





  }

  //   Fin du temps d'un des joueurs**********************************

  while (etape == 3) {
    lcd.setCursor(0, 0);
    lcd.print("Fin du temps");
    lcd.setCursor(0, 1); //player 2
    sprintf(timeline, "%0.2d : %0.2d ", mins2, sec2);
    lcd.print(timeline);
  }

  while (etape == 4) {
    lcd.setCursor(0, 1);
    lcd.print("Fin du temps");
    lcd.setCursor(0, 0); //player 1
    sprintf(timeline, "%0.2d : %0.2d ", mins1, sec1);
    lcd.print(timeline);
  }





  Serial.print(' ');
  Serial.print(stepp);
  Serial.print(' ');
  Serial.print(sec1);
  Serial.print(' ');
  Serial.print(player);
  Serial.print('\n');




}
