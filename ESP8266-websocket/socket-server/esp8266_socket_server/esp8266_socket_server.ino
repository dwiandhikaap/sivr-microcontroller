#include <ArduinoWebsockets.h>
#include <ESP8266WiFi.h>

const char* ssid = "Aperture Science II";
const char* password = "anjaymabar";

using namespace websockets;
WebsocketsServer server;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  
  Serial.begin(115200);
  Serial.println();

  WiFi.begin(ssid, password);

  Serial.print("Connecting");
  while(WiFi.status() != WL_CONNECTED){
    delay(1000);
    Serial.print(".");
  }

  Serial.println();

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());

  server.listen(4000);

  Serial.print("Server Status: ");
  Serial.println(server.available());
}

void loop() {
  auto client = server.accept();

  while(client.available()){
    auto message = client.readBlocking();
    if(message.isText()){
        String msg = message.data();
        client.send("LOG :" + msg);
        Serial.println(msg.toInt());
        analogWrite(LED_BUILTIN, msg.toInt());
      }
    delay(100);
  }
}
