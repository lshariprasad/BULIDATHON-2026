#include <IRremote.hpp>

// ===============================
// PIN CONFIGURATION
// Change these to match your wiring
// ===============================

#define IR_RECEIVE_PIN 11

// L293D motor pins
#define ENA 5
#define IN1 7
#define IN2 8

#define ENB 6
#define IN3 9
#define IN4 10

// ===============================
// MOTOR FUNCTIONS
// ===============================

void stopRobot() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void forwardRobot(int speedValue = 180) {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, speedValue);
  analogWrite(ENB, speedValue);
}

void backwardRobot(int speedValue = 180) {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  analogWrite(ENA, speedValue);
  analogWrite(ENB, speedValue);
}

void leftRobot(int speedValue = 180) {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, speedValue);
  analogWrite(ENB, speedValue);
}

void rightRobot(int speedValue = 180) {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  analogWrite(ENA, speedValue);
  analogWrite(ENB, speedValue);
}

// ===============================
// TELEMETRY
// ===============================

void sendStatus(const char* state) {
  Serial.println(state);
}

// ===============================
// SETUP
// ===============================

void setup() {

  Serial.begin(9600);

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  stopRobot();

  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);

  sendStatus("CAR_READY");
}

// ===============================
// LOOP
// ===============================

void loop() {

  if (IrReceiver.decode()) {

    unsigned long code = IrReceiver.decodedIRData.command;

    Serial.print("IR_COMMAND:");
    Serial.println(code);

    // IMPORTANT:
    // Replace these command values with the HEX commands
    // from YOUR TV remote.

    switch (code) {

      case 0x18:
        forwardRobot();
        sendStatus("CAR_FORWARD");
        break;

      case 0x52:
        backwardRobot();
        sendStatus("CAR_BACKWARD");
        break;

      case 0x08:
        leftRobot();
        sendStatus("CAR_LEFT");
        break;

      case 0x5A:
        rightRobot();
        sendStatus("CAR_RIGHT");
        break;

      case 0x1C:
        stopRobot();
        sendStatus("CAR_STOP");
        break;

      default:
        stopRobot();
        sendStatus("UNKNOWN_COMMAND");
        break;
    }

    IrReceiver.resume();
  }
}
