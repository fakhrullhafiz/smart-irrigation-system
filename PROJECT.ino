//PRACTICE MAKING NEW COMMIT AND PUSH
const int relayPin = 5;          // Output pin for the relay
const int waterSensorPin = 6;    // Input pin for the soil moisture sensor
const int moistureAnalogPin = A0; // Analog input pin for the soil moisture sensor

int value;                        // Variable to store the ADC value
float voltage;                    // Variable to store the calculated voltage

void setup() {
  pinMode(relayPin, OUTPUT);      // Set relay pin as output
  pinMode(waterSensorPin, INPUT); // Set sensor pin as input
  pinMode(moistureAnalogPin, INPUT); // Set analog pin as input
  Serial.begin(9600);             // Begin serial communication
}

void loop() {
  int sensorValue = digitalRead(waterSensorPin); // Read the state of the soil moisture sensor

  // Read the analog value from the moisture sensor
  value = analogRead(moistureAnalogPin);         // Read the ADC value from A0
  voltage = map(value, 0, 1023, 0, 3300) / 1000.0; // Map the ADC value to a range of 0-3300 mV and convert to volts

  // Print the analog reading and calculated voltage to the serial monitor
  Serial.print("Minimum Value:");
  Serial.print(0);
  Serial.print(",");

  Serial.print("Read Voltage:");
  Serial.print(voltage);
  Serial.print(",");

  Serial.print("Maximum Value:");
  Serial.println(3.3);

  // Control the relay based on the sensor state
  if (sensorValue == LOW) { 
    // LED SIGNAL ON
    digitalWrite(relayPin, LOW);    // Turn off the pump if the soil is wet
  } else { 
    // LED SIGNAL OFF
    digitalWrite(relayPin, HIGH);   // Turn on the pump if the soil is dry
  }
}
