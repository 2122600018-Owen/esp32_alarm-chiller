#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// put pin declarations here:
int relay = 32;
int oneWireBus = 5;

// put value variables here:
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

float temp = 0.0;
float maxTemp = 30.0;
float minTemp =25.0;

bool heatWarning = false;

// put object declarations here:
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);

// put function declarations here:


void setup() {
  // Begin Serial
  Serial.begin(115200);
  Serial.println("Serial Started...");
  
  // Begin oneWire sensors
  sensors.begin();
  
  // Begin SSD1306 OLED display
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Clear the buffer
  display.clearDisplay();

  // Show text on the screen
  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0,0);
  display.println("Alarm Chiller");
  display.display();
  delay(2000); // Pause for 2 seconds
  
  pinMode(relay, OUTPUT);
  digitalWrite(relay, LOW);
}

void loop() {
  // clear the display for next loop
  display.clearDisplay();

  // basic readout serial monitor print from DS18B20
  sensors.requestTemperatures(); 
  float temperatureC = sensors.getTempCByIndex(0);
  Serial.print(temperatureC);
  Serial.println("ºC");

  // display current temperature and thresholds
  display.setTextSize(2);      // Larger 2:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0,8);
  display.print(temperatureC);
  display.print((char)247);display.println("C");
  display.display();

  
  // temperature threshold checking
  if(temperatureC >= maxTemp){
    digitalWrite(relay, HIGH);
    heatWarning = 1;
  }

  if(heatWarning){
    display.setTextSize(1);      // Normal 1:1 pixel scale
    display.setCursor(86,8);
    display.print("|");
    display.setCursor(display.getCursorX() + 3,display.getCursorY());
    display.print("HEAT");
    display.setCursor(display.getCursorX() + 3,display.getCursorY());
    display.print("|");
    display.setCursor(86,16);
    display.print("|ALERT|");
    Serial.println("HI-TEMP");
    display.display();
  }

  // For the MAX6675 to update, you must delay AT LEAST 250ms between reads!
  delay(1000);
}

// put function definitions here: