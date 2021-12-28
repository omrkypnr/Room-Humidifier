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
#include "DHT.h"        // DHT library
#define DHTPIN D4       // Digital pin where we connect the DHT11
#define DHTTYPE DHT11   // DHT type where we use to
#define relay D1        // Digital pin where we connect the relay
//--------------------------------------------------------------------------------
//Define Output Pins
int b_first_led = D3;     // First blue LED.
int b_second_led = D0;    // Second blue LED. 
int b_third_led = D5;     // Third blue LED.
int r_first_led = D6;     // First red LED.
int r_second_led = D7;    // Second red LED.
int r_third_led = D8;     // Third red LED.
DHT dht(DHTPIN, DHTTYPE); // DHT pin.
//-------------------------------------------------------------------------------------
// put your setup code here, to run once:
void setup() 
{
  //pinMode Output Pins
  pinMode (b_first_led,OUTPUT);   //First blue LED pin defined as OUTPUT.
  pinMode (b_second_led,OUTPUT);  //Second blue LED pin defined as OUTPUT.
  pinMode (b_third_led,OUTPUT);   //Third blue LED pin defined as OUTPUT.
  pinMode (r_first_led,OUTPUT);   //First red LED pin defined as OUTPUT.
  pinMode (r_second_led,OUTPUT);  //Second red LED pin defined as OUTPUT.
  pinMode (r_third_led,OUTPUT);   //Third red LED pin defined as OUTPUT.
  pinMode (relay,OUTPUT);         //Relay pin defined as OUTPUT.
  
  Serial.begin(9600);             //Sets the baud rate for serial data transmission.
  
  dht.begin();
}
//--------------------------------------------------------------------------------------
// put your main code here, to run repeatedly:
void loop() 
{
  delay(2000);                                  //delay time
  float h = dht.readHumidity();                 //read the humidity
  float t = dht.readTemperature();              //read the temprature (for *C)
  float f = dht.readTemperature(true);          //read the temprature (for *F)
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

  
//LED for ambient humidity
  if (61>h && 45<=h)                  
    digitalWrite(b_first_led,HIGH);   //Suitable for human health so turn on first blue LED.
  else 
    digitalWrite(b_first_led,LOW);    //Not suitable for human health so turn off first blue LED.
   
   if (61<h || 45>h)
    digitalWrite(r_first_led,HIGH);   //Not suitable for human health so turn on first red LED. 
  else 
    digitalWrite(r_first_led,LOW);    //Suitable for human health so turn off first red LED.
//Relay switching for humidifier
   if (h<45)                           
     digitalWrite(relay,LOW);         //Current is flowing
   else
     digitalWrite(relay,HIGH);        //current is not flowing
//LED for ambient temperature
   if (17<t && 33>t)                  
    digitalWrite(b_second_led,HIGH);  //Suitable for human health so turn on second blue LED.
  else 
    digitalWrite(b_second_led,LOW);   //Not suitable for human health so turn off second blue LED.
    
   if (33<t || t<17) 
    digitalWrite(r_second_led,HIGH);  //Not suitable for human health so turn on second red LED.
  else 
    digitalWrite(r_second_led,LOW);   //Suitable for human health so turn off second red LED.
//Relay switching for heater
   //if (t<17)
    //digitalWrite(relay,LOW);        //If a heater is connected instead of a humidifier
   //else
    //digitalWrite(relay,HIGH);       //If a heater is connected instead of a humidifier
//LED for ambient heat index     
   if (19<hic && 38>hic)              
    digitalWrite(b_third_led,HIGH);   //Suitable for human health so turn on third blue LED.
  else 
    digitalWrite(b_third_led,LOW);    //Not suitable for human health so turn off third blue LED.
    
   if (38<hic || hic<19)
    digitalWrite(r_third_led,HIGH);   //Not suitable for human health so turn on third red LED.
  else 
    digitalWrite(r_third_led,LOW);    //Suitable for human health so turn off third red LED.
}
