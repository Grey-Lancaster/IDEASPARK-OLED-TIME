#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <TimeLib.h>
#include <Fonts/FreeSans9pt7b.h>  // Include the FreeSans9pt7b font

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
  display.setFont(&FreeSans9pt7b);  // Set the custom font
  display.setTextSize(1);
  display.setTextColor(WHITE);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");

  // Display "Wi-Fi Connected" message
  display.clearDisplay();
  display.setCursor(0,13);
  display.println("Connected");
  display.display();
  delay(4000); // Show the message for 2 seconds
    // Display the local IP address
  display.clearDisplay();
  display.setCursor(0,13);
  display.print("IP: ");
  //display.println(WiFi.localIP());
  display.setCursor(0, 30);
  display.print("Gateway: ");
 // display.println(WiFi.gatewayIP());
  display.setCursor(0, 50);
  display.print("DNS: ");
 // display.println(WiFi.dnsIP());
  display.display();
  delay(5000); // Show the IP address for 5 seconds

  // Display the local IP address
  display.clearDisplay();
  display.setCursor(0,13);
  //display.print("IP: ");
  display.println(WiFi.localIP());
  display.setCursor(0, 30);
 // display.print("Gateway: ");
  display.println(WiFi.gatewayIP());
  display.setCursor(0, 50);
 // display.print("DNS: ");
  display.println(WiFi.dnsIP());
  display.display();
  delay(5000); // Show the IP address for 5 seconds

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

  // Set up the display with the custom font and size
  display.setFont(&FreeSans9pt7b);
  display.setTextSize(1);

  // Display time
  display.setCursor(0, 15);
  display.print(currentHour < 10 ? "0" : "");
  display.print(currentHour);
  display.print(":");
  display.print(currentMinute < 10 ? "0" : "");
  display.print(currentMinute);
  display.print(":");
  display.print(currentSecond < 10 ? "0" : "");
  display.print(currentSecond);
  display.print(" ");
  display.println(am_pm);

  // Display the day of the week
  display.setCursor(0, 35);
  display.println(dayStr(dayOfWeek));

  // Display the date in MM/DD/YYYY format
  display.setCursor(0, 60);
  display.print(currentMonth < 10 ? "0" : "");
  display.print(currentMonth);
  display.print("/");
  display.print(currentDay < 10 ? "0" : "");
  display.print(currentDay);
  display.print("/");
  display.println(currentYear);

  // Send the buffer to the display
  display.display();

  delay(1000); // Wait for a second before looping again
}
