#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <max6675.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

// put pin declarations here:
int thermoDO = 19;
int thermoCS = 23;
int thermoCLK = 5;
int relay = 32;

// put value variables here:
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

float temp = 0.0;
float maxTemp = 32.0;
float minTemp = 30.0;

// put object declarations here:
MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// put function declarations here:


void setup() {
  // Begin Serial
  Serial.begin(115200);
  Serial.println("Serial Started...");

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  //display.display();
  //delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();

  // Show text on the screen
  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0,0);
  display.println("Alarm Chiller");
  display.display();
  delay(2000); // Pause for 2 seconds
  display.clearDisplay();
  

  pinMode(relay, OUTPUT);
  digitalWrite(relay, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  
}

// put function definitions here: