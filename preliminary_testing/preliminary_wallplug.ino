void setup() {
  Serial.begin(9600);
  pinMode(5, OUTPUT);
}

void loop() {
  digitalWrite(5,HIGH);
  Serial.println("Wall Plug On");
  delay(5000);
  digitalWrite(5,LOW);
  Serial.println("Wall Plug Off");
  delay(5000);
}
