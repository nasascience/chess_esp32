#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer_Generic.h>

// Replace with your network credentials
const char* ssid = "🚀 NASA 2GHZ 🚀";
const char* password = "sRTR7s97E5d2";

// Set GPIO pin for the Hall Sensor
const int ledPin = 4;
const int sensorPin = 5;

// Create an instance of the web server on port 80
WebServer server(80);
WebSocketsServer webSocket(81);

// Track the relay state
bool relayState = false;
void setupPins(){
  pinMode(ledPin, OUTPUT);
  pinMode(sensorPin, INPUT_PULLUP); //The pins are set INPUT by default
}

void setup() {
  Serial.begin(115200);
  setupPins();

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
  //server.on("/toggle", handleToggle);

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

  int e2 = digitalRead(sensorPin);
  if(e2 > 0 ){
digitalWrite(ledPin,LOW);
 sendMessageToClients(">0");
}else{
  digitalWrite(ledPin,HIGH);
  sendMessageToClients("<0");
}
}

// void handleToggle() {
//   relayState = !relayState;
//   digitalWrite(relayPin, relayState ? HIGH : LOW);
//   server.send(200, "text/plain", relayState ? "ON" : "OFF");
// }

void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length) {
  switch(type) {
    case WStype_DISCONNECTED:
      Serial.printf("Client %u disconnected!\n", num);
      break;
    case WStype_CONNECTED:
      {
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("Client %u connected from %s\n", num, ip.toString().c_str());
      }
      break;
    case WStype_TEXT:
      Serial.printf("Client %u sent text: %s\n", num, payload);
      // Example: Toggle relay state based on received message
      if (strcmp((char *)payload, "toggle") == 0) {
        // Send a message to all clients
        webSocket.sendTXT(num, "ON");
      }
      break;
    case WStype_ERROR:
      Serial.printf("Error on WebSocket connection %u\n", num);
      break;
    default:
      break;
  }
  
}

// Function to send a message to all connected clients
void sendMessageToClients(const String &message) {
  webSocket.broadcastTXT(message);
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

