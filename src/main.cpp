#include <Arduino.h>
#include <WiFi.h>
#include <Adafruit_BME280.h>
#include <Adafruit_Sensor.h>
#include <ESPAsyncWebServer.h>
#include <StringArray.h>

#define LED 2

// Set your access point network credentials
const char* ssid = "ESP32-Access-Point";
const char* password = "123456789";
int profil=0;
String prof= String(profil);
// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

Adafruit_BME280 bme; // I2C

String readTemp()
{
  return String(bme.readTemperature());
  //return String(1.8 * bme.readTemperature() + 32);
}

String readHumi()
{
  return String(bme.readHumidity());
}

String readPres()
{
  return String(bme.readPressure() / 100.0F);
}

void setup()
{
  pinMode(LED,OUTPUT);
  digitalWrite(LED,LOW);
  // Serial port for debugging purposes
  Serial.begin(115200);
  Serial.println();
  
  // Setting the ESP as an access point
  Serial.print("Setting AP (Access Point)…");
  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readTemp().c_str());
  });
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readHumi().c_str());
  });
  server.on("/pressure", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readPres().c_str());
  });

  /*server.on("/profil/01", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    profil = 1;
    request->send_P(200, "text/plain", "Le profil actif est maintenant 01");
  });*/

  /*server.on("/profil/00", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    profil = 0;
    request->send_P(200, "text/plain", "Le profil actif est maintenant 00");
  });*/

  server.on("/profiltype", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    if(profil==0) request->send_P(200, "text/plain", "00");
    if(profil==1) request->send_P(200, "text/plain", "01");
  });

  server.on("/status", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    String status = "Profil actuel : "+String(profil)+"\n\rTemperature : "+String(readTemp())+"\n\rHumidite : "+String(readHumi())+"\n\rPression : "+String(readPres());
    request->send_P(200, "text/plain", status.c_str());
  });

  server.on(
    "/profil",
    HTTP_POST,
    [](AsyncWebServerRequest * request){},
    NULL,
    [](AsyncWebServerRequest * request, uint8_t *data, size_t len, size_t index, size_t total) 
    {
      String type = "";
      for (size_t i = 0; i < len; i++)
      {
        switch(data[i])
        {
          case 'a': type+= 'a';break;
          case 'b': type+= 'b';break;
          case 'c': type+= 'c';break;
          case 'd': type+= 'd';break;
          case 'e': type+= 'e';break;
          case 'f': type+= 'f';break;
          case 'g': type+= 'g';break;
          case 'h': type+= 'h';break;
          case 'i': type+= 'i';break;
          case 'j': type+= 'j';break;
          case 'k': type+= 'k';break;
          case 'l': type+= 'l';break;
          case 'm': type+= 'm';break;
          case 'n': type+= 'n';break;
          case 'o': type+= 'o';break;
          case 'p': type+= 'p';break;
          case 'q': type+= 'q';break;
          case 'r': type+= 'r';break;
          case 's' :type+= 's';break;
          case 't': type+= 't';break;
          case 'u': type+= 'u';break;
          case 'v': type+= 'v';break;
          case 'w': type+= 'w';break;
          case 'x': type+= 'x';break;
          case 'y': type+= 'y';break;
          case 'z': type+= 'z';break;
        }
      }
      Serial.println(type);
      if(type=="banane") profil = 1;
      if(type=="algue") profil = 0;
      Serial.print(profil);
      Serial.println();
      request->send(200);
    });

  bool status;
  // default settings
  // (you can also pass in a Wire library object like &Wire2)
  status = bme.begin(0x77);  
  if (!status) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }
  
  // Start server
  server.begin();
  digitalWrite(LED,HIGH);

}

void loop()
{  
}