// Libraries
#include <EEPROM.h>                  // Memory library
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>            // OLED display library
#include <Adafruit_SSD1306.h>
#include <Keypad.h>                  // Keypad library

// OLED display configuration
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Keypad matrix configuration
const byte ROWS = 4;               // Four rows
const byte COLS = 3;               // Three columns
char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
byte rowPins[ROWS] = {6, 7, 8, 9}; // Connect to the row pinouts of the keypad
byte colPins[COLS] = {10, 11, 13}; // Connect to the column pinouts of the keypad
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// Pin configurations for the relay and soil moisture sensor
const int relayPin = 4;            // Output pin for the relay 
const int waterSensorPin = 5;      // Input pin for the soil moisture sensor 
const int moistureAnalogPin = A0;  // Analog input pin for the soil moisture sensor

// Variables for sensor readings and display
int value;                        // Variable to store the ADC value
float voltage;                    // Variable to store the calculated voltage
String currentNumber = "";        // Holds the current number entered from the keypad

void setup() {
  // Initialize serial communication
  Serial.begin(115200);

  // Set up the relay and sensor pins
  pinMode(relayPin, OUTPUT);      // Set relay pin as output
  pinMode(waterSensorPin, INPUT); // Set sensor pin as input
  pinMode(moistureAnalogPin, INPUT); // Set analog pin as input

  // Set up the OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);  // Don't proceed, loop forever
  }
  display.clearDisplay();          // Clear the display on startup
  display.display();
  introAnimation();
  loadingCircle(10);
  mainPage();  
}

void loop() {

  // Wait until a key is pressed
  char key = keypad.getKey();
  if (key) {
    //Clear current display
    display.clearDisplay();
    // Display the corresponding message based on the key pressed
    switch (key) {
    case '1':  // If '1' is pressed
      loadingCircle(0.1);
      display.setCursor(0, 0);
      display.setTextSize(1.5);
      display.print("Enter a volume:");
      display.setTextSize(1);
      display.setCursor(0, 55);
      display.print("Press * to go back to main menu");
      break;

    case '2':  // If '2' is pressed
      loadingCircle(0.1);
      display.setCursor(0, 0);
      display.setTextSize(1.5);
      display.print("Moisture Value: ");
      display.print(String(voltage));  // Display the moisture voltage
      display.setTextSize(1);
      display.setCursor(0, 55);
      display.print("Press * to go back to main menu");
      break;

    case '3':  // If '3' is pressed
      loadingCircle(0.1);
      display.setCursor(0, 0);
      display.setTextSize(1.5);
      display.print("Exit");
      display.setCursor(0, 55);
      display.setTextSize(1);
      display.print("Press * to go back to main menu");
      break;

    default:
      break;
  }
    // Update the display
    display.display();
  }

  // Wait for the * key to go back to the main page
  char keyBack = keypad.getKey();
  if (keyBack == '*') {
    display.clearDisplay();
    mainPage();  // Show the main menu page again
    display.display();  // Update the display
  }

  // Other logic for soil moisture sensor and relay control
  int sensorValue = digitalRead(waterSensorPin);
  value = analogRead(moistureAnalogPin);  // Read the ADC value
  voltage = map(value, 0, 1023, 0, 3300) / 1000.0; // Convert ADC value to voltage

  // Control the relay based on the sensor state
  if (sensorValue == LOW) { 
    // Soil is wet, turn off the pump
    digitalWrite(relayPin, LOW);    
  } else { 
    // Soil is dry, turn on the pump
    digitalWrite(relayPin, HIGH);   
  }
}

//----------------------------------------------A N I M A T I O N--------------------------------------------//

void introAnimation() {
  // Clear display
  display.clearDisplay();
  
  // Display "Irrigation System" with larger text
  display.setTextSize(2); // Large text size for title
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Irrigation");
  display.setCursor(0, 20);
  display.println("System");

  // Display "powered by AFA" with smaller text
  display.setTextSize(1); // Smaller text size for the subheading
  display.setCursor(0, 50);
  display.println("powered by AFA");

  // Display a simple plant symbol using custom characters
  display.setTextSize(1);  // Normal text size for the symbol
  display.setCursor(110, 30);

  display.display(); // Update the display

  delay(2000); // Keep the intro animation for 3 seconds

  // Optionally clear the display after the animation
  display.clearDisplay();
  display.display();
}

void mainPage() {
  int boxSize = 30;  // Size of the box
  int margin = 10;   // Margin between boxes
  int startX = (SCREEN_WIDTH - (boxSize * 3 + margin * 2)) / 2; // Starting X position for the first box
  int startY = (SCREEN_HEIGHT - boxSize) / 2; // Y position for the boxes (centered vertically)

  // Draw the first square box
  display.drawRect(startX, startY, boxSize, boxSize, SSD1306_WHITE);  // Draw the box border
  display.setTextSize(2);  // Bold text size
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(startX + (boxSize / 2) - 6, startY + (boxSize / 2) - 8);  // Adjust text position
  display.print("1");  // Draw the number 1 in the first box

  // Draw the second square box
  display.drawRect(startX + boxSize + margin, startY, boxSize, boxSize, SSD1306_WHITE);  // Draw the second box border
  display.setCursor(startX + boxSize + margin + (boxSize / 2) - 6, startY + (boxSize / 2) - 8);  // Adjust text position
  display.print("2");  // Draw the number 2 in the second box

  // Draw the third square box
  display.drawRect(startX + 2 * (boxSize + margin), startY, boxSize, boxSize, SSD1306_WHITE);  // Draw the third box border
  display.setCursor(startX + 2 * (boxSize + margin) + (boxSize / 2) - 6, startY + (boxSize / 2) - 8);  // Adjust text position
  display.print("3");  // Draw the number 3 in the third box

  display.display();  // Update the display to show the boxes and numbers
}


void loadingCircle(float animationDelay) {
  int centerX = SCREEN_WIDTH / 2;  // Center X coordinate of the circle
  int centerY = SCREEN_HEIGHT / 2; // Center Y coordinate of the circle
  int radius = 10;                 // Radius of the circle

  for (int angle = 0; angle < 360; angle += 15) {  // Increment angle in steps
    display.clearDisplay();

    // Draw static circle outline
    display.drawCircle(centerX, centerY, radius, SSD1306_WHITE);

    // Calculate the position of the loading point
    float radian = angle * (PI / 180.0);
    int x = centerX + radius * cos(radian);
    int y = centerY + radius * sin(radian);

    // Draw the loading point
    display.fillCircle(x, y, 2, SSD1306_WHITE);

    display.display();
    delay(animationDelay); // Delay to create animation effect
  }
    display.clearDisplay();
    display.display();
}

void loadingAnimation() {
  unsigned long startTime = millis();  // Record the start time
  unsigned long duration = 5000;  // Set duration for 5 seconds (5000 ms)
  
  int barWidth = 0;  // Initial width of the loading bar
  int barMaxWidth = SCREEN_WIDTH - 20;  // Maximum width of the loading bar
  
  while (millis() - startTime < duration) {  // Continue for 5 seconds
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(10, 10);
    display.print("Loading...");

    // Update the width of the loading bar
    barWidth = map(millis() - startTime, 0, duration, 0, barMaxWidth);

    // Draw the loading bar
    display.drawRect(10, 30, barMaxWidth, 10, SSD1306_WHITE);  // Draw the outer rectangle
    display.fillRect(10, 30, barWidth, 10, SSD1306_WHITE);     // Fill the progress part of the bar

    display.display();  // Update the display
    delay(40);  // Delay to make the animation visible
  }

  display.clearDisplay();
  display.setCursor(10, 30);
  display.display();
}
