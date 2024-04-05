#include <dht.h>
dht DHT;

#define DHT11_PIN 6

// Variable para almacenar el último valor de la temperatura del aire
int ultimo_valor_temperatura_aire = 0;

// Variable para almacenar el tiempo de la última lectura del sensor
unsigned long ultimo_tiempo_lectura = 0;

// Intervalo de tiempo entre lecturas del sensor (en milisegundos)
const unsigned long intervalo_lectura = 5000;  // Lee el sensor cada 5 segundos

void setup() {
  Serial.begin(9600);
}

void loop() {
  // Obtener el tiempo actual
  unsigned long tiempo_actual = millis();

  // Si ha pasado suficiente tiempo desde la última lectura del sensor
  if (tiempo_actual - ultimo_tiempo_lectura >= intervalo_lectura) {
    // Leer la temperatura del aire del sensor DHT11
    DHT.read11(DHT11_PIN);

    // Obtener el valor actual de la temperatura del aire
    float temperatura_aire_actual = DHT.temperature;

    // Convertir la temperatura a un número entero
    int temperatura_entera = int(temperatura_aire_actual);

    // Calcular la diferencia entre el valor actual y el último valor
    float diferencia = abs(temperatura_aire_actual - ultimo_valor_temperatura_aire);

    // Si la diferencia es mayor o igual a 1 grado, mostrar la temperatura
    if (diferencia >= 1) {
      // Actualizar el último valor de la temperatura del aire
      ultimo_valor_temperatura_aire = temperatura_entera;

      // Mostrar la temperatura del aire en el puerto serie
      Serial.print("Temperatura del aire: ");
      Serial.print(temperatura_entera); // Mostrar la temperatura como un número entero
      Serial.println(" C");
    }

    // Actualizar el tiempo de la última lectura del sensor
    ultimo_tiempo_lectura = tiempo_actual;
  }
}
