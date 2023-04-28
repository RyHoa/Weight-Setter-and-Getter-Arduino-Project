#include <SPI.h>
#include <Tiny4kOLED.h>

int slaveSelect;


int detectPin = 3;
bool detectFlag;
bool prev;

int weightLoss = 0; 

int sendPin = 2; 


void setup() 
{
  pinMode(detectPin, INPUT);
  pinMode(slaveSelect, OUTPUT);

  pinMode(sendPin, INPUT); //button press to send the weight to the slave
  
  Serial.begin(9600);
  SPI.begin();
  digitalWrite(slaveSelect, HIGH);

  attachInterrupt(digitalPinToInterrupt(sendPin),sendWeight,RISING);
  oled.begin();
  oled.setFont(FONT8X16);
  oled.clear();
  oled.switchRenderFrame();
  oled.clear();
  oled.switchRenderFrame();
}

void loop() 
{ 
  detectSensor();
  oled.setCursor(0,0);
  oled.print("Today's ");
  oled.setCursor(0,2);
  oled.print("WLoss (lbs): ");
  oled.print(weightLoss);
  oled.on();
  delay(500);


}
void detectSensor()
{
  detectFlag = digitalRead(detectPin);
  if(detectFlag != prev)
  {    
    weightLoss++;
    prev = detectFlag;
  }  
}
void sendWeight()
{
  weightLoss = 0;
  digitalWrite(slaveSelect, LOW);
  SPI.transfer(weightLoss);
  delay(500);
  digitalWrite(slaveSelect, HIGH);  
  delay(500);
}