#include <PZEM004Tv30.h>
#include <ESP8266WiFi.h>
#include "FirebaseESP8266.h"

/* Use software serial for the PZEM
 * Pin 11 Rx (Connects to the Tx pin on the PZEM)
 * Pin 12 Tx (Connects to the Rx pin on the PZEM)
*/
#ifndef STASSID
#define STASSID "MBC Laboratory."
#define STAPSK  "123gogoans"
#endif

const char* ssid     = STASSID;
const char* password = STAPSK;

//#define HardwareSerial Serial2(2);

FirebaseData firebaseData;
PZEM004Tv30 pzem(14,12);
const int relay = 5;
String relayStat;

void setup() {
  Serial.begin(115200);
  pinMode (relay, OUTPUT);

  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin("abdimas-94c26.firebaseio.com", "PcZg4HWRj1obVL4PZw8EVFlNKc2TWXGbOh0qzpMw");
  Firebase.reconnectWiFi(true);
  Firebase.setMaxRetry(firebaseData, 3);

}

void loop() {
    float voltage = pzem.voltage();
    if( !isnan(voltage) ){
        Serial.print("Voltage: "); Serial.print(voltage); Serial.println(" V");
        Firebase.setString(firebaseData, "/output/tegangan", String(voltage) + " V" );
    } else {
        Firebase.setString(firebaseData, "/output/tegangan", "0 V" );
        Serial.println("Error reading voltage");
    }

    float current = pzem.current();
    if( !isnan(current) ){
        Serial.print("Current: "); Serial.print(current); Serial.println(" A");
        Firebase.setString(firebaseData, "/output/arus", String(current) + " A");
    } else {
        Firebase.setString(firebaseData, "/output/arus", "0 A");
        Serial.println("Error reading current");
    }

    float power = pzem.power();
    if( !isnan(power) ){
        Serial.print("Power: "); Serial.print(power); Serial.println(" W");
        Firebase.setString(firebaseData, "/output/daya", String(power) + " Watt");
    } else {
        Firebase.setString(firebaseData, "/output/daya", "0 Watt");
        Serial.println("Error reading power");
    }

    Firebase.getString(firebaseData, "/output/saklar", relayStat);
    Serial.println(relayStat);
    if (relayStat == "ON") {
      Serial.print("\n Relay ON");
      digitalWrite(relay, LOW);  
   }
   else{
      Serial.print("\n Relay OFF");
      digitalWrite(relay, HIGH);  
   }
    Serial.println();
    delay(200);
}
