#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <TimeLib.h>  // Include TimeLib to handle date and time functions

// Replace with your network details
const char* ssid = "shop2";
const char* password = "mine0313";

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
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("Wi-Fi Connected");
  display.display();
  delay(2000); // Show the message for 2 seconds

  // Start the NTP client
  timeClient.begin();
  // Uncomment and adjust the following line for daylight saving time if necessary
  // timeClient.setTimeOffset(-4 * 3600); // EDT offset for daylight saving time
}

void loop() {
  timeClient.update();
  unsigned long epochTime = timeClient.getEpochTime();

  // Parse the NTP time to extract date and time components
  int currentHour = hour(epochTime);
  int currentMinute = minute(epochTime);
  int currentSecond = second(epochTime);
  int dayOfWeek = weekday(epochTime);
  int currentDay = day(epochTime);
  int currentMonth = month(epochTime);
  int currentYear = year(epochTime);

  // Convert to 12-hour format and set AM/PM
  String am_pm = "AM";
  if (currentHour >= 12) {
    am_pm = "PM";
    if (currentHour > 12) currentHour -= 12;
  }
  if (currentHour == 0) currentHour = 12;

  // Clear the display buffer
  display.clearDisplay();

  // Display time
  display.setTextSize(1);
  display.setCursor(0,0);
  display.print("Time: ");
  if (currentHour < 10) display.print('0');
  display.print(currentHour);
  display.print(":");
  if (currentMinute < 10) display.print('0');
  display.print(currentMinute);
  display.print(":");
  if (currentSecond < 10) display.print('0');
  display.print(currentSecond);
  display.print(" ");
  display.println(am_pm);

  // Display the day of the week
  display.print("Day: ");
  display.println(dayStr(dayOfWeek));

  // Display the date in MM/DD/YYYY format
  display.print("Date: ");
  if (currentMonth < 10) display.print('0');
  display.print(currentMonth);
  display.print("/");
  if (currentDay < 10) display.print('0');
  display.print(currentDay);
  display.print("/");
  display.println(currentYear);

  // Send the buffer to the display
  display.display();

  delay(1000); // Wait for a second before looping again
}
