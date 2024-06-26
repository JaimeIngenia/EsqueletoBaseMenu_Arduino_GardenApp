/*
	Capitulo 38 de Arduino desde cero en Español.
	Programa que activa modulo de rele a las 14:30 hs y desactiva una hora mas tarde
	a las 15:30 hs todos los dias.

	Autor: bitwiseAr  

*/

#include <Wire.h>		// incluye libreria para interfaz I2C
#include <RTClib.h>		// incluye libreria para el manejo del modulo RTC

RTC_DS3231 rtc;			// crea objeto del tipo RTC_DS3231

bool evento_inicio = true;	// variable de control para inicio de evento con valor true
bool evento_fin = true;		// variable de control para finalizacion de evento con valor true

# define RELE 4			// constante RELE con valor 3 que corresponde a pin digital 3
				// donde se encuentra conectado el modulo de rele
void setup () {
 Serial.begin(9600);		// inicializa comunicacion serie a 9600 bps
 pinMode(RELE, OUTPUT);		// pin 3 como salida

 if (! rtc.begin()) {				// si falla la inicializacion del modulo
 Serial.println("Modulo RTC no encontrado !");	// muestra mensaje de error
 while (1);					// bucle infinito que detiene ejecucion del programa
 }
}

void loop () {
 DateTime fecha = rtc.now();				// funcion que devuelve fecha y horario en formato
							// DateTime y asigna a variable fecha
 if ( fecha.hour() == 17 && fecha.minute() == 22  && fecha.second() == 10){	// si hora = 14 y minutos = 30
    if ( evento_inicio == true ){			// si evento_inicio es verdadero
      digitalWrite(RELE, HIGH);				// activa modulo de rele con nivel alto
      Serial.println( "Rele encendido" );		// muestra texto en monitor serie
      evento_inicio = false;				// carga valor falso en variable de control
    }							// para evitar ingresar mas de una vez
  }

 if ( fecha.hour() == 17 && fecha.minute() == 22  && fecha.second() == 20 ){	// si hora = 15 y minutos = 30
    if ( evento_fin == true ){				// si evento_fin es verdadero
      digitalWrite(RELE, LOW);				// desactiva modulo de rele con nivel bajo
      Serial.println( "Rele apagado" );			// muestra texto en monitor serie
      evento_fin = false;				// carga valor falso en variable de control
    }							// para evitar ingresar mas de una vez
  }

 Serial.print(fecha.day());				// funcion que obtiene el dia de la fecha completa
 Serial.print("/");					// caracter barra como separador
 Serial.print(fecha.month());				// funcion que obtiene el mes de la fecha completa
 Serial.print("/");					// caracter barra como separador
 Serial.print(fecha.year());				// funcion que obtiene el año de la fecha completa
 Serial.print(" ");					// caracter espacio en blanco como separador
 Serial.print(fecha.hour());				// funcion que obtiene la hora de la fecha completa
 Serial.print(":");					// caracter dos puntos como separador
 Serial.print(fecha.minute());				// funcion que obtiene los minutos de la fecha completa
 Serial.print(":");					// caracter dos puntos como separador
 Serial.println(fecha.second());			// funcion que obtiene los segundos de la fecha completa
 
 delay(1000);						// demora de 1 segundo

  if ( fecha.hour() == 2 && fecha.minute() == 0 ){ 	// si hora = 2 y minutos = 0 restablece valores de
    evento_inicio = true;				// variables de control en verdadero
    evento_fin = true;
  }
}