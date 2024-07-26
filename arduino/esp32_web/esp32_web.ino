#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer_Generic.h>

// Replace with your network credentials
const char* ssid = "🚀 NASA 2GHZ 🚀";
const char* password = "sRTR7s97E5d2";

// Set GPIO pin for the relay
const int relayPin = 5;

// Create an instance of the web server on port 80
WebServer server(80);
WebSocketsServer webSocket(81);

// Track the relay state
bool relayState = false;
void setupPins(){
  pinMode(5, INPUT_PULLUP); //The pins are set INPUT by default
}

void setup() {
  Serial.begin(115200);
  setupPins();

  // Initialize the relay pin as an output
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW); // Turn off the relay initially

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi..");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("Connected!");

  // Print the ESP32 IP address
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Define the web server routes
  server.on("/", handleRoot);
  server.on("/toggle", handleToggle);

  // Start the web server
  server.begin();

  // Initialize WebSocket server
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  Serial.println("HTTP server started");
}

void loop() {
  // Handle client requests
  server.handleClient();
  webSocket.loop();

  int e2 = digitalRead(5);
  if(e2 > 0 ){
 webSocket.sendTXT(1,">0");
}else{
  webSocket.sendTXT(2,"<0");
}
}

void handleToggle() {
  relayState = !relayState;
  digitalWrite(relayPin, relayState ? HIGH : LOW);
  server.send(200, "text/plain", relayState ? "ON" : "OFF");
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length) {
  switch(type) {
    case WStype_TEXT:
      if (strcmp((char*)payload, "toggle") == 0) {
        relayState = !relayState;
        String message = relayState ? "ON" : "OFF";
        webSocket.sendTXT(num, message);
      }
      break;
  }
}

void handleRoot() {
  String html = "<!DOCTYPE html><html lang=\"en\">";
html += "<head>";
html += "<link rel=\"stylesheet\" href=\"https://www.chess.com/chesscom-artifacts/packages/@chesscom/chessboard-layout/1.8.0/style.client.css\" />";
html += "<link rel=\"stylesheet\" href=\"https://www.chess.com/chesscom-artifacts/packages/@chesscom/play-computer-client/2.5.0/play-computer-vendors.client.css\" />";
html += "<link rel=\"stylesheet\" href=\"https://raw.githack.com/nasascience/chess_esp32/main/css/pieces.css\">";
html += "<style>";
html += "#board-play-computer, .fade-in-overlay {";
html += "background-image: url(https://images.chesscomfiles.com/chess-themes/boards/icy_sea/200.png);";
html += "}";
html += "</style>";
html += "</head>";
html += "<body>";
html += "<div id=\"board-layout-chessboard\" class=\"board-layout-chessboard board-board\">";
html += "<wc-chess-board class=\"board\" id=\"board-play-computer\"></wc-chess-board>";
html += "</div>";
html += "<script src=\"https://raw.githack.com/nasascience/chess_esp32/main/js/chess_esp32.js\"></script>";
html += "</body></html>";


  server.send(200, "text/html", html);
}

