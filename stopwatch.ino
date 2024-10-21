#include <LiquidCrystal.h> 

// Define pins for button and LEDs
#define BUTTON2 2
#define BUTTON1 3
#define LED1 10
#define LED2 9
#define LED3 8

// Define LCD pins
const int RS = 11, EN = 12, D4 = 4, D5 = 5, D6 = 6, D7 = 7;
LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);  // Initialize the LCD

// State flags and timing variables
bool counting = false;
bool onPause = false;
bool btn1flag = false;
bool btn2flag = false;

unsigned long startTime = 0;
unsigned long elapsedTime = 0;
unsigned long lapTime = 0;
unsigned long totalTime = 0;

unsigned long minutes, seconds, milliseconds;

// Constants
const unsigned long longPressDuration = 2000;  // 2 seconds for a long press

void setup() {
  lcd.begin(16, 2);  // Set up the LCD's number of columns and rows
  lcd.clear();

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(BUTTON1, INPUT);  // Set BUTTON1 and BUTTON2 as INPUT
  pinMode(BUTTON2, INPUT);

  lcd.setCursor(0, 0);
  lcd.print("Stopwatch Ready");
}

void loop() {
  unsigned long currentMillis = millis();  // Get the current time

  int btn1State = digitalRead(BUTTON1);  // Read button state for BUTTON1
  int btn2State = digitalRead(BUTTON2);  // Read button state for BUTTON2

  // Handle BUTTON1 (Start/Pause/Resume)
  if (btn1State && !btn1flag) {
    btn1flag = true;
  }
  
  if (!btn1State && btn1flag) {
    btn1flag = false;

    if (!counting && !onPause) {  // Start
      startTime = currentMillis;  // Start counting
      counting = true;
      lcd.clear();  // Clear the display to start fresh
    } 
    else if (counting && !onPause) {  // Pause
      elapsedTime += (currentMillis - startTime);  // Add elapsed time
      counting = false;
      onPause = true;
    } 
    else if (onPause) {  // Resume
      startTime = currentMillis;  // Set start time again
      counting = true;
      onPause = false;
    }
  }

  // Handle BUTTON2 for lap time (only if the stopwatch is running)
  if (btn2State && !btn2flag) {
    btn2flag = true;
    if (counting) {  // Only record lap time when counting
      lapTime = (currentMillis - startTime) + elapsedTime;  // Capture lap time
      displayLapTime(lapTime);  // Display the lap time immediately
    }
  }

  if (!btn2State && btn2flag) {
    btn2flag = false;
  }

  // Update the display if counting
  if (counting) {
    totalTime = (currentMillis - startTime) + elapsedTime;  // Total time
    updateDisplay(totalTime);
  }
  
  // Display paused time on the first row
  if (onPause) {
    displayPausedTime(elapsedTime);  // Show paused time when not counting
  }
}

// Function to update the LCD display while counting
void updateDisplay(unsigned long totalTime) {
  milliseconds = totalTime % 1000;
  seconds = (totalTime / 1000) % 60;
  minutes = totalTime / 60000;

  lcd.setCursor(0, 0);  // First row
  lcd.print("Time: ");
  lcd.print(minutes);    
  lcd.print(":");
  lcd.print(seconds);
  lcd.print(":");
  lcd.print(milliseconds);
}

// Function to display paused time on the first row
void displayPausedTime(unsigned long pauseTime) {
  unsigned long pauseMilliseconds = pauseTime % 1000;
  unsigned long pauseSeconds = (pauseTime / 1000) % 60;
  unsigned long pauseMinutes = pauseTime / 60000;

  lcd.setCursor(0, 0);  // Set cursor to the first row
  lcd.print("Paused: ");
  lcd.print(pauseMinutes);    
  lcd.print(":");
  lcd.print(pauseSeconds);
  lcd.print(":");
  lcd.print(pauseMilliseconds);
}

// Function to display lap time on the second row
void displayLapTime(unsigned long lapTime) {
  unsigned long lapMilliseconds = lapTime % 1000;
  unsigned long lapSeconds = (lapTime / 1000) % 60;
  unsigned long lapMinutes = lapTime / 60000;

  lcd.setCursor(0, 1);  // Set cursor to the second row
  lcd.print("Lap: ");
  lcd.print(lapMinutes);    
  lcd.print(":");
  lcd.print(lapSeconds);
  lcd.print(":");
  lcd.print(lapMilliseconds);
}

// Function to reset the stopwatch (not triggered in this code)
void resetStopwatch() {
  startTime = 0;
  elapsedTime = 0;
  lapTime = 0;
  counting = false;
  onPause = false;

  lcd.clear();  // Clear the display
  lcd.setCursor(0, 0);
  lcd.print("Timer Reset");
  delay(1000);
  lcd.clear();  // Clear LCD after reset message
}
