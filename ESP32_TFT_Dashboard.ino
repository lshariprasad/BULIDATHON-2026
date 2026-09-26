#include <WiFi.h>
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();

// =====================================
// WIFI
// =====================================

const char* WIFI_SSID = "YOUR_WIFI_NAME";
const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";

// =====================================
// UART
// =====================================

#define RXD2 16
#define TXD2 17

HardwareSerial RobotSerial(2);

// =====================================
// ROBOT DATA
// =====================================

String carStatus = "WAITING";
String armStatus = "READY";
String gripStatus = "UNKNOWN";

float distanceCm = 0.0;
float batteryVoltage = 0.0;

bool wifiConnected = false;

// =====================================
// DISPLAY
// =====================================

void drawHeader() {

  tft.fillScreen(TFT_BLACK);

  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.setTextSize(3);

  tft.setCursor(55, 15);
  tft.println("RAVEN");

  tft.drawLine(
    10, 55,
    310, 55,
    TFT_BLUE
  );
}

void drawDashboard() {

  tft.fillRect(
    0, 65,
    320,
    175,
    TFT_BLACK
  );

  tft.setTextSize(2);

  tft.setTextColor(TFT_WHITE);

  tft.setCursor(15, 75);
  tft.print("CAR : ");
  tft.setTextColor(TFT_GREEN);
  tft.println(carStatus);

  tft.setTextColor(TFT_WHITE);

  tft.setCursor(15, 105);
  tft.print("ARM : ");
  tft.println(armStatus);

  tft.setCursor(15, 135);
  tft.print("GRIP: ");
  tft.println(gripStatus);

  tft.setCursor(15, 165);
  tft.print("DIST: ");
  tft.print(distanceCm, 1);
  tft.println(" cm");

  tft.setCursor(15, 195);
  tft.print("BATT: ");
  tft.print(batteryVoltage, 2);
  tft.println(" V");

  tft.setCursor(15, 225);
  tft.print("WiFi: ");

  if (wifiConnected) {

    tft.setTextColor(TFT_GREEN);
    tft.println("ONLINE");

  } else {

    tft.setTextColor(TFT_RED);
    tft.println("OFFLINE");
  }
}

// =====================================
// SERIAL DATA PARSER
// =====================================

void processTelemetry(String data) {

  data.trim();

  if (data == "CAR_FORWARD") {

    carStatus = "FORWARD";
  }

  else if (data == "CAR_BACKWARD") {

    carStatus = "BACKWARD";
  }

  else if (data == "CAR_LEFT") {

    carStatus = "LEFT";
  }

  else if (data == "CAR_RIGHT") {

    carStatus = "RIGHT";
  }

  else if (data == "CAR_STOP") {

    carStatus = "STOP";
  }

  else if (data == "ARM_READY") {

    armStatus = "READY";
  }

  else if (data == "GRIP_OPEN") {

    gripStatus = "OPEN";
  }

  else if (data == "GRIP_CLOSE") {

    gripStatus = "CLOSED";
  }

  else if (data.startsWith("DISTANCE:")) {

    distanceCm =
      data.substring(9).toFloat();
  }

  else if (data.startsWith("BATTERY:")) {

    batteryVoltage =
      data.substring(8).toFloat();
  }

  drawDashboard();
}

// =====================================
// SETUP
// =====================================

void setup() {

  Serial.begin(115200);

  RobotSerial.begin(
    9600,
    SERIAL_8N1,
    RXD2,
    TXD2
  );

  tft.init();

  tft.setRotation(1);

  drawHeader();

  // Wi-Fi
  WiFi.begin(
    WIFI_SSID,
    WIFI_PASSWORD
  );

  unsigned long startTime = millis();

  while (
    WiFi.status() != WL_CONNECTED &&
    millis() - startTime < 8000
  ) {

    delay(500);
  }

  wifiConnected =
    WiFi.status() == WL_CONNECTED;

  drawDashboard();
}

// =====================================
// LOOP
// =====================================

void loop() {

  while (RobotSerial.available()) {

    String data =
      RobotSerial.readStringUntil('\n');

    processTelemetry(data);
  }

  if (WiFi.status() == WL_CONNECTED) {

    wifiConnected = true;

  } else {

    wifiConnected = false;
  }

  delay(50);
}
