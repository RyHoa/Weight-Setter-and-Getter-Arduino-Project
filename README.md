# Weight-Setter-and-Getter-Arduino-Project
Authors: Elijah Gibson, Ryan Hoang, Trajan Salinas, and Bethany Sumner

## Summary 

The project allows users to track the weight they have lost between two Arduinos using
SPI. The purpose is to provide users with a way to visually track the number of pounds they have
lost and their current weight. There are two Arduinos, A and B. On Arduino A, users will have
the option to set their weight using the rotary encoder. This will be displayed on an OLED
screen. Arduino B will track the weight loss through an IR LED and phototransistor sensor setup
to count the ‘pounds’ placed in the jar, indicating the number of pounds lost. Once the user
presses the pushbutton on Arduino B, the data is sent to Arduino A through SPI, updating the
current weight on Arduino A. The result is an accurate count for the weight and weight loss on
the two screens. The Analog Discovery 2 tool will be able to measure the SPI data which is the
pounds lost by the user. Overall, the project demonstrates how to use circuitry and SPI
communication in a practical way to track weight loss. It also demonstrates the usage of Analog
Discovery 2 tool on the SPI communication between two microcontrollers.

## Introduction 

Weight loss is a common goal for many individuals, and there are numerous methods
available to help achieve it. This includes the ‘weight loss jar’ which is popular on many social
media platforms such as Instagram. With this method users will place various objects such as a
marble/pebble, folded paper, or a popsicle stick into a jar to represent each pound they have lost
in their weight loss journey. This method is a popular way to track weight loss, as users are able
to see their progress which will motivate them. They will also have the satisfaction of adding
another ‘pound loss’ to their jar. In this report, we present a project that replicates a weight loss
jar using Arduino boards that count the progress and updates it for the user. This device makes it
easy for those on a long weight loss journey to keep track of how much they have lost. For
instance, without the automated counter, users who have lost a lot of weight may have to
continuously recount how many pounds they have already put into the jar. Likewise, if someone
takes a gap in their weight loss journey, they may forget how much they previously placed into
the jar, but by having the Arduino weight loss jar it ensures the user will know exactly how many
pounds they have inputted, even after a period of time.
The project utilizes two boards, one for user input of current weight and the other for
tracking the weight lost. The user will input their current weight by pressing a pushbutton,
turning a rotary encoder, and then pressing a pushbutton again to lock it. This will be displayed
on the OLED screen. The weight loss is tracked by using an IR LED and phototransistor to
detect how many ‘pounds’ are placed into the jar, which is displayed on a second OLED screen.
The primary objective of this experiment is to implement serial communication (SPI) between
two Arduinos where Arduino B will send the weight loss to the user inputted weight on Arduino
A, resulting in Arduino A having an accurate updated weight. The project provides an enhanced
version of a popular weight loss tracking tool, where users will not have to worry about manually
keeping track of their weight loss while still having a motivated visual to look at.

## Equipment

- 2x Arduinos
- 2x Breadboard
- 2x OLED Screen
- 1x IR LED
- 1x Phototransistor
- 1x Rotary Encoder
- 2x pushbuttons
- 1x 100 Ohm resistor
- 4x 1k resistor
- 3x 10k resistor
- 4x 0.10 microFarad Capacitor
- Analog Discovery 2
- Wires

## Procedure

In order to simulate a weight loss jar, we made our system take in 3 inputs: SetWeight,
SendWeight, and Pebbles. As output, the system will display weight loss
(DisplayWeightLossOnOLEDA) and the current weight (DisplayWeightOnOLEDB)

![figure1](https://github.com/RyHoa/Weight-Setter-and-Getter-Arduino-Project/assets/129560634/a79748ac-1586-42d0-b0b0-13c3c1db5054)

Figure 1: Block Diagram

This system was split into two main parts. System 1 (Slave) lets the user set their initial
weight via a rotary encoder. System 2 (Master) uses a phototransistor and IR led to detect the
“pebbles'' and calculate weight loss. Using SPI, the two Adruino Unos will communicate with
one being the master and the other being the slave. The master controller will keep the
information for how much weight was lost and send that information to the slave updating its
current weight. Data should only be taken from the MOSI line since the master is the only one
sending valid information.
To build System 1, we used 1x Arduino Uno, 1x Breadboard, 1x Rotary Encoder, 1x 10k
resistor, 3x 1k resistor, 1x OLED Screen, 3x 0.10 microFarad Capacitor, 1x Pushbutton, and
wires. We built it exactly as the circuit diagram below.
Figure 2: Circuit Diagram for System 1
To build System 2, we used 1x Arduino Uno, 1x Breadboard, 1x OLED Screen, 1x
phototransistor, 1x IR LED, 1x 0.10 microFarad Capacitor, 1x Pushbutton, 2x 10k resistors,1x
100 ohm resistor, 1x 1k resistor, and wires. We built it exactly as the circuit diagram below.

![figure2](https://github.com/RyHoa/Weight-Setter-and-Getter-Arduino-Project/assets/129560634/b8c413c4-49a6-4b9b-8ad5-1467e88eba8d)

Figure 2: Circuit Diagram for System 2

After building the circuits separately, we connected the two together using pins 13,12,11,10 on
the Arduino. These are the SPI pins. We have to use this pins, as the SPI library uses it. If we
wanted to manually bit bang we could use any pins. For both circuits, the 10k resistor and
capacitor are included to counter button bouncing. They are not needed, as software can counter
this issue, but having them alongside software helps fix double boucing. After that we soldered
the circuit. It should match the circuit diagram below.

![figure3](https://github.com/RyHoa/Weight-Setter-and-Getter-Arduino-Project/assets/129560634/db6e117a-882e-4c12-aae7-9f4553240e5e)

Figure 3: Total Circuit Diagram

After building the system, we wrote the code with our communication protocol being SPI.
With the system built we had to test it before presenting. We filmed two videos, one was for the
system and the other was using the analog discovery. To prove that the data is transmitted
correctly we used the Analog Discovery 2 and the Waveforms application to display the data
being passed using SPI. It includes the MOSI and MISO but we only want MOSI in our
application since only the master is sending the weightloss to the slave (Master Out Slave In).
We did this by splitting the wires at pins 13,12,11,10 of both boards into a separate breadboard
and connected them to the Analog Discover Input/Output pins. So on one connective rail on the
breadboard, it would have the pin 13 from System 1, a Analog Discover Pin, and then pin 13
from System 2. We repeated this process on new connective rails with the rest of the SPI pins
(13,12,11,10). So in the end, we had used 4 rails with 3 connections in each of them. This
allowed us to read the data. At the start we were getting uninitialized data reading but after
setting the SPI frequency in the code to 250 kHz we got readings. We set our samples to 10 M
and were able to capture the information. As you can see here we got 2 pounds sent on the MOSI
side.

![figure4](https://github.com/RyHoa/Weight-Setter-and-Getter-Arduino-Project/assets/129560634/d9deb7fe-bff4-419e-aadc-4705d3a6fa73)

Figure 4: Analog Discovery 2 data

## Conclusion

In conclusion, this project successfully showcased two Arduinos using the serial
communication protocol SPI to transmit weight loss data, providing an accurate way to track
weight loss progress. The rotary encoder allowed users to have an easy way to input their current
weight and the OLED screen provided a clear display so users could see what data they were
about to enter. The pounds placed into the jar were accurately counted by the phototransistor/IR
LED and accounted for when calculating the new updated current weight. This was also proven
through the use of the Analog Discovery 2 tool. Overall, the objectives of the experiment were
achieved by using two Arduino boards to track weight loss and providing users with an enhanced
version of a popular weight loss tracking method.
