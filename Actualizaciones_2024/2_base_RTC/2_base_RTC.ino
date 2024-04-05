
#include <RTClib.h>

  

RTC_DS3231 modulo_rtc;


char DiasDeLaSemana[7][12] = {"Domingo", "Lunes", "Martes", "Miercoles", "Jueves", "Viernes", "Sabado"};
  

void setup () {
  
  Serial.begin(9600);

  if( !modulo_rtc.begin() )
  {
    Serial.println("No se encontró ningun RTC");
    Serial.flush();
    abort();
  }

   modulo_rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

   modulo_rtc.adjust(DateTime(2024, 4, 4, 18, 22, 0));


}

  

void loop () {



DateTime ahora = modulo_rtc.now();


  



       Serial.print(ahora.year());

       Serial.print('/');

       Serial.print(ahora.month());

       Serial.print('/');

       Serial.print(ahora.day());

       Serial.print(" (");

       Serial.print(DiasDeLaSemana[ahora.dayOfTheWeek()]);

       Serial.print(") ");

       Serial.print(ahora.hour());

       Serial.print(':');

       Serial.print(ahora.minute());

       Serial.print(':');

       Serial.print(ahora.second());

       Serial.println();



       Serial.print("Temperatura: ");

       Serial.print(modulo_rtc.getTemperature());

       Serial.println(" *C");

        

       Serial.println();

       delay(1000);

}