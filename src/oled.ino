#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>

// Replace with your network details
const char* ssid = "fireplace";
const char* password = "";

// OLED display settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
#define OLED_ADDRESS  0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// NTP Client setup
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", -5 * 3600, 60000); // New York UTC offset -5 hours

void setup() {
  Serial.begin(115200);

  // Start I2C communication for OLED
  Wire.begin(D6, D5); // GPIO12, GPIO14 respectively
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");

  // Start the NTP client
  timeClient.begin();
  // Uncomment and adjust the following line for daylight saving time if necessary
  // timeClient.setTimeOffset(-4 * 3600); // EDT offset
}

void loop() {
  timeClient.update();

  // Get the current hour, minute and second from the NTP client
  int currentHour = timeClient.getHours();
  String am_pm = "AM";
  
  if (currentHour >= 12) {
    am_pm = "PM";
    if (currentHour > 12) currentHour -= 12; // Convert to 12-hour format
  }
  if (currentHour == 0) {
    currentHour = 12; // Midnight is shown as 12AM
  }

  // Construct the time string
  String currentTime = String(currentHour < 10 ? "0" : "") + String(currentHour) + ":" +
                       String(timeClient.getMinutes() < 10 ? "0" : "") + String(timeClient.getMinutes()) + ":" +
                       String(timeClient.getSeconds() < 10 ? "0" : "") + String(timeClient.getSeconds()) + " " + am_pm;

  // Display the time on the OLED
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("Time at the Farm:");
  display.setTextSize(1.5);
  display.setCursor(0, 20);
  display.println(currentTime);
  display.display();

  delay(1000);
}
