int analogButton;
int readVal;
int i, j, k;

const int leftHeadPin = 36; 
const int leftFootPin = 38;
const int rightHeadPin = 40;
const int rightFootPin = 42;

int leftHeadState = 0;
int leftFootState = 0;
int rightHeadState = 0;
int rightFootState = 0;

enum PANEL_TYPE {
  BOTTOM_LEFT_OUTER,
  BOTTOM_LEFT_MIDDLE,
  BOTTOM_LEFT_INNER,
  TOP_LEFT_INNER,
  TOP_LEFT_OUTER,
  BOTTOM_RIGHT_OUTER,
  BOTTOM_RIGHT_MIDDLE,
  BOTTOM_RIGHT_INNER,
  TOP_RIGHT_INNER,
  TOP_RIGHT_OUTER
};

const PANEL_TYPE PANELS_ORDERED [] = {  TOP_LEFT_OUTER,
                                        TOP_LEFT_INNER,
                                        TOP_RIGHT_INNER,
                                        TOP_RIGHT_OUTER,
                                        BOTTOM_RIGHT_OUTER,
                                        BOTTOM_RIGHT_MIDDLE,
                                        BOTTOM_RIGHT_INNER,
                                        BOTTOM_LEFT_INNER,
                                        BOTTOM_LEFT_MIDDLE,
                                        BOTTOM_LEFT_OUTER };
PANEL_TYPE panel;

// See wiring diagram for details:
const String BUTTON_NAMES [] = {  "LF0", // Foot Left
                                  "RF0", // Foot Right
                                  "LH0", // Head Left
                                  "RH0", // Head Right
                                  // [4] Left Top Inner
                                  "LF6", "LF4", "LF2", "LF1", "LF3", "LF5", 
                                  // [10] Left Top Outer
                                  "LB6", "LB4", "LB2", "LB1", "LB3", "LB5",
                                  // [16] Right Top Inner
                                  "RF5", "RF3", "RF1", "RF2", "RF4", "RF6",
                                  // [22] Right Top Outer
                                  "RB5", "RB3", "RB1", "RB2", "RB4", "RB6",
                                  // [28] Left Bottom Outer
                                  "BL6", "BL4", "BL2", "BL1", "BL3", "BL5",
                                  // [34] Left Bottom Middle
                                  "LM6", "LM4", "LM2", "LM1", "LM3", "LM5",
                                  // [40] Left Bottom Inner
                                  "LI6", "LI4", "LI2", "LI1", "LI3", "LI5",
                                  // [46] Right Bottom Inner
                                  "RI6", "RI4", "RI2", "RI1", "RI3", "RI5",
                                  // [52] Right Bottom Middle
                                  "RM5", "RM3", "RM1", "RM2", "RM4", "RM6",
                                  // [58] Right Bottom Outer
                                  "BR5", "BR3", "BR1", "BR2", "BR4", "BR6"
                                };

const int analogThresh [6] = { 75, 150, 240, 310, 370, 425 }; //All button thresholds 

const int NUM_PANELS = 10;

String serialOutput;
String state;

int panelNum = 0;

int panelStartIndex = 0;

/*____CODE BEGIN____*/
void setup() {
  // baudrate
  Serial.begin(9600);

  //Pull all of the analog input pins high, see circuit diagram for details
  digitalWrite(A1, HIGH);
  digitalWrite(A2, HIGH);
  digitalWrite(A3, HIGH);
  digitalWrite(A4, HIGH);
  digitalWrite(A5, HIGH);
  digitalWrite(A6, HIGH);
  digitalWrite(A7, HIGH);
  digitalWrite(A8, HIGH);
  // A9 and A10 for v2 only
  digitalWrite(A9, HIGH);
  digitalWrite(A10, HIGH);

  // set pin modes
  pinMode(leftHeadPin, INPUT);
  pinMode(leftFootPin, INPUT);
  pinMode(rightHeadPin, INPUT);
  pinMode(rightFootPin, INPUT);

  serialOutput = "";

} // end setup

void loop() {
  ReadHeadFootState ();
  ReadPanelStates ();
  PrintToSerial ();
} //end loop


/*____FUNCTIONS____*/

void ReadHeadFootState () 
{
  leftHeadState = digitalRead(leftHeadPin);
  leftFootState = digitalRead(leftFootPin);
  rightHeadState = digitalRead(rightHeadPin);
  rightFootState = digitalRead(rightFootPin);
  state = "";
  
  if (leftHeadState == HIGH){
    serialOutput += BUTTON_NAMES[2];
  }

  if (leftFootState == HIGH){
    serialOutput += BUTTON_NAMES[0];
  }

  if (rightHeadState == HIGH){
    serialOutput += BUTTON_NAMES[3];
  }

  if (rightFootState == HIGH){
    serialOutput += BUTTON_NAMES[1];
  }
}


void ReadPanelStates () 
{
  for (i=0; i < NUM_PANELS; ++i) {
    panelNum = 0;
    panel = PANELS_ORDERED[i];
    analogButton = i + 1;

    switch (panel){
      case TOP_LEFT_INNER:
        panelStartIndex = 4;
        break;

      case TOP_LEFT_OUTER:
        panelStartIndex = 10;
        break;
        
      case TOP_RIGHT_INNER:
        panelStartIndex = 16;
        break;
        
      case TOP_RIGHT_OUTER:
        panelStartIndex = 22;
        break;

      case BOTTOM_LEFT_OUTER:
        panelStartIndex = 28;
        break;

      case BOTTOM_LEFT_MIDDLE:
        panelStartIndex = 34;
        break;

      case BOTTOM_LEFT_INNER:
        panelStartIndex = 40;
        break;

      case BOTTOM_RIGHT_INNER:
        panelStartIndex = 46;
        break;

      case BOTTOM_RIGHT_MIDDLE:
        panelStartIndex = 52;
        break;

      case BOTTOM_RIGHT_OUTER:
        panelStartIndex = 58;
        break;

      default:
        serialOutput += "ERROR";
        delay (10);
        return;
    } // End of switch

    readVal = analogRead (analogButton);

    if (readVal < analogThresh[0]) {
      serialOutput += BUTTON_NAMES[panelStartIndex + 0];
    }//end if
    
    else if (readVal < analogThresh[1]) {
      serialOutput += BUTTON_NAMES[panelStartIndex + 1];
    }//end else if
    
    else if (readVal < analogThresh[2]) {
      serialOutput += BUTTON_NAMES[panelStartIndex + 2];  
    }//end else if
  
    else if (readVal < analogThresh[3]) {
      serialOutput += BUTTON_NAMES[panelStartIndex + 3];
    }//end else if

    else if (readVal < analogThresh[4]) {
      serialOutput += BUTTON_NAMES[panelStartIndex + 4];
    }//end else if

    else if (readVal < analogThresh[5]) {
      serialOutput += BUTTON_NAMES[panelStartIndex + 5]; 
    }//end else if

    else {
      // nothing
    }//end else 

  delay (10);
  }//end for
}

void PrintToSerial () 
{
  Serial.println("ARDUINO" + serialOutput);
  serialOutput = "";
}
