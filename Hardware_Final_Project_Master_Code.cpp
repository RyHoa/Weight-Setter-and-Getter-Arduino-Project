#include <SPI.h>

int slaveSelect;


int detectPin;
bool detectFlag;
bool prev;

int weightLoss; 

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
  
}

void loop() 
{ 
  detectSensor();

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
  digitalWrite(slaveSelect, LOW);
  SPI.transfer(count);
  delay(500);
  digitalWrite(slaveSelect, HIGH);  
  delay(500);
}