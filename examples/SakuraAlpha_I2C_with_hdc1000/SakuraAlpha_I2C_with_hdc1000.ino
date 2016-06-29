#include <SakuraAlpha.h>

#define BUF_LEN 16

uint32_t cnt = 0;

SakuraAlphaI2C sakura;

#include "HDC1000.h"

HDC1000 hdc1000;

void setup() {

  hdc1000.begin();
  
  Serial.begin(9600);  // start serial for output
  
  Serial.println("Waiting to come online...");
  for(;;){
    if( sakura.getNetworkStatus() == 1 ) break;
    delay(1000);
  }
}

uint8_t updated[BUF_LEN];

void loop() {

  float temp = hdc1000.getTemperature();

  if (temp == HDC1000_ERROR_CODE) {
    hdc1000.begin(); // retry once
    temp = hdc1000.getTemperature();
  }
  Serial.print("Temperature: ");
  Serial.println(temp);

  float humi = hdc1000.getHumidity();

  if (humi== HDC1000_ERROR_CODE) {
    hdc1000.begin(); // retry once
    humi = hdc1000.getHumidity();
  }

  Serial.print("Humidity: ");
  Serial.println(humi);
  
  cnt++;
  Serial.println(cnt);
  sakura.writeChannel(0,temp);
  sakura.writeChannel(1,humi);
  sakura.writeChannel(2,cnt);
  delay(250);

  int num_updated = sakura.getUpdatedChannels(updated, BUF_LEN);
  Serial.print(num_updated);
  Serial.print(" updated channels: ");
  for (int i = 0; i < min(num_updated,BUF_LEN); i++) {
    Serial.print(' ');
    Serial.print((int)updated[i]);
  }
  Serial.println("");
  
  char type=0;
  byte value[8]={0};
  
  sakura.readChannel(0, &type, value);

  for (int i = 0; i < 8; i++) {
    Serial.print(' ');
    Serial.print(value[i],HEX);
  }

  Serial.print(" type:");
  Serial.print(type);
  Serial.println("");
  delay(250);

  Serial.print("Tx[0] status: ");
  Serial.print( sakura.getTxChannelStatus(0) );
  Serial.println("");

  sakura.transmit(TRANSMIT_ONCE);
  delay(250);

  num_updated = sakura.getUntransmittedChannels(updated, BUF_LEN);
  Serial.print(num_updated);
  Serial.print(" untransmitted channels: ");
  for (int i = 0; i < min(num_updated,BUF_LEN); i++) {
    Serial.print(' ');
    Serial.print((int)updated[i]);
  }
  Serial.println("");
  delay(1000);

  Serial.println("");
}
