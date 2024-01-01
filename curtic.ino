#include <Adafruit_SSD1306.h>
#include <splash.h>

#include <Adafruit_GFX.h>
#include <Adafruit_GrayOLED.h>
#include <Adafruit_SPITFT.h>
#include <Adafruit_SPITFT_Macros.h>
#include <gfxfont.h>

#include <HTTPClient.h>
#include <Adafruit_SSD1306.h>
#include <ArduinoJson.h>
#include <ArduinoJson.hpp>
#include "SPI.h"
#include "WiFi.h"
#define SCREEN_WIDTH 128 // Ширина дисплея в пикселях
#define SCREEN_HEIGHT 64 // Высота дисплея в пикселях

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
#define SCREEN_WIDTH 128 // Ширина дисплея в пикселях
#define SCREEN_HEIGHT 64 // Высота дисплея в пикселях
const char* ssid = "HUAWEI-B311-A07C";
const char* password =  "AMB1GGDA34R";
String ticker = "GRIMACE";
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(1); // установка размера шрифта
  display.setTextColor(WHITE); // установка цвета текста
  display.setCursor(0, 10); // установка курсора
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WIFI");
  while (WiFi.status()!=WL_CONNECTED){
    Serial.print(".");
    delay(500);
  }
  Serial.println("\nConnected to WiFi");

}

void loop() {
  // put your main code here, to run repeatedly:
  if ((WiFi.status()==WL_CONNECTED)) {
    HTTPClient client;

    client.begin("https://min-api.cryptocompare.com/data/price?fsym="+ticker+"&tsyms=USD");
    int httpCode = client.GET();
    if (httpCode>0){
      String payload = client.getString();
      Serial.println("\nstatuscode: "+ String(httpCode));

      DynamicJsonDocument doc(192);
      DeserializationError error = deserializeJson(doc, payload);
      float price = doc["USD"]; // 46.71
      Serial.println(price);
      display.clearDisplay();
      display.setTextWrap(false);
      drawCentreString(ticker, 64, 5);
      display.setTextWrap(false);
      drawCentreString("$"+String(price), 64, 32);
      display.display();
    }
    else{
      Serial.println("http request error");
    }
  }
  else {
    Serial.println("Connection lost");
  }
  delay(3000);
}
void drawCentreString(String buf, int x, int y)
{
  int16_t x1, y1;
  uint16_t w, h;
  display.setTextSize(2);
  display.getTextBounds(buf, x, y, &x1, &y1, &w, &h); //calc width of new string
  display.setCursor(x - w / 2, y);
  display.print(buf);
}