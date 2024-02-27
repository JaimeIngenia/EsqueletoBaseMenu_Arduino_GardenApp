# define RELE 24		


void setup() {

    Serial.begin(9600);		// inicializa comunicacion serie a 9600 bps
    pinMode(RELE, OUTPUT);		// pin 3 como salida

 }



void loop() {
  
  Serial.println("Hola Dilbani que estudia landas");	
  Serial.println("Rele: ENCENDIDO");
  digitalWrite(RELE, HIGH);	
  delay(5000);
  Serial.println("Rele: APAGADO");
  digitalWrite(RELE, LOW);	
  delay(5000);
}
