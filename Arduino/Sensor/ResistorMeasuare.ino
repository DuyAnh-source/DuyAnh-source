// I used a 1000ohm resistor in this diagram
// 5V - R - A0 - 1kohmR - GND
// In case that there is no tool for measurement

void setup() {
pinMode(A0, INPUT);
Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int t = analogRead(A0);
  if(t == 0) t = 1;
  float R = 1000*((1023-(float)(t))/((float)(t)));
  Serial.println(R);
  delay(500);
}
