const int NUM_MACHINES = 4;

// Sensor input pins (Analog)
const int tempSensorPins[NUM_MACHINES] = {A0, A1, A2, A3};

// Solenoid valve control pins (Digital)
const int valvePins[NUM_MACHINES] = {4, 5, 6, 7};

// Settings
const float TEMP_THRESHOLD = 45.0;                 // Activation temp in °C
const unsigned long LUBRICATION_DURATION = 5000;   // Duration in ms (5 sec)

// State tracking
bool valveActive[NUM_MACHINES];
unsigned long lastActivationTime[NUM_MACHINES];

void setup() {
  Serial.begin(9600);

  for (int i = 0; i < NUM_MACHINES; i++) {
    pinMode(valvePins[i], OUTPUT);
    digitalWrite(valvePins[i], LOW);
    valveActive[i] = false;
    lastActivationTime[i] = 0;
  }

  Serial.println("Lubrication system initialized...");
}

void loop() {
  unsigned long currentTime = millis();

  for (int i = 0; i < NUM_MACHINES; i++) {
    float temperature = readTemperature(tempSensorPins[i]);

    Serial.print("Machine ");
    Serial.print(i + 1);
    Serial.print(" Temperature: ");
    Serial.print(temperature);
    Serial.println(" °C");

    if (temperature >= TEMP_THRESHOLD && !valveActive[i]) {
      digitalWrite(valvePins[i], HIGH);
      valveActive[i] = true;
      lastActivationTime[i] = currentTime;
      Serial.print("→ Lubrication started for Machine ");
      Serial.println(i + 1);
    }

    if (valveActive[i] && (currentTime - lastActivationTime[i] >= LUBRICATION_DURATION)) {
      digitalWrite(valvePins[i], LOW);
      valveActive[i] = false;
      Serial.print("✓ Lubrication stopped for Machine ");
      Serial.println(i + 1);
    }
  }

  delay(500);  // Short delay for stability
}

float readTemperature(int pin) {
  int raw = analogRead(pin);
  float voltage = raw * (5.0 / 1023.0);
  float tempC = (voltage - 0.5) * 100.0;  // LM35 formula
  return tempC;
}
