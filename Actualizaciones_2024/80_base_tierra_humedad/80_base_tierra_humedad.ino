int valores = 0;

void setup() {

    Serial.begin(9600);

}

void loop() {
    valores = analogRead(A0);
    Serial.println(valores);
    delay(10);
}
