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
float maxTemp = 30.0;
float minTemp =25.0;

bool heatWarning = false;

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
  

  pinMode(relay, OUTPUT);
  digitalWrite(relay, LOW);
}

void loop() {
  // clear the display for next loop
  display.clearDisplay();

  // basic readout test, just print the current temp on serial monitor
  Serial.print("C = ");
  Serial.println(thermocouple.readCelsius());

  // display current temperature and thresholds
  //display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextSize(2);      // Larger 2:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0,8);
  //display.print("Temp: ");
  display.print(thermocouple.readCelsius());
  display.print((char)247);display.println("C");
  //display.print("Max : ");
  //display.print(maxTemp);
  //display.println(" C");
  //display.print("Min : ");
  //display.print(minTemp);
  //display.println(" C");
  display.display();

  
  // temperature threshold checking
  if (thermocouple.readCelsius() >= maxTemp) {
    digitalWrite(relay, HIGH);
    heatWarning = 1;
  }
  //else if(thermocouple.readCelsius() <= minTemp){
  //  digitalWrite(relay, LOW);
  //  //heatWarning = 0;
  //  Serial.println("LO-TEMP");
  //}
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
    //digitalWrite(relay, HIGH);
    //delay(1000);
    //digitalWrite(relay, LOW);
  }

  // For the MAX6675 to update, you must delay AT LEAST 250ms between reads!
  delay(1000);
}

// put function definitions here: