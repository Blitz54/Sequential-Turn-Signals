///////////////////////////////////////////////////
//                                               //
//    Sequential turn signals for Pontiac Fiero  //
//                                               //
//    Made by: Blitz54                           //
//    V1.0 - September 4, 2024                   //
//                                               //
///////////////////////////////////////////////////


//        NOTES        //
// Currently no debouncing on the signals and brake inputs.
// Functions fine for what it is, but something to consider.
// Not sure if proper debouncing with interrupt pin via software is possible,
// there are some libraries online that supposedly work so it
// might be worth looking into down the road.
//

#define LEFT_SIGNAL_INPUT A1 
#define RIGHT_SIGNAL_INPUT A2 
#define BRAKE_INPUT 3

#define ON LOW
#define OFF HIGH

#define DELAYMILLIS 200 //ms

#define MAX_REPEAT_ON_TAP 3

const int leftMarkers = 4;
const int leftOuter = 5;
const int leftMiddle = 6; 
const int leftInner = 7;
const int rightInner = 8; 
const int rightMiddle = 9; 
const int rightOuter = 10; 
const int rightMarkers = 11;

int tapmillis = 200;
int LeftFlashCount = 0;
int RightFlashCount = 0;
bool isLeftTap = false;
bool isRightTap = false;

void setup() {
  pinMode(leftInner, OUTPUT);
  pinMode(leftMiddle, OUTPUT);
  pinMode(leftOuter, OUTPUT);
  pinMode(rightInner, OUTPUT); 
  pinMode(rightMiddle, OUTPUT);
  pinMode(rightOuter, OUTPUT);
  pinMode(leftMarkers, OUTPUT);
  pinMode(rightMarkers, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(BRAKE_INPUT), brakeInterrupt, CHANGE);
  off();
  pinMode(LEFT_SIGNAL_INPUT, INPUT_PULLUP);
  pinMode(RIGHT_SIGNAL_INPUT, INPUT_PULLUP);
  pinMode(BRAKE_INPUT, INPUT_PULLUP);
}


void LeftLights(boolean leftInnerPower,boolean leftMiddlePower,boolean leftOuterPower,boolean leftMarkersPower){
  digitalWrite(leftInner,leftInnerPower);
  digitalWrite(leftMiddle,leftMiddlePower);
  digitalWrite(leftOuter,leftOuterPower);
  digitalWrite(leftMarkers,leftMarkersPower);
}

void LeftSequence(){
  isLeftTap = true;
  delay(DELAYMILLIS);
  LeftLights(ON, OFF, OFF, OFF);
  delay(DELAYMILLIS);
  LeftLights(ON, ON, OFF, ON);
  delay(DELAYMILLIS);
  LeftLights(ON, ON, ON, ON);
  delay(DELAYMILLIS);
  LeftOff();
  LeftFlashCount++;
}

void RightLights(boolean rightInnerPower,boolean rightMiddlePower,boolean rightOuterPower,boolean rightMarkersPower){
  digitalWrite(rightInner,rightInnerPower);
  digitalWrite(rightMiddle,rightMiddlePower);
  digitalWrite(rightOuter,rightOuterPower);
  digitalWrite(rightMarkers,rightMarkersPower);
}

void RightSequence(){
  isRightTap = true;
  delay(DELAYMILLIS);
  RightLights(ON, OFF, OFF, OFF);
  delay(DELAYMILLIS);
  RightLights(ON, ON, OFF, ON);
  delay(DELAYMILLIS);
  RightLights(ON, ON, ON, ON);
  delay(DELAYMILLIS);
  RightOff();
  RightFlashCount++;
}

void HazardFlashingNoBrakes(){
  RightLights(OFF, ON, ON, ON);
  LeftLights(OFF, ON, ON, ON);
  delay(380);
  off();
  delay(300);
}

void HazardFlashingWithBrakes(){
  RightLights(ON, ON, ON, ON);
  LeftLights(ON, ON, ON, ON);
  delay(380);
  RightLights(OFF, ON, ON, OFF);
  LeftLights(OFF, ON, ON, OFF);
  delay(300);
}

void BrakeOnlyOn(){
  LeftLights(OFF,ON,ON,OFF);
  RightLights(OFF,ON,ON,OFF);
}

void BrakesLeft(){
  LeftLights(OFF,ON,ON,OFF);
}

void BrakesRight(){
  RightLights(OFF,ON,ON,OFF);
}

void off(){
  LeftLights(OFF,OFF,OFF,OFF);
  RightLights(OFF,OFF,OFF,OFF);
}

void LeftOff(){
  LeftLights(OFF,OFF,OFF,OFF);
}

void RightOff(){
  RightLights(OFF,OFF,OFF,OFF);
}

void brakeInterrupt()
{
   // 1. Applying brakes
   if(digitalRead(BRAKE_INPUT) == 0)
   {
      // 1.2. Here is if the right turn signal and brake are on.
      if( (digitalRead(LEFT_SIGNAL_INPUT) == 1) and ( (digitalRead(RIGHT_SIGNAL_INPUT) == 0) || (isRightTap) ) )
      {         
        BrakesLeft();
      }
      
      // 1.3. Now if the left turn signal and brake are both on
      else if( (digitalRead(RIGHT_SIGNAL_INPUT) == 1) and ( (digitalRead(LEFT_SIGNAL_INPUT) == 0) || (isLeftTap) ) )
      {
        BrakesRight();         
      }
    } 
    // 2. Releasing Brakes
    else
    {       
      // 2.2. Here is if the right turn signal is on and brake is released.
      if( (digitalRead(LEFT_SIGNAL_INPUT) == 1) and ( (digitalRead(RIGHT_SIGNAL_INPUT) == 0) || (isRightTap) ) )
      {         
        LeftOff();
      }
      
      // 2.3. Now if the left turn signal is on and brake is released.
      else if( (digitalRead(RIGHT_SIGNAL_INPUT) == 1) and ( (digitalRead(LEFT_SIGNAL_INPUT) == 0) || (isLeftTap) ) )
      {
        RightOff();              
      }
   }
}

void loop() {
switch (digitalRead(BRAKE_INPUT)){
  case 0: // Brakes on (grounded)
    if ((digitalRead(LEFT_SIGNAL_INPUT) == 1) && (digitalRead(RIGHT_SIGNAL_INPUT) == 1)){ //if both signals are off, check if one was tapped.
      if((LeftFlashCount > 0) && (LeftFlashCount < MAX_REPEAT_ON_TAP)){ //checks if left signal flashed 1 or 2 times. Flashers always flash 3 times, unless other direction is engaged.
        RightFlashCount = 0;
        isRightTap = false;
        LeftSequence();
        break;
      }
      else if((RightFlashCount > 0) && (RightFlashCount < MAX_REPEAT_ON_TAP)){//checks right signal. Same as left.
        LeftFlashCount = 0;
        isLeftTap = false;
        RightSequence();
        break;
      }
      else{ //will run when the most recent flasher count hits 3, and the other is 0. Clears the counter so tap works again next time.
        BrakeOnlyOn();
        isLeftTap = false;
        isRightTap = false;
        LeftFlashCount = 0;
        RightFlashCount = 0;
        break;
      }
    }
    else if((digitalRead(LEFT_SIGNAL_INPUT) == 1) && (digitalRead(RIGHT_SIGNAL_INPUT) == 0)){
      LeftFlashCount = 0;
      BrakesLeft();
      RightSequence();
      break;
      }
    else if((digitalRead(LEFT_SIGNAL_INPUT) == 0) && (digitalRead(RIGHT_SIGNAL_INPUT) == 1)){
      RightFlashCount = 0;
      BrakesRight();
      LeftSequence();
      break;
      }
    else if((digitalRead(LEFT_SIGNAL_INPUT) == 0) && (digitalRead(RIGHT_SIGNAL_INPUT) == 0)){
      LeftFlashCount = 0;
      RightFlashCount = 0;
      HazardFlashingWithBrakes();
      break;
      }    
  case 1: //Brakes off (pin not grounded)
    if ((digitalRead(LEFT_SIGNAL_INPUT) == 1) && (digitalRead(RIGHT_SIGNAL_INPUT) == 1)){
      if((LeftFlashCount > 0) && (LeftFlashCount < MAX_REPEAT_ON_TAP)){
        RightFlashCount = 0;
        isRightTap = false;
        LeftSequence();
        break;
      }
      else if((RightFlashCount > 0) && (RightFlashCount < MAX_REPEAT_ON_TAP)){
        LeftFlashCount = 0;
        isLeftTap = false;
        RightSequence();
        break;
      }
      else{
        off();
        isLeftTap = false;
        isRightTap = false;
        LeftFlashCount = 0;
        RightFlashCount = 0;
        break;
      }
    }
    else if((digitalRead(LEFT_SIGNAL_INPUT) == 1) && (digitalRead(RIGHT_SIGNAL_INPUT) == 0)){
      LeftFlashCount = 0;
      isLeftTap = false;
      LeftOff();
      RightSequence();
      break;
      }
    else if((digitalRead(LEFT_SIGNAL_INPUT) == 0) && (digitalRead(RIGHT_SIGNAL_INPUT) == 1)){
      RightFlashCount = 0;
      isRightTap = false;
      RightOff();
      LeftSequence();
      break;
      }
    else if((digitalRead(LEFT_SIGNAL_INPUT) == 0) && (digitalRead(RIGHT_SIGNAL_INPUT) == 0)){
      LeftFlashCount = 0;
      RightFlashCount = 0;
      isLeftTap = false;
      isRightTap = false;
      HazardFlashingNoBrakes();
      break;
      }
    else{
      break;
      }      
  }
}