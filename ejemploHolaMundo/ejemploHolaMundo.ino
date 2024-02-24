#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Dirección I2C de la pantalla LCD (ajusta según la configuración de tu módulo)
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  // Inicializar la pantalla LCD
  lcd.begin(16, 2);
  
  // Imprimir "Hola Mundo" en la primera línea de la pantalla
  lcd.setCursor(0, 0);
  lcd.print("Hola Mundo");
}

void loop() {
  // El loop está vacío en este ejemplo, ya que solo queremos imprimir una vez en la inicialización.
}
