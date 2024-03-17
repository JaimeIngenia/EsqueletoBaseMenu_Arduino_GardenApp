// Definimos el pin donde está conectado el switch
const int switchPin = 8; // Puedes cambiar el número de pin según tu configuración

void setup() {
  // Inicializamos el pin del switch como entrada
  pinMode(switchPin, INPUT_PULLUP); // Activamos la resistencia pull-up interna

  // Iniciamos la comunicación serie
  Serial.begin(9600);
}

void loop() {
  // Leemos el estado del switch
  int switchState = digitalRead(switchPin);

  // Invertimos el estado porque la resistencia pull-up activa el pin en HIGH cuando el switch está en la posición "off"
  switchState = !switchState;

  // Enviamos el estado del switch al monitor en serie
  Serial.println(switchState);

  // Esperamos un breve momento antes de volver a leer el estado del switch
  delay(100);
}
