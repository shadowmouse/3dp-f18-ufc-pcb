#include <Encoder.h>

/* Complete USB Joystick Example
   Teensy becomes a USB joystick with 16 or 32 buttons and 6 axis input

   You must select Joystick from the "Tools > USB Type" menu

   Pushbuttons should be connected between the digital pins and ground.
   Potentiometers should be connected to analog inputs 0 to 5.

   This example code is in the public domain.
*/

// Configure the number of buttons.  Be careful not
// to use a pin for both a digital button and analog
// axis.  The pullup resistor will interfere with
// the analog voltage.
const int numButtons = 32;  // 16 for Teensy, 32 for Teensy++
Encoder leftKnob(21,22);
Encoder rightKnob(31,30);

void setup() {
  // you can print to the serial monitor while the joystick is active!
  Serial.begin(9600);
  // configure the joystick to manual send mode.  This gives precise
  // control over when the computer receives updates, but it does
  // require you to manually call Joystick.send_now().
  Joystick.useManualSend(true);
  for (int i=0; i<numButtons; i++) {
    if ( i == 21 || i == 22 || i == 30 || i == 31 ) { continue; }
    pinMode(i, INPUT_PULLUP);
  }
  Serial.println("Begin Complete Joystick Test");
}

byte allButtons[numButtons];
byte prevButtons[numButtons];
int angle=0;

long lp = 0;
long rp = 0;

void loop() {
  // read 6 analog inputs and use them for the joystick axis
  Joystick.X(analogRead(33));
  Joystick.Y(analogRead(34));
  Joystick.Z(analogRead(35));
//  Joystick.Zrotate(analogRead(3));
//  Joystick.sliderLeft(analogRead(4));
//  Joystick.sliderRight(analogRead(5));
  
  // read digital pins and use them for the buttons
  for (int i=0; i<numButtons; i++) {
    if ( i == 21 || i == 22 || i == 30 || i == 31 ) { continue; }
    if (digitalRead(i)) {
      // when a pin reads high, the button is not pressed
      // the pullup resistor creates the "on" signals
      allButtons[i] = 0;
    } else {
      // when a pin reads low, the button is connecting to ground.
      allButtons[i] = 1;
    }  
  }

  // Right Encoder
  long rnp = rightKnob.read();
  allButtons[21] = 0;
  allButtons[22] = 0;
  if(rnp != rp) {
    rp = rnp;
    if(rnp >= 4) { 
      Serial.println("Right Knob Left");
      rightKnob.write(0);
      allButtons[22] = 1;
    }

    if(rnp <= -4) { 
      Serial.println("Right Knob Right");
      rightKnob.write(0);
      allButtons[21] = 1;
    }
  }

  // Left Encoder
  long lnp = leftKnob.read();
  allButtons[30] = 0;
  allButtons[31] = 0;
  if(lnp != lp) {
    lp = lnp;
    if(lnp >= 4) { 
      Serial.println("Left Knob Left");
      leftKnob.write(0);
      allButtons[31] = 1;
    }

    if(lnp <= -4) { 
      Serial.println("Left Knob Right");
      leftKnob.write(0);
      allButtons[30] = 1;
    }
  }
 
  for (int i=0; i<numButtons; i++) {
    Joystick.button(i + 1, allButtons[i]);
  }
  
  Joystick.send_now();
  
  // check to see if any button changed since last time
  boolean anyChange = false;
  for (int i=0; i<numButtons; i++) {
    if (allButtons[i] != prevButtons[i]) anyChange = true;
    prevButtons[i] = allButtons[i];
  }
  
  // if any button changed, print them to the serial monitor
  if (anyChange) {
    Serial.print("Buttons: ");
    for (int i=0; i<numButtons; i++) {
      Serial.print(allButtons[i], DEC);
    }
    Serial.println();
  }
  
  // a brief delay, so this runs "only" 200 times per second
  delay(5);
}
