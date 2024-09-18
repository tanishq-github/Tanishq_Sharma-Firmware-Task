void setup() {
  // put your setup code here, to run once:
   // Initialize serial communication at 9600 baud rate
  Serial.begin(9600);
  Serial.println("Waiting for a single byte of data...");
}

void loop() {
  // put your main code here, to run repeatedly:
     // Check if data is available in the serial buffer
  if (Serial.available() > 0) {
    // Read the incoming single byte
    char receivedByte = Serial.read();  // Read a single byte (char)

    // Print the received byte to the Serial Monitor
    Serial.print("Received byte: ");
    Serial.println(receivedByte);

  }
}