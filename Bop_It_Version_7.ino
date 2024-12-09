#include <SPI.h>
#include <SD.h>
#include <LiquidCrystal.h>
#include <ezButton.h>

const int chipSelect = 10;
File dataFile;
int newScore;

const int buttonPin = 8;
const int switchPin = 9;
const int startPin = 7;
const int xpin = 0;
const int ypin = 1;
int UD;
int LR;
int UD_neutral;
int LR_neutral;
int currentScore;

int deadzone = 50;
int leftonlydeadzone = 10;

LiquidCrystal lcd(17, 16, 5, 4, 3, 2);

ezButton button1(8);
ezButton switch1(9);
float starttime = 0;
float elapsed = 0;
float time = 3000;
float remainder;
int currentState;
int previousState;

int score = 0;
int action;
int play;
int flag;

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(switchPin, INPUT);
  pinMode(startPin, INPUT);
  pinMode(chipSelect, OUTPUT);
  pinMode(ypin, INPUT);
  pinMode(xpin, INPUT);
  lcd.begin(16,2);
  randomSeed(analogRead(0));
  Serial.begin(9600);
  button1.setDebounceTime(50);
  switch1.setDebounceTime(50);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
  Serial.println("card initialized.");
  dataFile = SD.open("scores.txt", FILE_WRITE);
  dataFile.close();
  
}

void loop() {
  score=0;
  time = 3000;
  lcd.print(" Hold Button to");
  lcd.setCursor(5,1);
  lcd.print("Start");
  delay(1500);
  lcd.clear();
  
  if (digitalRead(startPin) == HIGH){
    Serial.println("Game will start");
    
    play = 1; 
    flag = 0;
    lcd.clear();
    lcd.print("Game Starts in");
    delay(1000);
    lcd.clear();
    lcd.print("3...");
    delay(1000);
    lcd.setCursor(4,0);
    lcd.print("2...");
    delay(1000);
    lcd.setCursor(8,0);
    lcd.print("1...");
    delay(1000);


    while (play != 0) {
      
      action = random(2,7);
      flag = 0;
      remainder = score % 10;
      previousState = digitalRead(switchPin);

      if ((remainder == 0) && (score != 0)){
        time = time*0.8;
      }

      if (action == 2)  {
        lcd.clear();
        lcd.setCursor(3,0);
        lcd.print("Press It");
        Serial.println("Press it");
        starttime = millis();
        elapsed = 0;
        UD_neutral = analogRead(ypin);
        LR_neutral = analogRead(xpin);
        
  
        while (elapsed <= time){

          elapsed = millis() - starttime;
          //Serial.println(elapsed);
          button1.loop(); 
          currentState = digitalRead(switchPin);
          UD = analogRead(ypin);
          LR = analogRead(xpin);
          
          
          if (button1.isPressed()) {
            score++;
            lcd.clear();
            Serial.println("The button is pressed");
            Serial.print("Score: ");
            Serial.println(score);
            lcd.print("Score: ");
            lcd.setCursor(7,0);
            lcd.print(score);
            delay(1000);
            flag = 1;
            play = 1;
            break;
          }
          else if ((currentState != previousState) || (UD >= UD_neutral+deadzone) || (UD <= UD_neutral-deadzone) || (LR >= LR_neutral+deadzone) || (LR <= LR_neutral-deadzone))  {
            wrongbutton();
          }
        }
        if (flag == 0){
          gameover();
        }
      }
      if (action == 3){
        lcd.clear();
        lcd.setCursor(3,0);
        lcd.print("Slide It");
        Serial.println("Slide it");
        starttime = millis();
        elapsed = 0;
        UD_neutral = analogRead(ypin);
        LR_neutral = analogRead(xpin);
        previousState = digitalRead(switchPin);
        
  
        while (elapsed <= time){

          elapsed = millis() - starttime;
          //Serial.println(elapsed);
          button1.loop(); 
          currentState = digitalRead(switchPin);
          LR = analogRead(xpin);
          UD = analogRead(ypin);

          if ((currentState != previousState)) {
            score++;
            lcd.clear();
            Serial.println("You slid the switch");
            Serial.print("Score: ");
            Serial.println(score);
            lcd.print("Score: ");
            lcd.setCursor(7,0);
            lcd.print(score);
            delay(1000);
            flag = 1;
            play = 1;
            break;
          }
          else if (button1.isPressed() || (UD >= UD_neutral+deadzone) || (UD <= UD_neutral-deadzone) || (LR >= LR_neutral+deadzone) || (LR <= LR_neutral-deadzone))  {
            wrongbutton();
          }
        }
        if (flag == 0){
          gameover();
        }
      }
      if (action == 4){
        lcd.clear();
        lcd.setCursor(3,0);
        lcd.print("Flick It");
        Serial.println("Flick it down");
        imagedown1();
        imagedown2();
        starttime = millis();
        elapsed = 0;
        UD_neutral = analogRead(ypin);
        LR_neutral = analogRead(xpin);
        Serial.println(UD_neutral);
        previousState = digitalRead(switchPin);
        
  
        while (elapsed <= time){

          elapsed = millis() - starttime;
          //Serial.println(elapsed);
          button1.loop(); 
          currentState = digitalRead(switchPin);
          UD = analogRead(ypin);
          LR = analogRead(xpin);
          
          
          if (UD >= UD_neutral+deadzone) {
            score++;
            lcd.clear();
            Serial.println("You flicked it down");
            Serial.print("Score: ");
            Serial.println(score);
            lcd.print("Score: ");
            lcd.setCursor(7,0);
            lcd.print(score);
            delay(1000);
            flag = 1;
            play = 1;
            break;
          }
          else if ((UD <= UD_neutral-deadzone) || (button1.isPressed()) || (currentState != previousState))  {
            wrongbutton();
          }
        }
        if (flag == 0){
          gameover();
        }
      }
      if (action == 5){
        lcd.clear();
        lcd.setCursor(3,0);
        lcd.print("Flick It");
        lcd.setCursor(0,1);

        Serial.println("Flick it up");
        imageup1();
        imageup2();
        starttime = millis();
        elapsed = 0;
        UD_neutral = analogRead(ypin);
        LR_neutral = analogRead(xpin);
        Serial.println(UD_neutral);
        previousState = digitalRead(switchPin);
        
  
        while (elapsed <= time){

          elapsed = millis() - starttime;
          //Serial.println(elapsed);
          button1.loop(); 
          currentState = digitalRead(switchPin);
          UD = analogRead(ypin);
          LR = analogRead(xpin);
          
          
          if (UD <= UD_neutral-deadzone) {
            score++;
            lcd.clear();
            Serial.println("You flicked it up");
            Serial.print("Score: ");
            Serial.println(score);
            lcd.print("Score: ");
            lcd.setCursor(7,0);
            lcd.print(score);
            delay(1000);
            flag = 1;
            play = 1;
            break;
          }
          else if ((UD >= UD_neutral+deadzone) || (button1.isPressed()) || (currentState != previousState))  {
            wrongbutton();
          }
        }
        if (flag == 0){
          gameover();
        }
      }
      if (action == 7){
        lcd.clear();
        lcd.setCursor(3,0);
        lcd.print("Flick It");
        imageleft();
        Serial.println("Flick it left");
        starttime = millis();
        elapsed = 0;
        UD_neutral = analogRead(ypin);
        LR_neutral = analogRead(xpin);
        Serial.println(UD_neutral);
        previousState = digitalRead(switchPin);
        
  
        while (elapsed <= time){

          elapsed = millis() - starttime;
          //Serial.println(elapsed);
          button1.loop(); 
          currentState = digitalRead(switchPin);
          UD = analogRead(ypin);
          LR = analogRead(xpin);
          Serial.println(LR);
          
          
          if (LR <= LR_neutral-leftonlydeadzone) {
            score++;
            lcd.clear();
            Serial.println("You flicked it left");
            Serial.print("Score: ");
            Serial.println(score);
            lcd.print("Score: ");
            lcd.setCursor(7,0);
            lcd.print(score);
            delay(1000);
            flag = 1;
            play = 1;
            break;
          }
          else if ((LR >= LR_neutral+deadzone) || (button1.isPressed()) || (currentState != previousState))  {
            wrongbutton();
          }
        }
        if (flag == 0){
          gameover();
        }
      }
      if (action == 6){
        lcd.clear();
        lcd.setCursor(3,0);
        lcd.print("Flick It");
        imageright();
        Serial.println("Flick it right");
        starttime = millis();
        elapsed = 0;
        UD_neutral = analogRead(ypin);
        LR_neutral = analogRead(xpin);
        Serial.println(UD_neutral);
        previousState = digitalRead(switchPin);
        
  
        while (elapsed <= time){

          elapsed = millis() - starttime;
          //Serial.println(elapsed);
          button1.loop(); 
          currentState = digitalRead(switchPin);
          UD = analogRead(ypin);
          LR = analogRead(xpin);
          
          
          if (LR >= LR_neutral+deadzone) {
            score++;
            lcd.clear();
            Serial.println("You flicked it right");
            Serial.print("Score: ");
            Serial.println(score);
            lcd.print("Score: ");
            lcd.setCursor(7,0);
            lcd.print(score);
            delay(1000);
            flag = 1;
            play = 1;
            break;
          }
          else if ((LR <= LR_neutral-leftonlydeadzone) || (button1.isPressed()) || (currentState != previousState))  {
            wrongbutton();
          }
        }
        if (flag == 0){
          gameover();
        }
      }
    }
  }
}

void gameover(){
  lcd.clear();
  lcd.setCursor(4,0);
  lcd.print("Game Over");
  Serial.println("Nothing was Pressed");
  delay(1000);
  lcd.clear();
  lcd.print("Score: ");
  lcd.setCursor(7,0);
  lcd.print(score);
  delay(2000);
  lcd.clear();

  currentScore = score;
  int previousScore = readPreviousScore();

  if (currentScore > previousScore) {
    updateHighScore(currentScore);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("High Score: ");
    lcd.setCursor(1, 0);
    lcd.print(newScore);
    delay(1000);
  } 
  else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("High Score: ");
    lcd.setCursor(1, 0);
    lcd.print(previousScore);
    delay(1000);
  }
  play = 0;
}

void wrongbutton(){
  Serial.println("Wrong Button");
  lcd.clear();
  lcd.setCursor(4,0);
  lcd.print("Game Over");
  delay(1000);
  lcd.clear();
  lcd.print("Score: ");
  lcd.setCursor(7,0);
  lcd.print(score);
  delay(1000);
  lcd.clear();
  currentScore = score;
  int previousScore = readPreviousScore();

  if (currentScore > previousScore) {
    updateHighScore(currentScore);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("High Score: ");
    lcd.setCursor(1, 0);
    lcd.print(newScore);
    delay(1000);
  } 
  else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("High Score: ");
    lcd.setCursor(1, 0);
    lcd.print(previousScore);
    delay(1000);
  }

  flag = 1;
  play = 0;
}

int readPreviousScore() {
  dataFile = SD.open("scores.txt", FILE_READ);
  int score = 0;

  if (dataFile) {
    score = dataFile.parseInt();
    dataFile.close();
  }
  return score;
}

void updateHighScore(int newScore) {
  dataFile = SD.open("scores.txt", FILE_WRITE);

  if (dataFile) {
    dataFile.println(newScore); // Store each score on a new line
    dataFile.close();
  }
}


void imagedown1() {

  byte image17[8] = {B00100, B00100, B00100, B00100, B00100, B11111, B01110, B00100};
  byte image18[8] = {B00100, B00100, B00100, B00100, B00100, B11111, B01110, B00100};
  byte image19[8] = {B00100, B00100, B00100, B00100, B00100, B11111, B01110, B00100};
  byte image20[8] = {B00100, B00100, B00100, B00100, B00100, B11111, B01110, B00100};
  byte image21[8] = {B00100, B00100, B00100, B00100, B00100, B11111, B01110, B00100};
  byte image22[8] = {B00100, B00100, B00100, B00100, B00100, B11111, B01110, B00100};
  byte image23[8] = {B00100, B00100, B00100, B00100, B00100, B11111, B01110, B00100};
  byte image24[8] = {B00100, B00100, B00100, B00100, B00100, B11111, B01110, B00100};

  lcd.createChar(0, image17);
  lcd.createChar(1, image18);
  lcd.createChar(2, image19);
  lcd.createChar(3, image20);
  lcd.createChar(4, image21);
  lcd.createChar(5, image22);
  lcd.createChar(6, image23);
  lcd.createChar(7, image24);

  lcd.setCursor(0, 1);
  lcd.write(byte(0));
  lcd.setCursor(1, 1);
  lcd.write(byte(1));
  lcd.setCursor(2, 1);
  lcd.write(byte(2));
  lcd.setCursor(3, 1);
  lcd.write(byte(3));
  lcd.setCursor(4, 1);
  lcd.write(byte(4));
  lcd.setCursor(5, 1);
  lcd.write(byte(5));
  lcd.setCursor(6, 1);
  lcd.write(byte(6));
  lcd.setCursor(7, 1);
  lcd.write(byte(7));
}

void imagedown2() {

  byte image25[8] = {B00100, B00100, B00100, B00100, B00100, B11111, B01110, B00100};
  byte image26[8] = {B00100, B00100, B00100, B00100, B00100, B11111, B01110, B00100};
  byte image27[8] = {B00100, B00100, B00100, B00100, B00100, B11111, B01110, B00100};
  byte image28[8] = {B00100, B00100, B00100, B00100, B00100, B11111, B01110, B00100};
  byte image29[8] = {B00100, B00100, B00100, B00100, B00100, B11111, B01110, B00100};
  byte image30[8] = {B00100, B00100, B00100, B00100, B00100, B11111, B01110, B00100};
  byte image31[8] = {B00100, B00100, B00100, B00100, B00100, B11111, B01110, B00100};
  byte image32[8] = {B00100, B00100, B00100, B00100, B00100, B11111, B01110, B00100};

  lcd.createChar(0, image25);
  lcd.createChar(1, image26);
  lcd.createChar(2, image27);
  lcd.createChar(3, image28);
  lcd.createChar(4, image29);
  lcd.createChar(5, image30);
  lcd.createChar(6, image31);
  lcd.createChar(7, image32);

  lcd.setCursor(8, 1);
  lcd.write(byte(0));
  lcd.setCursor(9, 1);
  lcd.write(byte(1));
  lcd.setCursor(10, 1);
  lcd.write(byte(2));
  lcd.setCursor(11, 1);
  lcd.write(byte(3));
  lcd.setCursor(12, 1);
  lcd.write(byte(4));
  lcd.setCursor(13, 1);
  lcd.write(byte(5));
  lcd.setCursor(14, 1);
  lcd.write(byte(6));
  lcd.setCursor(15, 1);
  lcd.write(byte(7));
}

void imageup1() {

  byte image17[8] = {B00100, B01110, B11111, B00100, B00100, B00100, B00100, B00100};
  byte image18[8] = {B00100, B01110, B11111, B00100, B00100, B00100, B00100, B00100};
  byte image19[8] = {B00100, B01110, B11111, B00100, B00100, B00100, B00100, B00100};
  byte image20[8] = {B00100, B01110, B11111, B00100, B00100, B00100, B00100, B00100};
  byte image21[8] = {B00100, B01110, B11111, B00100, B00100, B00100, B00100, B00100};
  byte image22[8] = {B00100, B01110, B11111, B00100, B00100, B00100, B00100, B00100};
  byte image23[8] = {B00100, B01110, B11111, B00100, B00100, B00100, B00100, B00100};
  byte image24[8] = {B00100, B01110, B11111, B00100, B00100, B00100, B00100, B00100};

  lcd.createChar(0, image17);
  lcd.createChar(1, image18);
  lcd.createChar(2, image19);
  lcd.createChar(3, image20);
  lcd.createChar(4, image21);
  lcd.createChar(5, image22);
  lcd.createChar(6, image23);
  lcd.createChar(7, image24);

  lcd.setCursor(0, 1);
  lcd.write(byte(0));
  lcd.setCursor(1, 1);
  lcd.write(byte(1));
  lcd.setCursor(2, 1);
  lcd.write(byte(2));
  lcd.setCursor(3, 1);
  lcd.write(byte(3));
  lcd.setCursor(4, 1);
  lcd.write(byte(4));
  lcd.setCursor(5, 1);
  lcd.write(byte(5));
  lcd.setCursor(6, 1);
  lcd.write(byte(6));
  lcd.setCursor(7, 1);
  lcd.write(byte(7));
}

void imageup2() {

  byte image32[8] = {B00100, B01110, B11111, B00100, B00100, B00100, B00100, B00100};
  byte image31[8] = {B00100, B01110, B11111, B00100, B00100, B00100, B00100, B00100};
  byte image30[8] = {B00100, B01110, B11111, B00100, B00100, B00100, B00100, B00100};
  byte image29[8] = {B00100, B01110, B11111, B00100, B00100, B00100, B00100, B00100};
  byte image28[8] = {B00100, B01110, B11111, B00100, B00100, B00100, B00100, B00100};
  byte image27[8] = {B00100, B01110, B11111, B00100, B00100, B00100, B00100, B00100};
  byte image26[8] = {B00100, B01110, B11111, B00100, B00100, B00100, B00100, B00100};
  byte image25[8] = {B00100, B01110, B11111, B00100, B00100, B00100, B00100, B00100};

  lcd.createChar(0, image32);
  lcd.createChar(1, image31);
  lcd.createChar(2, image30);
  lcd.createChar(3, image29);
  lcd.createChar(4, image28);
  lcd.createChar(5, image27);
  lcd.createChar(6, image26);
  lcd.createChar(7, image25);

  lcd.setCursor(15, 1);
  lcd.write(byte(0));
  lcd.setCursor(14, 1);
  lcd.write(byte(1));
  lcd.setCursor(13, 1);
  lcd.write(byte(2));
  lcd.setCursor(12, 1);
  lcd.write(byte(3));
  lcd.setCursor(11, 1);
  lcd.write(byte(4));
  lcd.setCursor(10, 1);
  lcd.write(byte(5));
  lcd.setCursor(9, 1);
  lcd.write(byte(6));
  lcd.setCursor(8, 1);
  lcd.write(byte(7));
}

void imageright() {

  byte image27[8] = {B11000, B11100, B11110, B11111, B11110, B11100, B11000, B11000};
  byte image26[8] = {B00000, B00000, B00000, B11111, B11111, B00000, B00000, B00000};
  byte image25[8] = {B00000, B00000, B00000, B11111, B11111, B00000, B00000, B00000};
  byte image24[8] = {B00000, B00000, B00000, B11111, B11111, B00000, B00000, B00000};
  byte image23[8] = {B00000, B00000, B00000, B11111, B11111, B00000, B00000, B00000};
  byte image22[8] = {B00000, B00000, B00000, B11111, B11111, B00000, B00000, B00000};

  lcd.createChar(0, image27);
  lcd.createChar(1, image26);
  lcd.createChar(2, image25);
  lcd.createChar(3, image24);
  lcd.createChar(4, image23);
  lcd.createChar(5, image22);

  lcd.setCursor(10, 1);
  lcd.write(byte(0));
  lcd.setCursor(9, 1);
  lcd.write(byte(1));
  lcd.setCursor(8, 1);
  lcd.write(byte(2));
  lcd.setCursor(7, 1);
  lcd.write(byte(3));
  lcd.setCursor(6, 1);
  lcd.write(byte(4));
  lcd.setCursor(5, 1);
  lcd.write(byte(5));
}

void imageleft() {

  byte image22[8] = {B00011, B00111, B01111, B11111, B11111, B01111, B00111, B00011};
  byte image23[8] = {B00000, B00000, B00000, B11111, B11111, B00000, B00000, B00000};
  byte image24[8] = {B00000, B00000, B00000, B11111, B11111, B00000, B00000, B00000};
  byte image25[8] = {B00000, B00000, B00000, B11111, B11111, B00000, B00000, B00000};
  byte image27[8] = {B00000, B00000, B00000, B11111, B11111, B00000, B00000, B00000};
  byte image26[8] = {B00000, B00000, B00000, B11111, B11111, B00000, B00000, B00000};

  lcd.createChar(0, image22);
  lcd.createChar(1, image23);
  lcd.createChar(2, image24);
  lcd.createChar(3, image25);
  lcd.createChar(4, image27);
  lcd.createChar(5, image26);

  lcd.setCursor(5, 1);
  lcd.write(byte(0));
  lcd.setCursor(6, 1);
  lcd.write(byte(1));
  lcd.setCursor(7, 1);
  lcd.write(byte(2));
  lcd.setCursor(8, 1);
  lcd.write(byte(3));
  lcd.setCursor(10, 1);
  lcd.write(byte(4));
  lcd.setCursor(9, 1);
  lcd.write(byte(5));
}

