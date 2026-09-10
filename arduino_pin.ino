#include <LiquidCrystal.h>

int cursorPos = 0;
int dig[4] = {0, 0, 0, 0};

// LCD Pins: RS=12, Enable=11, D4=5, D5=4, D6=3, D7=2
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void resetUI() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Enter password:");
  
  cursorPos = 0;
  for (int i = 0; i < 4; i++) {
    dig[i] = 0;
  }

  lcd.setCursor(6, 1);
  lcd.print("0000");
  lcd.setCursor(6, 1);
  lcd.cursor();
}

void setup() {
  pinMode(7, INPUT_PULLUP); // Joystick SW button

  lcd.begin(16, 2);
  resetUI();
}

void loop() {
  // 1. Check if Joystick Button is pressed (SW)
  if (digitalRead(7) == LOW) {
    checkCode();
    delay(2000); // Hold result for 2 seconds
    resetUI();   // Reset UI safely
    return;
  }

  // 2. Read Joystick Axes
  int sensorX = analogRead(A0);
  int sensorY = analogRead(A1);

  // Convert raw readings (0-1023) to angles (0-180)
  float angleX = sensorX * (180.0 / 1023.0);
  float angleY = sensorY * (180.0 / 1023.0);

  // 3. Move Cursor Left/Right
  if (angleX > 110) {
    moveRight();
    delay(300); // Debounce delay
  } else if (angleX < 60) {
    moveLeft();
    delay(300);
  } 
  // 4. Change Number Up/Down
  else if (angleY > 110) {
    decrease(cursorPos);
    delay(250);
  } else if (angleY < 60) {
    increase(cursorPos);
    delay(250);
  }
}

void moveRight() {
  if (cursorPos == 3) {
    cursorPos = 0;
  } else {
    cursorPos++;
  }
  lcd.setCursor(6 + cursorPos, 1);
}

void moveLeft() {
  if (cursorPos == 0) {
    cursorPos = 3;
  } else {
    cursorPos--;
  }
  lcd.setCursor(6 + cursorPos, 1);
}

void increase(int pos) {
  dig[pos] = (dig[pos] == 9) ? 0 : dig[pos] + 1;
  lcd.print(dig[pos]);
  lcd.setCursor(6 + pos, 1);
}

void decrease(int pos) {
  dig[pos] = (dig[pos] == 0) ? 9 : dig[pos] - 1;
  lcd.print(dig[pos]);
  lcd.setCursor(6 + pos, 1);
}

void checkCode() {
  lcd.noCursor();
  lcd.setCursor(0, 1);
  lcd.print("                "); // Clear line 2

  // Password set to 1975
  if (dig[0] == 1 && dig[1] == 9 && dig[2] == 7 && dig[3] == 5) {
    lcd.setCursor(4, 1);
    lcd.print("Correct!");
  } else {
    lcd.setCursor(5, 1);
    lcd.print("Wrong!  ");
  }
}