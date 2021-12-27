/*****************************************************
 * IEEE Project 2021-2022                            *   
 * Project Name : Room Humidifier                    *
 * Author       : Ömer KAYAPINAR                     * 
 * Rev. Date    : 27.12.2021                         *
 * ***************************************************
 * PURPOSE      : Controlling the ambient            *
 * humidity, temperature and heat index and          *
 * switching the relay according to the situation.   * 
 * ***************************************************
 * SYSTEM INPUT :                                    *
 *      DHT11 --->Temperature and Humidity Sensor    *   
 * ***************************************************
 * SYSTEM OUTPUT:                                    *  
 *      LEDS  --->Three Blue and Three Red LEDS      *
 *      RELAY                                        *  
 * ***************************************************    
 * SYSTEM WORK  :Through the sensor, it calculates   *
 * the humidity and temperature in the environment   *
 * and the heat index, if the values are suitable    *
 * for human health, it lights the blue led, if not, *
 * it lights the red led and switches the relay      *
 * until it brings the humidity to the appropriate   *
 * level.                                            *
 *****************************************************
 */   
// DEFİNİTONS
#include "DHT.h"
#define DHTPIN D4     // what digital pin we're connected to
#define DHTTYPE DHT11
#define relay D1       // what digital pin we're connected to

//Define Output Pins
int b_first_led = D3;    //b is Blue.
int b_second_led = D0;
int b_third_led = D5;
int r_first_led = D6;    //r is Red.
int r_second_led = D7;
int r_third_led = D8;
DHT dht(DHTPIN, DHTTYPE);

// put your setup code here, to run once:
void setup() 
{
  //pinMode Output Pins
  pinMode (b_first_led,OUTPUT);
  pinMode (b_second_led,OUTPUT);
  pinMode (b_third_led,OUTPUT);
  pinMode (r_first_led,OUTPUT);
  pinMode (r_second_led,OUTPUT);
  pinMode (r_third_led,OUTPUT);
  pinMode (relay,OUTPUT);
  
  Serial.begin(9600);
  
  dht.begin();
}

// put your main code here, to run repeatedly:
void loop() 
{
  delay(2000);                          //delay time
  float h = dht.readHumidity();         //read the humidity
  float t = dht.readTemperature();      //read the temprature (for *C)
  float f = dht.readTemperature(true);  //read the temprature (for *F)
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  float hif = dht.computeHeatIndex(f, h);         //heat index calculation (for *C)
  float hic = dht.computeHeatIndex(t, h, false);  //heat index calculation (for *F)

  //Serial Port Prints
  Serial.print("Humidity: ");       
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C\t");
  Serial.print(f);
  Serial.print(" *F\t");
  Serial.print("Heat index: ");
  Serial.print(hic);
  Serial.print(" *C\t");
  Serial.print(hif);
  Serial.println(" *F");

  

  if (61>h && 45<=h)                  //LED for ambient humidity
    digitalWrite(b_first_led,HIGH);   //suitable for human health
  else 
    digitalWrite(b_first_led,LOW);
   
   if (61<h || 45>h)
    digitalWrite(r_first_led,HIGH);   //not suitable for human health   
  else 
    digitalWrite(r_first_led,LOW);

   if (h<45)                          //relay switching for humidifier 
     digitalWrite(relay,LOW);         //relay = 1
   else
     digitalWrite(relay,HIGH);        //relay = 0

   if (17<t && 33>t)                  //LED for ambient temperature
    digitalWrite(b_second_led,HIGH);  //suitable for human health
  else 
    digitalWrite(b_second_led,LOW);
    
   if (33<t || t<17) 
    digitalWrite(r_second_led,HIGH);  //not suitable for human health 
  else 
    digitalWrite(r_second_led,LOW);

   //if (t<17)
    //digitalWrite(relay,LOW);        //if a heater is connected instead of a humidifier
   //else
    //digitalWrite(relay,HIGH);        //if a heater is connected instead of a humidifier
     
   if (19<hic && 38>hic)              //LED for ambient heat index
    digitalWrite(b_third_led,HIGH);   //suitable for human health 
  else 
    digitalWrite(b_third_led,LOW);
    
   if (38<hic || hic<19)
    digitalWrite(r_third_led,HIGH);   //not suitable for human health
  else 
    digitalWrite(r_third_led,LOW);
}
