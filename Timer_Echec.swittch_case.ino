// Import the Liquid Crystal library
#include <LiquidCrystal.h>
//Initialise the LCD with the arduino. LiquidCrystal(rs, enable, d4, d5, d6, d7)
LiquidCrystal lcd(12, 11, 7, 6, 5, 4);

volatile int player = 0;
const int butPin = 2;
void setup() {
  lcd.begin(16, 2);
  Serial.begin(9600);
}

void loop() { // Programme principal
  int sec1 = 5;
  int sec2 = 5;
  int tempsdejeu1 = 0;
  int tempsdejeu2 = 0;
  int dt1;
  int dt2;
  int etape=0;
  Serial.println("Loop ");


  while (etape == 0) { //choix du Temps
    Serial.println("etape 0" );
    choixTemps(tempsdejeu1);

    tempsdejeu2 = tempsdejeu1;
    delay(250); // deboucing
    if (digitalRead(butPin) == HIGH) {
      Serial.print("0 a 1 ");
      etape = 1;
    }

  }
  while (etape == 1) {
    Serial.println("etape 1");

    LeJeu(tempsdejeu1, tempsdejeu2,sec1,sec2, etape);

  }
  while (etape == 2) { //Affichage fin de jeu
    Findutemps(dt1, dt2);
    if (digitalRead(butPin) == HIGH) {
      Serial.println("restart");
      lcd.clear();
      etape = 0;
    }
  }
}

void choixTemps(int &tempsdejeu) { //Choix du temps de jeu
  int potPin = A1;
  pinMode(potPin, INPUT);
  char timeline[16];
  tempsdejeu = analogRead(potPin) / 51; //Lecture du voltage 0-1023(0-5V) donc max 20 minutes de jeu
  lcd.setCursor(0, 0);
  lcd.print("Temps de jeu");
  lcd.setCursor(0, 1);
  sprintf(timeline, "%0.1d mins chaque", tempsdejeu);
  lcd.print(timeline);

}



void turn() { //Change au prochain joueur
  delay(250);// debouncing
  Serial.print("turn ");
  player++;
  player = player % 2;
  Serial.print(player);
}

void LeJeu(int &min1, int &min2, int &sec1, int &sec2, int &etape) {
  char timeline1[16];
  char timeline2[16];
  int dt1 = 0;
  int dt2 = 0;
  pinMode(butPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(butPin), turn , RISING); // met l'interruption lorsque butPin1 passe de LOW a HIGH
  switch (player) { //tours
    case 1 : // Player 1
      dt1++;
      if (sec1 != 0 ) {
        sec1--;
        Serial.print(sec1);
        Serial.print("-");
        break;
      }
      else { // changer de minute
        Serial.print("IF 2");
        min1--;
        Serial.print("IF 3");
        sec1 = 59;
        Serial.print("IF 4");
        break;

      }
      break;
    case 0: // Player 2
      dt2++;
      if (sec2 == 0) { // changer de minute
        min2--;
        sec2 = 59;
        break;
      }
      else {
        sec2--;
        break;
      }
  }
  lcd.clear();
  sprintf(timeline1, "%0.2d:%0.2d", min1, sec1);
  sprintf(timeline2, "%0.2d:%0.2d", min2, sec2);
  lcd.setCursor(5, 0); //affichage player 1
  lcd.print(timeline1);
  lcd.setCursor(5, 1); // affichage / player 2
  lcd.print(timeline2);
  delay(1000);
  Serial.print("FINJEU ");
  if ((min1 == 0 && sec1 == 0) || (min2 == 0 && sec2 == 0)) { // condition fin de jeu
    Serial.print("condition fin jeu ");
    lcd.clear();
    etape = 2;
  }
}


void Findutemps(int dt1, int dt2) { // Temps termine
  char stats[16];
  lcd.setCursor(0, 0);
  lcd.print("Fin du temps");
  lcd.setCursor(0, 1);
  sprintf(stats, "P1 %0.2d s P2 %0.2d s", dt1, dt2);
  lcd.print(stats);
  Serial.println("Voyons ");
}
