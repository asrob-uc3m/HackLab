#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

const char* ssid = "ASROB";
const char* password = "asrobchallenge";

ESP8266WebServer server(80);

const int led = 13;
const int LED_PIN_RED = D0;
const int LED_PIN_BLUE = D2;
const int LED_PIN_GREEN = D1;

void handleRoot() {
  digitalWrite(led, 1);
  server.send(200, "text/plain", "IoTeces #2 Ejemplo basico de un servidor web multicolor");
  digitalWrite(led, 0);
}

void handleNotFound(){
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}
void handleRed() {
  digitalWrite(LED_PIN_RED, LOW);
  digitalWrite(LED_PIN_BLUE, HIGH);
  digitalWrite(LED_PIN_GREEN, HIGH);
  server.send(200, "text/plain", "Soy rojo");
}
void handleBlue() {
  digitalWrite(LED_PIN_RED, HIGH);
  digitalWrite(LED_PIN_BLUE, LOW);
  digitalWrite(LED_PIN_GREEN, HIGH);
  server.send(200, "text/plain", "Soy azul");
}
void handleGreen() {
  digitalWrite(LED_PIN_RED, HIGH);
  digitalWrite(LED_PIN_BLUE, HIGH);
  digitalWrite(LED_PIN_GREEN, LOW);
  server.send(200, "text/plain", "Soy verde");
}
void handleOff() {
  digitalWrite(LED_PIN_RED, HIGH);
  digitalWrite(LED_PIN_BLUE, HIGH);
  digitalWrite(LED_PIN_GREEN, HIGH);
  server.send(200, "text/plain", "Me apago");
}
void handleColor() {
  if(server.args() == 3){
    digitalWrite(LED_PIN_RED, 1-server.arg(0).toInt());
    digitalWrite(LED_PIN_BLUE, 1-server.arg(2).toInt());
    digitalWrite(LED_PIN_GREEN, 1-server.arg(1).toInt());

  }
  String message = "Your COLOR is (digitalOutput):\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(200, "text/plain", message);
}
void setup(void){
  pinMode(led, OUTPUT);
  pinMode(LED_PIN_RED, OUTPUT);
  pinMode(LED_PIN_BLUE, OUTPUT);
  pinMode(LED_PIN_GREEN, OUTPUT);
  digitalWrite(led, 0);
  digitalWrite(LED_PIN_RED, HIGH);
  digitalWrite(LED_PIN_BLUE, HIGH);
  digitalWrite(LED_PIN_GREEN, HIGH);

  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/red", handleRed);
  server.on("/blue", handleBlue);
  server.on("/green", handleGreen);
  server.on("/off", handleOff);
  server.on("/color", handleColor);

  server.on("/inline", [](){
    server.send(200, "text/plain", "this works as well");
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void){
  server.handleClient();
}
