//
//
//

#include <Tiny4kOLED.h>
#include<SPI.h>








float weight = 100; //set current weight to this


//set weight using rotary encoder
int outA = 3;
int outB = 4;
float temp;
bool currA;
bool currB;
bool prevA = 0; 


float weight = 100; //set current weight to this
float incrementFactor = 5; //how much to increment the weight
int setWeightPin = 2; //uses an interrupt pin to setWeight
volatile bool setWeightFlag = 0; //flag used in interrupt method

byte weightLoss; //value for weighloss



void setup() 
{
  
  Serial.begin(9600); //set Commns with serial
  //SPI.begin();
  
  //set pins to INPUT pin
  pinMode(outA, INPUT);
  pinMode(outB, INPUT);
  pinMode(setWeightPin, INPUT);

  //ISR command on setWeightPin to call toggleWeight on the RISING edge
  attachInterrupt(digitalPinToInterrupt(setWeightPin), toggleWeight, RISING); 



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

  //SPI setup
  SPCR |= 0b11000000; //turns on SPIE (enable SPI interrupt), turns on SPE (enables SPI)
  pinMode(MISO, OUTPUT); //for slave to send Data to master


  //oled setup
  oled.begin();
  oled.on();
  oled.clear();
  oled.setFont(FONT8X16);
  oled.clear();
  oled.switchRenderFrame();
  oled.clear();
  oled.switchRenderFrame();
  
  prevA = digitalRead(outA); //set prevA to read of outA
}

ISR (SPI_STC_vect) //SPI interrupt routine
{
  
  weightLoss = SPDR; //read data from the SPI data register
  Serial.println("weightLoss");
  Serial.println(weightLoss);
  weight -= weightLoss; //adjust the weight from the data received
}



void loop() 
{  
  setWeight(); //method to set weight
  printOLED(); //method to print to screen
}

//method to print weight on the LCD screen
void printOLED()
{
  
  oled.setCursor(0,0); //set position on oled
  oled.print("CWeight:"); //print statement on oled

  oled.setCursor(0,2); //set position on oled
  oled.print(weight); //print statement on weight
  oled.print(" lbs");  //print statement 
  //Serial.print(weight);
  //Serial.println(" lbs");
  //delay(1000);
  //oled.clear();
  //delay(1000);

}

//method to set weight
void setWeight()
{
  if(setWeightFlag)
  {
    //bool to read outA and outB    
    currA = digitalRead(outA);
    currB = digitalRead(outB);
    // Serial.print("currA ");
    // Serial.println(currA);
    // Serial.print("currB ");
    // Serial.println(currB);

    if((currA != prevA) && currA) //if current state is not equal to previous state and outA is high
    {
      if (currB) //if b is "low"
      {
        weight+=incrementFactor; //increment count  
        //Serial.println("hi"); 
        delay(20);     
      }
      else if(!currB && (weight - incrementFactor > 0)) //if b is "high" and decrement won't go below 0
      {
        weight-=incrementFactor; //decrement count  
       // Serial.println("bye"); 
        delay(20); 
      }        
    }  
  prevA = currA; //current state sent to previous state

  //clear the screen only if the weight changes
  if (temp != weight)
  {
    temp = weight;
    oled.clear();
  }
}
}
void toggleWeight()
{
  setWeightFlag = !setWeightFlag; //switch flag value
  //Serial.print("setWeightFlag = ");
  //Serial.println(setWeightFlag);
  delay(20); //delay for 20 ms
}
