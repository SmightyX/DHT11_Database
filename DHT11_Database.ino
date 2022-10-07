#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "DHT.h"

#define DHTPIN D4     

#define DHTTYPE DHT11   // DHT 11
// #define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)
 
const char* ssid = "PUSBANGNIS"; //Your SSID WiFi
const char* password = "pusbangnisuns123"; //Your WiFi Password

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  //Serial.println(F("DHTxx test!"));
  dht.begin();
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
    int i=0;
    while(WiFi.status() != WL_CONNECTED){ 
      Serial.print(".");
      delay(1000);     
    } 
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  delay(2000); 
}

void loop() {
  double kelembaban = dht.readHumidity();
  // Read temperature as Celsius (the default)
  double suhu = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(kelembaban) || isnan(suhu)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  Serial.print(F("Temperature:"));
  Serial.print(suhu);
  Serial.print(F("°C   Humidity:"));
  Serial.print(kelembaban);
  Serial.println(F("%"));
  
  if ((WiFi.status() == WL_CONNECTED)) {
    WiFiClient client;
    HTTPClient http;
    
    String address;
    //equate with your computer's IP address and your directory application
    //check your IP go to CMD -> "ipconfig" Or C:\xampp\htdocs\arducoding_tutorial\nodemcu_log\webapi\api\create.php
    address ="http://10.101.2.149/testiot/dht.php?temperature=";
    address += String(suhu);
    address += "&humidity="; 
    address += String(kelembaban) ;
      
    http.begin(client,address);  //Specify request destination
    int httpCode = http.GET();//Send the request
    String payload;  
    if (httpCode > 0) { //Check the returning code    
        payload = http.getString();   //Get the request response payload
        payload.trim();
        if( payload.length() > 0 ){
           Serial.println(payload + "\n");
        }
    }
    
    http.end();   //Close connection
  }else{
    Serial.print("Not connected to wifi ");Serial.println(ssid);
  }
  delay(6000); //interval 60s
}