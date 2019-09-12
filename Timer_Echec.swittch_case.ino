// Import the Liquid Crystal library
#include <LiquidCrystal.h>
//Initialise the LCD with the arduino. LiquidCrystal(rs, enable, d4, d5, d6, d7)
LiquidCrystal lcd(12, 11, 7, 6, 5, 4);

volatile int player = 0;
const int butPin = 2;
int actualisation = 0;
void setup() {
  lcd.begin(16, 2);
  Serial.begin(9600);
}

void loop() { // Programme principal
  Mode_Timer_Echec();

}

void choixTemps(int &tempsdejeu) { //Choix du temps de jeu
  int potPin = A1;
  pinMode(potPin, INPUT);
  char timeline[16];
  tempsdejeu = analogRead(potPin) / 51; //Lecture du voltage 0-1023(0-5V) donc max 20 minutes de jeu
  lcd.setCursor(0, 0);
  lcd.print("Temps de jeu");
  lcd.setCursor(0, 1);
  sprintf(timeline, "%0.1d mins", tempsdejeu);
  lcd.print(timeline);

}



void turn() { //Change au prochain joueur
  delay(250);// debouncing
  Serial.print("turn ");
  player++;
  player = player % 2;
  actualisation = 1;
}

void LeJeu(int &min1, int &min2, int &sec1, int &sec2, int &etape) {
  char timeline1[16];
  char timeline2[16];
  int dt1 = 0;
  int dt2 = 0;
  actualisation = 0;
  pinMode(butPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(butPin), turn , RISING); // met l'interruption lorsque butPin1 passe de LOW a HIGH
  switch (player) { //tours
    case 1 : // Player 1
      dt1++;
      Minuteur(sec1, min1);
      break;
    case 0: // Player 2
      dt2++;
      Minuteur(sec2, min2);
      break;
  }
  lcd.clear();
  sprintf(timeline1, "%0.2d:%0.2d", min1, sec1);
  sprintf(timeline2, "%0.2d:%0.2d", min2, sec2);
  lcd.setCursor(5, 0); //affichage player 1
  lcd.print(timeline1);
  lcd.setCursor(5, 1); // affichage / player 2
  lcd.print(timeline2);
  delay(1000);
  Serial.print("- FINJEU ");
  if ((min1 == 0 && sec1 == 0) || (min2 == 0 && sec2 == 0)) { // condition fin de jeu
    Serial.print("condition fin jeu ");
    lcd.clear();
    etape = 2;
  }
}
void calculAverage(int &bucket1, int &bucket2, int &tprec1, int &tprec2, int &coups, int sec1, int sec2) { // Calcul la moyenne de temps que chaque joueur prend pour jouer
  int dt1;
  int dt2;
  if (player == 0) { //player 1
    coups++;
    if (tprec1 < sec1) { // changement de minute
      tprec1 = tprec1 + 60;
    }
    dt1 = tprec1 - sec1;
    tprec1 = sec1;
    bucket1 = (bucket1 + dt1) / coups;
  }
  if (player == 1) { // player 2
    if (tprec2 < sec2) { // changement de minute
      tprec2 = tprec2 + 60;
    }
    dt2 = tprec2 - sec2;
    tprec2 = sec2;
    bucket2 = (bucket2 + dt2) / coups;
  }
}

void Findutemps(int dt1, int dt2, int coups) { // Temps termine
  char stats1[16];
  char stats2[16];
  lcd.setCursor(0, 0);
  sprintf(stats1, "Times Up! %0.1d Cps", coups);
  lcd.print(stats1);
  lcd.setCursor(0, 1);
  sprintf(stats2, "P1 %0.2ds P2 %0.2ds", dt1, dt2);
  lcd.print(stats2);
  Serial.println("Voyons ");
}

void Mode_Timer_Echec() {

  int sec1 = 0;
  int sec2 = 0;
  int tempsdejeu1 = 0;
  int tempsdejeu2 = 0;
  int etape = 0;
  int bucket1 = 0; // contenant de la moyenne de temps de jeu de la game
  int bucket2 = 0;
  int tprec1 = 60; // enregistrement du temps precedent
  int tprec2 = 60;
  int coups = 0; // nb de coups
  Serial.println("Loop ");


  while (etape == 0) { //choix du Temps
    Serial.println("etape 0 " );
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
    if (actualisation == 1) {
      calculAverage(bucket1, bucket2, tprec1, tprec2, coups, sec1, sec2);
    }
    LeJeu(tempsdejeu1, tempsdejeu2, sec1, sec2, etape);

    Serial.print("- coups: ");
    Serial.print(coups);
    Serial.print("Bucket1: ");
    Serial.print(bucket1);

  }
  while (etape == 2) { //Affichage fin de jeu
    Findutemps(bucket1, bucket2, coups);
    if (digitalRead(butPin) == HIGH) {
      lcd.clear();
      etape = 0;
    }
  }
}
/////////////////////////////////////////////////////////////////////////////// Partie a continuer
void Mode_Minuteur() {
  int etape;
  int mins;
  int secs;
  char affichage[16];

  while (etape = 0) { // choix du temps pour la minuterie
    choixTemps(mins);
    if (digitalRead(butPin) == HIGH) {
      etape = 1;
    }
  }
  lcd.clear();
  lcd.setCursor(0, 5);
  lcd.print("Minuteur");
  lcd.setCursor(1, 6);
  sprintf(affichage, " %0.2d:%0,2d ", mins, secs);
  lcd.print(affichage);
  Minuteur(secs, mins); /// Minutage

}

void Minuteur(int &sec1, int &min1) { // minuteur a mettre partout lorsque le temps doit descendre
  if (sec1 != 0 ) {
    sec1--;
  }
  else { // changer de minute
    min1--;
    sec1 = 59;

  }
}

void Mode_Chronometre() {
  int mins;
  int secs;
  int etape;
  char affichage[16];
  while (etape = 0) { // choix du temps pour la minuterie
    choixTemps(mins);
    if (digitalRead(butPin) == HIGH) {
      etape = 1;
    }
  }
  if (secs != 60 ) {// addition du temps pour faire un chronometre
    secs++;
  }
  else { // changer de minute
    mins++;
    secs = 0;

  }

  lcd.clear();
  lcd.setCursor(0, 5);
  lcd.print("Chronometre");
  lcd.setCursor(1, 6);
  sprintf(affichage, " %0.2d:%0.2d", mins, secs);
  lcd.print(affichage);
}
