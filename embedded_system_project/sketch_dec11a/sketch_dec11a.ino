#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {9, 10, 11, 12};
byte colPins[COLS] = {5, 6, 7, 8};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

const int buttonPin = 2;
const int buzzerPin = 4;

unsigned long timerDuration = 300000;
unsigned long startTime = 0;
bool timerActive = false;
bool settingTime = false;
String timeInput = "";

bool lastButtonState = HIGH;
bool buttonState = HIGH;

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);
  
  lcd.begin();
  lcd.backlight();
  lcd.clear();
  
  showWelcomeScreen();
  
  Serial.begin(9600);
  Serial.println("Sleep Timer with Keypad Ready");
}

void loop() {
  char key = keypad.getKey();
  
  if (key) {
    handleKeypadInput(key);
  }
  
  buttonState = digitalRead(buttonPin);
  
  if (buttonState == LOW && lastButtonState == HIGH) {
    delay(50);
    
    if (!timerActive && !settingTime) {
      enterTimeSettingMode();
    } else if (timerActive) {
      stopTimer();
    }
  }
  
  lastButtonState = buttonState;
  
  if (timerActive) {
    unsigned long elapsedTime = millis() - startTime;
    unsigned long remainingTime = timerDuration - elapsedTime;
    
    if (elapsedTime % 1000 < 100) {
      int minutesLeft = remainingTime / 60000;
      int secondsLeft = (remainingTime % 60000) / 1000;
      
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Time Remaining:");
      lcd.setCursor(0, 1);
      
      if (minutesLeft < 10) lcd.print("0");
      lcd.print(minutesLeft);
      lcd.print(":");
      if (secondsLeft < 10) lcd.print("0");
      lcd.print(secondsLeft);
      
      int progress = map(elapsedTime, 0, timerDuration, 0, 8);
      lcd.setCursor(8, 1);
      for (int i = 0; i < progress; i++) {
        lcd.print((char)255);
      }
    }
    
    if (elapsedTime >= timerDuration) {
      timerFinished();
    }
  }
}

void handleKeypadInput(char key) {
  if (!settingTime) return;
  
  if (key >= '0' && key <= '9') {
    if (timeInput.length() < 4) {
      timeInput += key;
      updateTimeInputDisplay();
    }
  }
  else if (key == '#') {
    if (timeInput.length() > 0) {
      int minutes = timeInput.toInt();
      if (minutes > 0 && minutes <= 9999) {
        timerDuration = (unsigned long)minutes * 60000;
        startTimer();
        settingTime = false;
        timeInput = "";
      } else {
        showError("Invalid time!");
        delay(1500);
        enterTimeSettingMode();
      }
    }
  }
  else if (key == '*') {
    if (timeInput.length() > 0) {
      timeInput.remove(timeInput.length() - 1);
      updateTimeInputDisplay();
    } else {
      settingTime = false;
      timeInput = "";
      showWelcomeScreen();
    }
  }
}

void enterTimeSettingMode() {
  settingTime = true;
  timeInput = "";
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Enter minutes:");
  lcd.setCursor(0, 1);
  lcd.print("# OK  * Back");
  
  Serial.println("Time setting mode");
  Serial.println("Enter minutes (# to confirm, * to cancel)");
}

void updateTimeInputDisplay() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Enter mins: ");
  lcd.print(timeInput);
  lcd.setCursor(0, 1);
  lcd.print("# OK  * Back");
}

void showWelcomeScreen() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Sleep Timer");
  lcd.setCursor(0, 1);
  lcd.print("Press Button");
}

void showError(String message) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Error:");
  lcd.setCursor(0, 1);
  lcd.print(message);
}

void startTimer() {
  timerActive = true;
  startTime = millis();
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Timer Started!");
  lcd.setCursor(0, 1);
  lcd.print("Duration: ");
  lcd.print(timerDuration / 60000);
  lcd.print(" min");
  delay(1500);
  
  Serial.print("Timer started - ");
  Serial.print(timerDuration / 60000);
  Serial.println(" minutes");
}

void stopTimer() {
  timerActive = false;
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Timer Stopped");
  lcd.setCursor(0, 1);
  lcd.print("Press to restart");
  
  Serial.println("Timer stopped");
  
  delay(2000);
  showWelcomeScreen();
}

void timerFinished() {
  timerActive = false;
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Time's Up!");
  lcd.setCursor(0, 1);
  lcd.print("Sleep Mode");
  
  Serial.println("Timer finished! Sleep mode activated");
  
  for (int i = 0; i < 3; i++) {
    digitalWrite(buzzerPin, HIGH);
    delay(300);
    digitalWrite(buzzerPin, LOW);
    delay(300);
  }
  
  delay(2000);
  
  showWelcomeScreen();
}