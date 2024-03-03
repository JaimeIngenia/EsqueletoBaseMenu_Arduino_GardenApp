//ENCODER
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <LiquidMenu.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);
#define outputA 6
#define outputB 7
#define sw 4
int aState;
int aLastState;  
bool editingMode = false;
int variableMostrarPantalla = 0;
//estados
int salida=0;
int estado=1;
int estadoAnterior=1;
enum estados {
  EST_ENCENDIDO_EDITAR,
  EST_LOGICA_EDITAR,
  EST_APAGADO_EDITAR,
};
int estado_actual=EST_ENCENDIDO_EDITAR;
int estado_anterior=EST_APAGADO_EDITAR;


void setup() {
  Serial.begin(9600);
  
  pinMode(sw,INPUT_PULLUP);

}

void loop() {
  Serial.println("Bienvenido");	
  estado = digitalRead(sw);
  Serial.println(estado);
  Serial.println("-------------------------------------------- ");
  arrancar();
}

void arrancar()
{

  if (estado_actual == EST_ENCENDIDO_EDITAR)
  {
      Serial.println("Estado: ");
      Serial.println(estado);
      Serial.println("Salida: ");
      Serial.println(salida);
      Serial.println("-------------------------------------------- ");
    estado = digitalRead(sw);
      Serial.println("Estado: ");
      Serial.println(estado);
    if((estado == LOW)&&(estadoAnterior == HIGH))
    {
      salida = 1;
      delay (20);            
      estado_actual = EST_LOGICA_EDITAR; 
      Serial.println("Salida: ");
      Serial.println(salida);  
      delay(5000);
    }
    estadoAnterior = estado;
  }  
}
