/*
 CheerLights IR Controller
 
 CheerLights API -> Particle Photon -> IR Transmitter
 
 This sketch gets the latest CheerLights color from ThingSpeak and
 transmits the color code over infrared to control IR lights.
 
 Parts:
   * SleepyLights Robot (http://amzn.to/2l8FCjM)
   * Particle Photon (http://amzn.to/2lTvsRe)
   * IR LED
   * 330Ω resistor
   * Breadboard
    
 Tutorial: http://nurseryhacks.com/nursery-robot-connected-cheerlights 
 
 Created: Feb 28, 2017 by Hans Scharler - http://nothans.com
 
 To join the CheerLights project, visit http://www.cheerlights.com
*/

// This #include statement was automatically added by the Particle IDE.
#include <ThingSpeak.h>

// This #include statement was automatically added by the Particle IDE.
#include <IRTransmitter.h>

#define IR_PIN D6
#define LED_PIN D7

IRTransmitter transmitter(IR_PIN, LED_PIN);

TCPClient client;
unsigned long cheerLightsChannelNumber = 1417;

// define ir remote control color buttons

// red
unsigned int red[67] = {8850,4400, 600,550, 550,550, 550,550, 550,550, 550,550, 550,550, 550,550, 600,500, 600,1600, 600,1600, 600,1600, 600,1600, 600,1650, 550,1650, 600,1600, 600,1600, 600,1600, 600,550, 550,1650, 550,550, 550,1650, 600,550, 500,550, 600,550, 550,550, 550,1600, 600,550, 550,1650, 600,550, 500,1650, 600,1600, 600,1650, 550};  // NEC FFA857

// green
unsigned int green[67] = {8850,4400, 600,500, 600,500, 650,450, 600,550, 600,450, 600,550, 550,550, 550,550, 600,1600, 600,1600, 600,1600, 600,1650, 550,1650, 550,1650, 600,1600, 600,1600, 600,1600, 600,500, 600,550, 600,1600, 550,1650, 550,550, 600,500, 650,450, 600,500, 600,1600, 600,1650, 550,500, 600,550, 550,1650, 550,1650, 550,1650, 600};  // NEC FF9867

// blue
unsigned int blue[67] = {8850,4400, 600,500, 600,500, 650,500, 550,500, 600,550, 600,500, 550,550, 550,550, 600,1600, 600,1600, 600,1600, 600,1600, 600,1650, 550,1650, 600,1600, 600,1650, 550,500, 600,1650, 550,500, 600,550, 600,1600, 600,500, 600,500, 600,500, 600,1650, 550,500, 600,1650, 550,1650, 600,500, 600,1600, 600,1600, 600,1650, 550};  // NEC FF48B7

// cyan
unsigned int cyan[67] = {8850,4400, 550,550, 550,550, 600,500, 550,550, 600,550, 550,500, 600,550, 550,500, 600,1600, 600,1650, 550,1650, 600,1600, 600,1600, 600,1600, 600,1600, 600,1650, 550,550, 600,550, 550,1600, 600,1650, 550,550, 550,550, 550,1650, 550,550, 600,1600, 600,1600, 600,550, 550,550, 550,1600, 600,1650, 550,550, 550,1650, 550};  // NEC FF32CD

// white
unsigned int white[67] = {8850,4400, 550,550, 600,550, 550,550, 550,550, 550,550, 550,550, 550,550, 550,550, 550,1650, 600,1600, 600,1600, 600,1600, 600,1650, 550,1650, 550,1650, 600,1600, 600,1600, 600,550, 550,550, 550,1650, 550,550, 550,550, 600,500, 550,550, 600,500, 600,1600, 600,1600, 600,550, 550,1650, 550,1650, 600,1600, 600,1600, 600};  // NEC FF906F

// oldlace
unsigned int oldlace[67] = {8850,4400, 550,550, 600,550, 550,550, 550,550, 550,550, 550,550, 550,550, 550,550, 550,1650, 600,1600, 600,1600, 600,1600, 600,1650, 550,1650, 550,1650, 600,1600, 600,1600, 600,550, 550,550, 550,1650, 550,550, 550,550, 600,500, 550,550, 600,500, 600,1600, 600,1600, 600,550, 550,1650, 550,1650, 600,1600, 600,1600, 600};  // NEC FF906F

// purple
unsigned int purple[67] = {8850,4400, 550,550, 550,550, 600,550, 500,550, 600,550, 550,550, 550,550, 550,550, 550,1600, 600,1650, 550,1650, 600,1600, 600,1600, 600,1600, 600,1600, 600,1650, 550,550, 600,500, 600,1600, 600,550, 550,550, 550,550, 550,550, 550,550, 550,1650, 600,1600, 600,550, 550,1600, 600,1650, 550,1650, 550,1650, 550,1650, 550};  // NEC FF20DF

// magenta
unsigned int magenta[67] = {8900,4350, 600,550, 550,550, 550,550, 550,550, 600,550, 500,550, 600,550, 550,550, 550,1600, 600,1600, 600,1650, 550,1650, 600,1600, 600,1600, 600,1650, 550,1650, 550,550, 600,1600, 600,1600, 600,500, 600,1650, 550,500, 600,550, 550,550, 550,1650, 600,500, 550,550, 600,1600, 600,550, 550,1650, 550,1650, 600,1600, 600};  // NEC FF6897

// yellow
unsigned int yellow[67] = {8900,4400, 550,550, 550,550, 550,550, 600,550, 500,550, 600,550, 550,550, 550,550, 550,1650, 550,1650, 550,1650, 550,1650, 550,1650, 600,1600, 600,1650, 550,1650, 600,550, 500,550, 600,550, 550,550, 550,550, 550,550, 550,1600, 600,550, 600,1600, 600,1600, 600,1600, 600,1600, 600,1650, 550,1650, 600,550, 500,1650, 600};  // NEC FF02FD

// orange 
unsigned int orange[67] = {8900,4350, 600,500, 600,500, 600,500, 650,500, 550,500, 600,550, 600,500, 550,550, 600,1600, 600,1600, 650,1550, 600,1650, 550,1650, 600,1600, 600,1600, 600,1600, 650,1550, 600,550, 600,1600, 550,1650, 550,550, 600,500, 600,1600, 600,500, 650,500, 550,1650, 600,500, 550,550, 600,1600, 600,1600, 650,500, 550,1650, 600};  // NEC FFB24D

// pink
unsigned int pink[67] = {8850,4400, 600,550, 550,550, 550,550, 550,550, 550,550, 550,550, 550,550, 550,550, 550,1650, 600,1600, 600,1600, 600,1600, 600,1600, 600,1650, 550,1650, 600,1600, 600,1650, 550,550, 550,550, 550,550, 550,1650, 600,550, 500,550, 600,550, 550,500, 600,1600, 600,1600, 600,1650, 550,550, 550,1650, 550,1650, 600,1600, 600};  // NEC FF8877


void setup() {
    ThingSpeak.begin(client);
}


void loop() {
    String color = ThingSpeak.readStringField(cheerLightsChannelNumber, 1);
    
         if (color.equals("red"))     { transmitter.Transmit(red, sizeof(red) / sizeof(red[0])); }
    else if (color.equals("green"))   { transmitter.Transmit(green, sizeof(green) / sizeof(green[0])); }
    else if (color.equals("blue"))    { transmitter.Transmit(blue, sizeof(blue) / sizeof(blue[0])); }
    else if (color.equals("cyan"))    { transmitter.Transmit(cyan, sizeof(cyan) / sizeof(cyan[0])); }
    else if (color.equals("white"))   { transmitter.Transmit(white, sizeof(white) / sizeof(white[0])); }
    else if (color.equals("oldlace")) { transmitter.Transmit(oldlace, sizeof(oldlace) / sizeof(oldlace[0])); }
    else if (color.equals("purple"))  { transmitter.Transmit(purple, sizeof(purple) / sizeof(purple[0])); }
    else if (color.equals("magenta")) { transmitter.Transmit(magenta, sizeof(magenta) / sizeof(magenta[0])); }
    else if (color.equals("yellow"))  { transmitter.Transmit(yellow, sizeof(yellow) / sizeof(yellow[0])); }
    else if (color.equals("orange"))  { transmitter.Transmit(orange, sizeof(orange) / sizeof(orange[0])); }
    else if (color.equals("pink"))    { transmitter.Transmit(pink, sizeof(pink) / sizeof(pink[0])); }
    
    // Check again in 5 seconds
    delay(5000);
}
