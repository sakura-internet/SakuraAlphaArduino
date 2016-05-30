#include <SakuraAlpha.h>

#define BUF_LEN 16

uint32_t cnt = 0;

SakuraAlphaI2C sakura;

void setup() {
  Serial.begin(9600);  // start serial for output
  
  Serial.println("Waiting to come online...");
  for(;;){
    if( sakura.getNetworkStatus() == 1 ) break;
    delay(1000);
  }
}

uint8_t updated[BUF_LEN];

void loop() {
  cnt++;
  Serial.println(cnt);
  sakura.writeChannel(0,cnt);
  sakura.writeChannel(1,cnt);
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

