int analogButton;
int readVal;
int i, j, k;

const int leftHeadPin = 52; 
const int leftFootPin = 44;
const int rightHeadPin = 36;
const int rightFootPin = 28;

int leftHeadState = 0;
int leftFootState = 0;
int rightHeadState = 0;
int rightFootState = 0;

enum PANEL_TYPE {
  BOTTOM_LEFT_OUTER,
  BOTTOM_LEFT_MIDDLE,
  TOP_LEFT_FRONT,
  TOP_LEFT_BACK,
  BOTTOM_RIGHT_OUTER,
  BOTTOM_RIGHT_MIDDLE,
  TOP_RIGHT_FRONT,
  TOP_RIGHT_BACK
};

const PANEL_TYPE PANELS_ORDERED [] = {  BOTTOM_LEFT_OUTER,
                                        TOP_LEFT_FRONT,
                                        TOP_LEFT_BACK,
                                        TOP_RIGHT_FRONT,
                                        TOP_RIGHT_BACK,
                                        BOTTOM_RIGHT_OUTER,
                                        BOTTOM_RIGHT_MIDDLE,
                                        BOTTOM_LEFT_MIDDLE };
PANEL_TYPE panel;

const String BUTTON_NAMES [] = {  "LF0", // Foot Left
                                  "RF0", // Foot Right
                                  "LH0", // Head Left
                                  "RH0", // Head Right
                                  // Left Top Front
                                  "LF6", "LF4", "LF2", "LF1", "LF3", "LF5", 
                                  // Left Top Back
                                  "LB5", "LB3", "LB1", "LB2", "LB4", "LB6",
                                  // Right Top Front
                                  "RF5", "RF3", "RF1", "RF2", "RF4", "RF6",
                                  // Right Top Back
                                  "RB6", "RB4", "RB2", "RB1", "RB3", "RB5",
                                  // Left Bottom Outer
                                  "BL2", "BL4", "BL3", "BL1",
                                  // Left Bottom Middle
                                  "LM2", "LM4", "LM3", "LM1",
                                  // Right Bottom Middle
                                  "RM1", "RM3", "RM4", "RM2",
                                  // Right Bottom Outer
                                  "BR1", "BR3", "BR4", "BR2"
                                };

const int analogThresh [6] = {75, 150, 225, 300, 365, 420}; //All button thresholds 

const int NUM_PANELS = 8;

//char output [6] = {'A', 'B', 'C', 'D', 'E', 'F'};

//String serialOutput [7] = {"X", "X", "X", "X", "X", "X", "X"};
String serialOutput;
String state;

int panelNum = 0;

int panelStartIndex = 0;

/*____ARDUINO____*/

void setup() {
  Serial.begin(9600);

  //Pull all of the analog input pins high, see circuit diagram for more detail
  digitalWrite(A1, HIGH);
  digitalWrite(A2, HIGH);
  digitalWrite(A3, HIGH);
  digitalWrite(A4, HIGH);
  digitalWrite(A5, HIGH);
  digitalWrite(A6, HIGH);
  digitalWrite(A7, HIGH);
  digitalWrite(A8, HIGH);

  pinMode(leftHeadPin, INPUT);
  pinMode(leftFootPin, INPUT);
  pinMode(rightHeadPin, INPUT);
  pinMode(rightFootPin, INPUT);

  serialOutput = "";

}//end setup

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
      case TOP_LEFT_FRONT:
        panelStartIndex = 4;
        break;

      case TOP_LEFT_BACK:
        panelStartIndex = 10;
        break;
        
      case TOP_RIGHT_FRONT:
        panelStartIndex = 16;
        break;
        
      case TOP_RIGHT_BACK:
        panelStartIndex = 22;
        break;

      case BOTTOM_LEFT_OUTER:
        panelStartIndex = 28;
        break;

      case BOTTOM_LEFT_MIDDLE:
        panelStartIndex = 32;
        break;

      case BOTTOM_RIGHT_MIDDLE:
        panelStartIndex = 36;
        break;

      case BOTTOM_RIGHT_OUTER:
        panelStartIndex = 40;
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
  } // end for
}

void PrintToSerial () 
{
  Serial.println("ARDUINO" + serialOutput);
  serialOutput = "";
}
