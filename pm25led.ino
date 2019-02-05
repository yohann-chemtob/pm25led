#include "SdsDustSensor.h"
#include <Adafruit_DotStar.h>
#include <SPI.h>
#define NUMPIXELS 14 

int rxPin = 3;
int txPin = 2;
SdsDustSensor sds(rxPin, txPin);

#define DATAPIN    4
#define CLOCKPIN   5
Adafruit_DotStar strip = Adafruit_DotStar(NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_BRG);

float minPM = 5;
float maxPM = 20;

void setup() {

  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000L)
  clock_prescale_set(clock_div_1); // Enable 16 MHz on Trinket
  #endif
  
  strip.begin(); // Initialize pins for output
  strip.show();

  Serial.begin(9600);
  sds.begin();

  Serial.println(sds.queryFirmwareVersion().toString()); // prints firmware version
  Serial.println(sds.setActiveReportingMode().toString()); // ensures sensor is in 'active' reporting mode
  Serial.println(sds.setContinuousWorkingPeriod().toString()); // ensures sensor has continuous working period - default but not recommended
  strip.setBrightness(8);
}


void loop() {
  
  PmResult pm = sds.readPm();
  if (pm.isOk()) {
    Serial.print("PM2.5 = ");
    Serial.print(pm.pm25);
    Serial.print(", PM10 = ");
    Serial.println(pm.pm10);
    pmled_strip(pm.pm25);

    // if you want to just print the measured values, you can use toString() method as well
    Serial.println(pm.toString());
  } else {
    // notice that loop delay is set to 0.5s and some reads are not available
    Serial.print("Could not read values from sensor, reason: ");
    Serial.println(pm.statusToString());
  }

  delay(500);
}




void pmled_strip(float pmvalu){
  float ledtofire = ((pmvalu-minPM)/(maxPM-minPM))*(float)strip.numPixels();
  Serial.println("Numb of led to power = ");
  Serial.println(ledtofire);

  for(int i=0; i<strip.numPixels(); i++) {
    if(((int)ledtofire-1)>=(i)){
      strip.setPixelColor(i, map(i,0,strip.numPixels(),200,0), map(i,0,strip.numPixels(),0,255), 0); 
      Serial.print(i);
      Serial.println(" On");
    }
    else strip.setPixelColor(i,strip.Color(0,0,0));
  }
  strip.show();

}
