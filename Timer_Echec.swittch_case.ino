// Import the Liquid Crystal library
#include <LiquidCrystal.h>
//Initialise the LCD with the arduino. LiquidCrystal(rs, enable, d4, d5, d6, d7)
LiquidCrystal lcd(12, 11, 7, 6, 5, 4);

volatile int player = 1;
int sec1;
int sec2;

int etape = 0;
const int butPin = 2;
void setup() {
  lcd.begin(16, 2);
  Serial.begin(9600);
}

void loop() { // Programme principal

  int tempsdejeu1 = 0;
  int tempsdejeu2 = 0;
  int dt1;
  int dt2;
  Serial.print(9);
  Serial.println("etape :");
  Serial.print(etape);


  while (etape == 0) { //choiz du Temps
    Serial.println("etape 0" );
    choixTemps(tempsdejeu1);

    tempsdejeu2 = tempsdejeu1;
    if (digitalRead(butPin) == HIGH) {
      etape = 1;
    }

  }
  while (etape == 1) {
    Serial.println("etape 1");

    LeJeu(tempsdejeu1, tempsdejeu2, etape);

  }
  while (etape == 4) {
    etape = Findutemps(dt1, dt2);
    if (etape == 2 && digitalRead(butPin) == HIGH) {
      Serial.print("encore");
      return (0);
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



int turn(int player) { //Change au prochain joueur
  player++;
  player = player % 2;
  return (player);
}

void LeJeu(int &min1, int &min2, int &etape) {
  char timeline1[16];
  char timeline2[16];
  int dt1 = 0;
  int dt2 = 0;
  pinMode(butPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(butPin), turn , RISING); // met l'interruption lorsque butPin1 passe de LOW a HIGH
  if ((min1 == 0 && sec1 == 0) || (min2 == 0 && sec2 == 0)) {
    Serial.print("condition fin jeu");
    etape = 2;
  }


  switch (player) {
    case 1 : // Player 1
      dt1++;
      if (sec1 != 0 ) { // changer de minute
        sec1--;
        Serial.print(sec1);
        Serial.print(" IF 1 ");
        break;
      }
      else {
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
  lcd.setCursor(0, 0); //affichage player 1
  lcd.print(timeline1);
  lcd.setCursor(0, 1); // affichage / player 2
  lcd.print(timeline2);
  delay(1000);
  Serial.print("FINJEU");

}


int Findutemps(int dt1, int dt2) { // Temps termine
  char stats[16];
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Fin du temps");
  lcd.setCursor(0, 1);
  sprintf(stats, "Blancs %0.2d secs Noirs %0.2d secs", dt1, dt2);
  lcd.print(stats);
  Serial.print("Voyons");

}
