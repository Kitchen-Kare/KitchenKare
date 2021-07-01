void setup() {
  //Initialize
  Serial.begin(9600);
  pinMode(5, OUTPUT);
}

void loop() {
  //Turn On
  digitalWrite(5,HIGH);
  Serial.println("Wall Plug On");

  //Wait 5 seconds
  delay(5000);

  //Turn Off
  digitalWrite(5,LOW);
  Serial.println("Wall Plug Off");
  
  //Wait 5 seconds
  delay(5000);
}
