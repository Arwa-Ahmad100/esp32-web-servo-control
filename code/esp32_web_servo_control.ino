#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>

Servo gateServo;
WebServer server(80);

// Pin configuration
const int SERVO_PIN = 18;
const int GREEN_LED = 26;
const int RED_LED = 27;

// Wi-Fi Access Point settings
const char* ssid = "ESP32_Access_Point";
const char* password = "12345678";

// Servo positions
const int OPEN_ANGLE = 90;
const int CLOSE_ANGLE = 0;


// ==============================
// Web Page
// ==============================

String webPage() {

  String page = R"rawliteral(
<!DOCTYPE html>
<html>
<head>

  <meta name="viewport" content="width=device-width, initial-scale=1">

  <title>ESP32 Servo Control</title>

  <style>

    body {
      font-family: Arial, sans-serif;
      text-align: center;
      background-color: #f4f4f4;
      margin-top: 60px;
    }

    h1 {
      font-size: 32px;
    }

    p {
      font-size: 18px;
    }

    button {
      width: 180px;
      padding: 18px;
      margin: 12px;
      font-size: 20px;
      border: none;
      border-radius: 10px;
      cursor: pointer;
      color: white;
    }

    .open {
      background-color: #4CAF50;
    }

    .close {
      background-color: #f44336;
    }

  </style>

</head>

<body>

  <h1>ESP32 Servo Control</h1>

  <p>Web-Based OPEN / CLOSE Control</p>

  <a href="/open">
    <button class="open">OPEN</button>
  </a>

  <br>

  <a href="/close">
    <button class="close">CLOSE</button>
  </a>

</body>
</html>
)rawliteral";

  return page;
}


// ==============================
// Open Gate
// ==============================

void openGate() {

  gateServo.write(OPEN_ANGLE);

  digitalWrite(GREEN_LED, HIGH);
  digitalWrite(RED_LED, LOW);

  server.send(200, "text/html", webPage());
}


// ==============================
// Close Gate
// ==============================

void closeGate() {

  gateServo.write(CLOSE_ANGLE);

  digitalWrite(GREEN_LED, LOW);
  digitalWrite(RED_LED, HIGH);

  server.send(200, "text/html", webPage());
}


// ==============================
// Setup
// ==============================

void setup() {

  Serial.begin(115200);

  // Configure LEDs
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);

  // Configure servo
  gateServo.attach(SERVO_PIN);

  // Initial state: CLOSED
  gateServo.write(CLOSE_ANGLE);

  digitalWrite(GREEN_LED, LOW);
  digitalWrite(RED_LED, HIGH);


  // Create Wi-Fi Access Point
  WiFi.softAP(ssid, password);

  Serial.println();
  Serial.println("ESP32 Access Point Started");

  Serial.print("Wi-Fi Network: ");
  Serial.println(ssid);

  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());


  // Main web page
  server.on("/", []() {

    server.send(
      200,
      "text/html",
      webPage()
    );

  });


  // OPEN command
  server.on("/open", openGate);


  // CLOSE command
  server.on("/close", closeGate);


  // Start web server
  server.begin();

  Serial.println("Web Server Started");
}


// ==============================
// Main Loop
// ==============================

void loop() {

  server.handleClient();

}
