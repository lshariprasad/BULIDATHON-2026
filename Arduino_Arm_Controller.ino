#include <Servo.h>

// =====================================
// SERVO OBJECTS
// =====================================

Servo baseServo;
Servo shoulderServo;
Servo elbowServo;
Servo gripperServo;

// =====================================
// PIN CONFIGURATION
// Change to your actual wiring
// =====================================

#define BASE_SERVO_PIN      3
#define SHOULDER_SERVO_PIN  5
#define ELBOW_SERVO_PIN     6
#define GRIPPER_SERVO_PIN   9

// =====================================
// CURRENT ANGLES
// =====================================

int baseAngle = 90;
int shoulderAngle = 90;
int elbowAngle = 90;
int gripperAngle = 90;

// =====================================
// ARM FUNCTIONS
// =====================================

void writeArm() {

  baseServo.write(baseAngle);
  shoulderServo.write(shoulderAngle);
  elbowServo.write(elbowAngle);
  gripperServo.write(gripperAngle);
}

void openGripper() {

  gripperAngle = 30;
  gripperServo.write(gripperAngle);

  Serial.println("GRIP_OPEN");
}

void closeGripper() {

  gripperAngle = 100;
  gripperServo.write(gripperAngle);

  Serial.println("GRIP_CLOSE");
}

void homePosition() {

  baseAngle = 90;
  shoulderAngle = 90;
  elbowAngle = 90;
  gripperAngle = 30;

  writeArm();

  Serial.println("ARM_HOME");
}

// =====================================
// SETUP
// =====================================

void setup() {

  Serial.begin(9600);

  baseServo.attach(BASE_SERVO_PIN);
  shoulderServo.attach(SHOULDER_SERVO_PIN);
  elbowServo.attach(ELBOW_SERVO_PIN);
  gripperServo.attach(GRIPPER_SERVO_PIN);

  homePosition();

  Serial.println("ARM_READY");
}

// =====================================
// SERIAL COMMAND PROCESSOR
// =====================================

void processCommand(String command) {

  command.trim();

  if (command == "ARM_HOME") {

    homePosition();
  }

  else if (command == "GRIP_OPEN") {

    openGripper();
  }

  else if (command == "GRIP_CLOSE") {

    closeGripper();
  }

  else if (command.startsWith("BASE:")) {

    baseAngle = constrain(
      command.substring(5).toInt(),
      0,
      180
    );

    baseServo.write(baseAngle);

    Serial.print("BASE=");
    Serial.println(baseAngle);
  }

  else if (command.startsWith("SHOULDER:")) {

    shoulderAngle = constrain(
      command.substring(9).toInt(),
      0,
      180
    );

    shoulderServo.write(shoulderAngle);

    Serial.print("SHOULDER=");
    Serial.println(shoulderAngle);
  }

  else if (command.startsWith("ELBOW:")) {

    elbowAngle = constrain(
      command.substring(6).toInt(),
      0,
      180
    );

    elbowServo.write(elbowAngle);

    Serial.print("ELBOW=");
    Serial.println(elbowAngle);
  }

  else if (command.startsWith("GRIPPER:")) {

    gripperAngle = constrain(
      command.substring(8).toInt(),
      0,
      180
    );

    gripperServo.write(gripperAngle);

    Serial.print("GRIPPER=");
    Serial.println(gripperAngle);
  }
}

// =====================================
// LOOP
// =====================================

void loop() {

  if (Serial.available()) {

    String command = Serial.readStringUntil('\n');

    processCommand(command);
  }
}
