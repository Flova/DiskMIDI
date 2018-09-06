int counter = 0;

void setup() {
   Serial.begin(9600);
}

void loop() {
  sendInt(counter);
  delay(1);
  counter++;

}

void sendInt(int value) {
  Serial.write(lowByte(value));
  Serial.write(highByte(value));
  Serial.println("");
}

