#include <Wire.h>
#include <LiquidCrystal_I2C.h>
 
// LCD configuration
LiquidCrystal_I2C lcd(0x27, 20, 4);
 
// Pins configuration
const int button1Pin = 18; // Button 1 for navigation
const int button2Pin = 19; // Button 2 for selection
const int ledPin = 2;      // LED pin
const int buzzerPin = 4;   // Buzzer pin
 
// Variables
unsigned long previousMillis = 0; // Timer
unsigned long interval = 1000;    // 1-second interval for alerts
int currentProgram = 0;           // Selected program index
bool testMode = false;
bool alertActive = false;
 
// Medication schedules (example times in seconds)
const int schedules[][2] = {
  {8, 0},  // Program 1: 8:00 AM
  {12, 0}, // Program 2: 12:00 PM
  {18, 0}, // Program 3: 6:00 PM
};
 
void setup() {
  lcd.begin();
  lcd.backlight();
  pinMode(button1Pin, INPUT_PULLUP);
  pinMode(button2Pin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
 
  // Startup behavior: Turn on LED and buzzer
  digitalWrite(ledPin, HIGH); // Turn on LED
  tone(buzzerPin, 1000);      // Turn on buzzer with 1kHz sound
  delay(2000);                // Keep them on for 2 seconds
  digitalWrite(ledPin, LOW);  // Turn off LED
  noTone(buzzerPin);          // Turn off buzzer
 
  // Welcome message
  lcd.setCursor(0, 0);
  lcd.print("Medication Timer");
  delay(2000);
  lcd.clear();
}
 
void loop() {
  // Display menu
  displayMenu();
 
  // Check buttons for navigation
  if (digitalRead(button1Pin) == LOW) {
    currentProgram = (currentProgram + 1) % (sizeof(schedules) / sizeof(schedules[0]));
    delay(300); // Debounce
  }
 
  if (digitalRead(button2Pin) == LOW) {
    startProgram();
    delay(300); // Debounce
  }
 
  // Test mode
  if (testMode) {
    testComponents();
  }
 
  // Handle active alert
  if (alertActive) {
    handleAlert();
  }
}
 
void displayMenu() {
  lcd.setCursor(0, 0);
  lcd.print("Select Program:");
  lcd.setCursor(0, 1);
  lcd.print("Program ");
  lcd.print(currentProgram + 1);
  lcd.setCursor(0, 2);
  lcd.print(schedules[currentProgram][0]);
  lcd.print(":");
  lcd.print(schedules[currentProgram][1]);
}
 
void startProgram() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Program Started");
  lcd.setCursor(0, 1);
  lcd.print("Alert at ");
  lcd.print(schedules[currentProgram][0]);
  lcd.print(":");
  lcd.print(schedules[currentProgram][1]);
  delay(2000);
  lcd.clear();
  alertActive = true;
}
 
void handleAlert() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    digitalWrite(ledPin, !digitalRead(ledPin)); // Blink LED
    tone(buzzerPin, 1000);                      // Buzzer sound
    delay(500);
    noTone(buzzerPin);
  }
}
 
void testComponents() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Testing Components");
 
  lcd.setCursor(0, 1);
  lcd.print("LED...");
  digitalWrite(ledPin, HIGH);
  delay(1000);
  digitalWrite(ledPin, LOW);
 
  lcd.setCursor(0, 2);
  lcd.print("Buzzer...");
  tone(buzzerPin, 1000);
  delay(1000);
  noTone(buzzerPin);
 
  lcd.setCursor(0, 3);
  lcd.print("Done!");
  delay(2000);
  lcd.clear();
 
  testMode = false;
}
