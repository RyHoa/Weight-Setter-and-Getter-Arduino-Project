#include <SPI.h> //included this library to use SPI connection
#include <Tiny4kOLED.h> //included this library to use oled screen

int slaveSelect = 10; //slave select pin for SPI to send information to

int sendPin = 2;  //pin to call the method to send the information to the other arduino


//ir led and photo transistor setup
int detectPin = 3; //pin to detect changes in sensor
bool detectFlag; //flag that is set to what the sensor reads
bool prev; //prev condition of the detect flag

volatile byte weightLoss = 0;  //int to indicate how much weight you have lost
volatile bool screenReset = 0;

void setup() 
{
  pinMode(detectPin, INPUT); //set pin to input since we want it to read values
  pinMode(slaveSelect, OUTPUT); //set to output because we need to pull low to send information and then pull high 
  pinMode(sendPin, INPUT); //button press to send the weight to the slave
  
  Serial.begin(9600); //begin comms
  SPI.begin(); //begin SPI library setup
  digitalWrite(slaveSelect, HIGH); //write high to slaveselect since we are not sending anything yet

//   https://docs.arduino.cc/tutorials/generic/introduction-to-the-serial-peripheral-interface
//   SPCR
// | 7    | 6    | 5    | 4    | 3    | 2    | 1    | 0    |
// | SPIE | SPE  | DORD | MSTR | CPOL | CPHA | SPR1 | SPR0 |
// SPIE - Enables the SPI interrupt when 1
// SPE - Enables the SPI when 1
// DORD - Sends data least Significant Bit First when 1, most Significant Bit first when 0
// MSTR - Sets the Arduino in controller mode when 1, peripheral mode when 0
// CPOL - Sets the data clock to be idle when high if set to 1, idle when low if set to 0
// CPHA - Samples data on the falling edge of the data clock when 1, rising edge when 0
// SPR1 and SPR0 - Sets the SPI speed, 00 is fastest (4MHz) 11 is slowest (250KHz)

  SPCR |= 0b11010011; //turns on SPIE (enable SPI interrupt), turns on SPE (enables SPI)

  attachInterrupt(digitalPinToInterrupt(sendPin),sendWeight,RISING); //interrupt command on the sendPin to call sendWeight method on the rising edge
  oled.begin(); //begin oled and start i2c communication

  // Two fonts: FONT8X16 and FONT6X8
  oled.setFont(FONT8X16);

  //to clear all the memory, clear both rendering frames
  oled.clear();
  oled.switchRenderFrame();
  oled.clear();
  oled.switchRenderFrame();

  
}

void loop() 
{ 

  if(screenReset)
  {
    oled.clear();
    screenReset = false;
  }
  detectSensor(); //calls the ir led and photo transistor sensor method 
  oled.setCursor(0,0); //set position on oled
  oled.print("Today's "); //print on oled
  oled.setCursor(0,2); //set position on oled
  oled.print("WLoss (lbs): "); //print on oled
  oled.print(weightLoss); //print on oled
  oled.on(); //turn on oled
  delay(500); //delay 500ms
  


}
void detectSensor()
{
  detectFlag = digitalRead(detectPin); //flag reads the detectPin volt and give it high or low 
  if(detectFlag != prev && detectFlag) //if detectFlag is not the same as prev flag 
  {    
    weightLoss++; //increase weightLoss by 1
    
  }  
  prev = detectFlag; //set prev to detectFlag
}

//method to send weight using SPI
void sendWeight()
{  

  digitalWrite(slaveSelect, LOW); //put slaveSelect low
  SPI.transfer(weightLoss); //transfer value to SPI
  delay(500); // This command will make the Arduino enter an idle state for approximately 0.5 seconds, also commonly stated in the more appropriate measure of time: 500 milliseconds.
  digitalWrite(slaveSelect, HIGH); //put slaveSelect high  
  delay(500); //delay 500 ms
  weightLoss = 0; //set weightloss to 0
  screenReset = true;
 
}