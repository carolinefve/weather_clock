// Include libraries
#include <SPI.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <TFT_eSPI.h>
#include <HTTPClient.h>
#include <ArduinoJson.h> 
#include <ezTime.h>
#include "sun.h"
#include "clouds.h"
#include "moon.h"
#include "rain.h"
#include "snow.h"
#include "thunder.h"

// Create TFT_eSPI class
TFT_eSPI tft = TFT_eSPI();   

// WIFI 
const char* ssid = "WIFI";
const char* password = "PASSWORD";

// City information structure
struct City {
  String name;
  String id;         
  String timezone;
};

// Array of cities
City cities[5] = {
  {"London", "2643743", "Europe/London"},
  {"Paris", "2988507", "Europe/Paris"},
  {"New York", "5128581", "America/New_York"}, 
  {"Tokyo", "1850144", "Asia/Tokyo"},
  {"Dubai", "292223", "Asia/Dubai"}
};

// Current selection
int currentCityIndex = 0;
String city = "";
String city_id = "";

// Weather API
const String api_key = "API_KEY";

StaticJsonDocument<1000> doc;

// Weather Variables
String json_data = "";
String description = "";
int temp = 0;

// Initialise time
Timezone myTZ;

// Button variables
#define BUTTON_S1 35
int press1 = 0;

void setup() {

  // Set initial structure
  pinMode(BUTTON_S1, INPUT); 
  Serial.begin(115200);
  Serial.println("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    Serial.print(".");
  }

  Serial.println(" WiFi connected");

  tft.init();
  tft.setRotation(3);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE); 

  waitForSync();
  
  // Set initial city
  city = cities[currentCityIndex].name;
  city_id = cities[currentCityIndex].id;
  myTZ.setLocation(cities[currentCityIndex].timezone);
  
  Serial.println("Initial city: " + city);
  Serial.println("City ID: " + city_id);
  
  // Get initial weather data
  getData();
}

void loop() {

  // Check button state
  int buttonValue = digitalRead(BUTTON_S1);

  if(buttonValue == 0) {
    if(press1 == 0) {
      press1 = 1;
      currentCityIndex = (currentCityIndex + 1) % 5;
      city = cities[currentCityIndex].name;
      city_id = cities[currentCityIndex].id;
      myTZ.setLocation(cities[currentCityIndex].timezone);
      
      Serial.println("Button pressed! New city: " + city);
      Serial.println("City index: " + String(currentCityIndex));
      Serial.println("City ID: " + city_id);
      
      // Get new weather data and update display 
      getData();
      updateDisplay();
    }
  } else press1 = 0;

  // Update weather data every 60 seconds
  static unsigned long lastUpdate = 0;
  if(millis() - lastUpdate >= 60000 || lastUpdate == 0) {
    getData();
    updateDisplay();
    lastUpdate = millis();
  }
  
  // Delay for button responsiveness
  delay(100); 
}

void getData() {
  if ((WiFi.status() == WL_CONNECTED)) { 
 
    HTTPClient http;
    
    // API endpoint 
    String api_endpoint = "https://api.openweathermap.org/data/2.5/weather?id=" + 
                          city_id + "&units=metric&appid=" + api_key;
 
    Serial.println("API Endpoint: " + api_endpoint);
    
    // Check HTTP request
    http.begin(api_endpoint); 
    int httpCode = http.GET();  
    
    Serial.println("HTTP Code: " + String(httpCode));
 
    if (httpCode > 0) { 
 
        json_data = http.getString();
       
        Serial.println("Response: " + json_data);
        
      }
 
    else {
      Serial.println("Error on HTTP request");
      Serial.println("HTTP Code: " + String(httpCode));
    }
 
    http.end(); 
  }

  // Get API data
  char input[1000];
  json_data.toCharArray(input, 1000);
  deserializeJson(doc, input);
  
  int data_temp = doc["main"]["temp"]; 
  String data_description = doc["weather"][0]["main"];

  temp = data_temp;  
  description = data_description; 
}

void updateDisplay() {
  tft.fillScreen(TFT_BLACK);  

  // Get date/time 
  String date = myTZ.dateTime("d-m-Y");
  String time = myTZ.dateTime("H:i");
  int hour = myTZ.dateTime("H").toInt();

  // Display info
  tft.setTextSize(2);
  tft.setCursor(0, 0);
  tft.println(date);

  tft.setTextSize(2);
  tft.setCursor(175, 0);
  tft.println(time);

  tft.setTextSize(3);
  tft.setCursor(10, 30);
  tft.println(city);

  tft.setTextSize(3);
  tft.setCursor(20, 90);
  tft.println(String(temp) + "C");

  tft.setTextSize(2);
  tft.setCursor(20, 70);
  tft.println(description);

  tft.setSwapBytes(true);

  // Display images
  if (description == "Clouds" || description == "Fog" || description == "Mist") {
    tft.pushImage(150, 40, 75, 75, clouds);
  } else if (description == "Rain" || description == "Drizzle") {
    tft.pushImage(150, 40, 75, 75, rain);
  } else if (description == "Thunderstorm" || description == "Tornado") {
    tft.pushImage(150, 40, 75, 75, thunder);
  } else if (description == "Snow") {
    tft.pushImage(150, 40, 75, 75, snow);
  } else if (description == "Clear" && (hour >= 18 || hour < 6)) {
    tft.pushImage(150, 40, 75, 75, moon);
  } else {
    tft.pushImage(150, 40, 75, 75, sun);
  }
}