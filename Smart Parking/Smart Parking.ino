#include <Servo.h>

#define IR1 2   // IR Sensor 1
#define IR2 3   // IR Sensor 2
#define IR3 4   // IR Sensor 3

#define LED1 5  // LED for Slot 1
#define LED2 6  // LED for Slot 2
#define LED3 7  // LED for Slot 3

#define TRIG 8  // Ultrasonic Trigger
#define ECHO 9  // Ultrasonic Echo
#define SERVO 11   // Servo motor for gate

Servo gate;

void setup() {
    pinMode(IR1, INPUT);
    pinMode(IR2, INPUT);
    pinMode(IR3, INPUT);
    
    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);
    pinMode(LED3, OUTPUT);
    
    pinMode(TRIG, OUTPUT);
    pinMode(ECHO, INPUT);
    
    gate.attach(SERVO);
    gate.write(0); // Gate initially closed

    Serial.begin(9600);
}

bool isParkingFull() {
    return digitalRead(IR1) == LOW && digitalRead(IR2) == LOW && digitalRead(IR3) == LOW;
}

int getDistance() {
    digitalWrite(TRIG, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG, LOW);
    return pulseIn(ECHO, HIGH) * 0.034 / 2; // Convert time to distance in cm
}

void loop() {
    bool slot1 = digitalRead(IR1);
    bool slot2 = digitalRead(IR2);
    bool slot3 = digitalRead(IR3);

    Serial.print("Slot1: "); Serial.print(slot1);
    Serial.print(" Slot2: "); Serial.print(slot2);
    Serial.print(" Slot3: "); Serial.println(slot3);

    // Update LED status based on slot availability
    digitalWrite(LED1, slot1 ? HIGH : LOW);
    digitalWrite(LED2, slot2 ? HIGH : LOW);
    digitalWrite(LED3, slot3 ? HIGH : LOW);

    int distance = getDistance();
    Serial.print("Vehicle Distance: "); Serial.println(distance);

    if (distance < 15 && !isParkingFull()) { // Vehicle detected & parking available
        Serial.println("Vehicle detected. Opening gate...");
        gate.write(90); // Open gate
        delay(3000); // Keep gate open for 3 seconds
        gate.write(0); // Close gate
        Serial.println("Gate closed.");
    }

    delay(500);
}
