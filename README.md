# ⏰ Weather Clock Display
A real-time weather display system that shows current weather conditions, temperature and time for the 5 most famous cities around the world. Built with ESP32, a TFT display and the OpenWeather API, the system features button-controlled city switching and automatic weather updates with dynamic weather icons.

## ⚙️ Installation & Usage

### Required Libraries
Install the following libraries:
- `TFT_eSPI` - For TFT display control
- `WiFi` - For WiFi connectivity
- `HTTPClient` - For API requests
- `ArduinoJson` - For JSON parsing
- `ezTime` - For timezone management

### Software Installation
1. Download or clone the `weather_clock.ino` file and weather icon header files
2. Open the file in Arduino IDE
3. Update the following credentials in the code:
```cpp
   const char* ssid = "YOUR_WIFI_SSID";
   const char* password = "YOUR_WIFI_PASSWORD";
   const String api_key = "YOUR_OPENWEATHER_API_KEY";
```
4. Configure TFT_eSPI library for your display model (edit User_Setup_Select.h)
5. Select your ESP32 board: `Tools > Board > ESP32 Dev Module`
6. Select the correct COM port: `Tools > Port > [Your Port]`
7. Upload the code to your ESP32

### How to Use
1. Power on the ESP32 and wait for WiFi connection
2. The display will show weather for the default city (London)
3. Current display shows:
   - Date (DD-MM-YYYY format)
   - Time (24-hour format)
   - City name
   - Weather description
   - Temperature in Celsius
   - Weather icon (sun, clouds, rain, snow, thunder or moon)
4. Press the button to cycle through cities:
   `London → Paris → New York → Tokyo → Dubai → (repeats)`
5. Weather data automatically updates every 60 seconds
6. Time adjusts automatically based on city timezone

## 👀 Demo

<video src="https://github.com/user-attachments/assets/f079cb47-1d85-4928-9f8b-fdee0c8efca3" muted style="width: 500px;">
</video>


## 🎯 Features
- **Auto-reconnect** - Maintains WiFi connection 
- **Multi-city support** - Five major cities with automatic timezone adjustment
- **Real-time updates** - Weather data refreshes every 60 seconds
- **Button control** - Navigation between cities with single button
- **Dynamic icons** - Weather-appropriate graphics that change with conditions
- **Day/night awareness** - Different icons for clear weather based on local time

## 💡 Future Improvements
- Expand city list 
- Include sunrise/sunset times with visual indicators
- Add temperature unit toggle (Celsius/Fahrenheit)

## 📚 Lessons Learnt
- TFT displays require specific library configuration for different hardware models
- JSON parsing needs appropriate buffer sizing to prevent data loss
- Weather icon conversion to header files enables efficient storage and display
