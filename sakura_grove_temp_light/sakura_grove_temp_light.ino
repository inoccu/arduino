#include <SakuraIO.h>

SakuraIO_I2C sakuraio;

// Grove Temperature Sensor
const int pinTemp = A0;
float temperature;
int B = 3975;
float resistance;

// Grove Light Sensor
const int pinLight = A1;

void setup() {
  Serial.begin(9600);
  Serial.print("Waiting to come online");
  for(;;){
    if( (sakuraio.getConnectionStatus() & 0x80) == 0x80 ) break;
    Serial.print(".");
    delay(1000);
  }
  Serial.println("");
}

void loop() {
  int val;
  int ret;
  
  // Temperature
  val = analogRead(pinTemp);
  resistance = (float)(1023-val)*10000/val;
  temperature = 1/(log(resistance/10000)/B+1/298.15)-273.15;
  ret = sakuraio.enqueueTx((uint8_t)0, temperature);
  Serial.print("Temperature ");
  Serial.println(temperature);

  // Light
  val = analogRead(pinLight);
  ret = sakuraio.enqueueTx((uint8_t)1, (int32_t)val);
  Serial.print("Light ");
  Serial.println(val);

  uint8_t avail;
  uint8_t queued;
  sakuraio.getTxQueueLength(&avail, &queued);
  Serial.print("Tx Available=");
  Serial.print(avail);
  Serial.print(" Queued=");
  Serial.println(queued);

  if(queued >= 30){
    ret = sakuraio.clearTx();
    Serial.print("Clear ");
    Serial.println(ret);
  }else if(queued >= 5){
    ret = sakuraio.send();
    Serial.print("Send ");
    Serial.println(ret);
  }

  delay(5000);
}
