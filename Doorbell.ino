#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>

// --- Config -- //

const char* ssid     = "<Wifi SSID>";
const char* password = "<Wifi password>";

const String TelegramAPI = "<Telegram API token>";
const String TelegramChatID = "Telegram Chat ID";

// --- End Config -- //

const String host = "https://api.telegram.org/bot" + TelegramAPI + "/sendMessage?chat_id=" + TelegramChatID + "&text=Deurbel";
const char* streamId   = "....................";
const char* privateKey = "....................";
const int BUTTON=2;
int BUTTONState=0;

void startwifi() {

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
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

}

void setup() {
  Serial.begin(115200);
  delay(10);
  startwifi();
  ringbell();
  ESP.deepSleep(0);
}

int value = 0;

void ringbell() {
  std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
  client->setInsecure();
  HTTPClient https;

  if (https.begin(*client, host)) {  // HTTPS
    Serial.println("[HTTPS] GET...");
    int httpCode = https.GET();

    // httpCode will be negative on error
    if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTPS] GET... code: %d\n", httpCode);
      // file found at server?
      if (httpCode == HTTP_CODE_OK) {
        String payload = https.getString();
        Serial.println(String("[HTTPS] Received payload: ") + payload);
        Serial.println(String("1BTC = ") + payload + "USD");
      }
    } else {
      Serial.printf("[HTTPS] GET... failed, error: %s\n\r", https.errorToString(httpCode).c_str());
    }

    https.end();
  } else {
    Serial.printf("[HTTPS] Unable to connect\n\r");
  }
}

void loop() {
  //DeepSleep 
}
