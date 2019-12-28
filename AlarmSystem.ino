int switchState = 0;
int movement = 0;

const int laser = 2;

const int butt1 = 3;
int buttState1 = 0;
const int butt2 = 4;
int buttState2 = 0;
const int butt3 = 5;
int buttState3 = 0;
const int butt4 = 6;
int buttState4 = 0;

const int pir = 10;
const int speaker = 11;
const int reds = 12;
const int green = 13;
const int prevSwitchState = 0;

boolean excuse = false;

String code = "123"; // starting password
String attempt = ""; // user-inputted password

void setup() {
  Serial.begin(9600);
  pinMode(laser, INPUT);
  pinMode(speaker, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(reds, OUTPUT);
  pinMode(pir, INPUT); 
  pinMode(butt1, INPUT);
  pinMode(butt2, INPUT);
  pinMode(butt3, INPUT);
  pinMode(butt4, INPUT);
} // setup()

void loop() {
  // put your main code here, to run repeatedly:
  switchState = digitalRead(laser);
  digitalWrite(green, HIGH);
  int button = 0;
  movement = digitalRead(pir);

  // If password hasn't been entered and movement is detected...
  // enter while loop
  if((switchState == prevSwitchState || movement != prevSwitchState) && (excuse == false)) {
     // while the user-inputted password is not equal to the actual
     // password, sound alarm and record buttons pressed
     while(code != attempt) {
        alert();
        button = buttonPress();
        if(button != 0) {
          releaseButtons();
          attempt += button;
        } // if()
        if(button == 4) {
          attempt = "";
          releaseButtons();
        } // if()
    } // while()
    
    // alarm will not go off again, unless reset
    excuse = true;
  } // if()
  button = buttonPress();
  releaseButtons();

  // Following if statement resets alarm if fourth button
  // is pressed
  if(excuse == true && button == 4) {
    digitalWrite(green, LOW);
    excuse = false;
    attempt = "";
    switchState = 1;
    movement = 0;
    delay(5000);
  } // if()

  // if button one is pressed, then all of the following button
  // presses will be recorded as new password
  else if(excuse == true && button == 1) {
    code = "";
    code = newPassword(button);

    // the following while loop makes it so that the user MUST
    // enter a password of at least 1 character
    while(code == ""){
      code = newPassword(button);
    } // while()
  } // else if()
} // loop()

// alert() controls alarm sounds and light flashes
void alert() {
  digitalWrite(green, LOW);
  digitalWrite(reds, HIGH);
  
  // Whoop up
  for(int hz = 440; hz < 1000; hz+=1.5) {
    tone(speaker, hz, 50);
    if(hz > 725) {
      digitalWrite(reds, LOW);
    } // if
  } // for()
  
  digitalWrite(reds, HIGH);
  noTone(3);
  digitalWrite(reds, LOW);
} // alert()

// buttonPress() returns the integer value of the button that was
// pressed. Returns a zero if no button is pressed
int buttonPress() {
  buttState1 = digitalRead(butt1);
  buttState2 = digitalRead(butt2);
  buttState3 = digitalRead(butt3);
  buttState4 = digitalRead(butt4);
  int pressed = 0;
  if(buttState1 == 1 || buttState2 == 1 || buttState3 == 1 || buttState4 == 1) {
    tone(speaker, 600, 50);
    if(buttState1 == 1) {
      releaseButtons();
      return 1;
    } // if()
    else if(buttState2 == 1) {
      releaseButtons();
      return 2;
    } // else if()
    else if(buttState3 == 1) {
      releaseButtons();
      return 3;
    } // else if()
    else if(buttState4 == 1) {
      releaseButtons();
      return 4;
    } // else if()
  } // if()
  return pressed;
}

// releaseButtons() waits for all buttons to be released before
// program execution can continue
void releaseButtons() {
  // Makes sure the program only reads one action when button 
  // is pressed. Makes sure button can't be held down
  while(buttState1 == 1 || buttState2 == 1 || buttState3 == 1 || buttState4 == 1){
        buttState1 = digitalRead(butt1);
        buttState2 = digitalRead(butt2);
        buttState3 = digitalRead(butt3);
        buttState4 = digitalRead(butt4);
  } // while()
} // releaseButtons()

// newPassword() allows for the user to change the password
// to any combination of the first three button presses
String newPassword(int button) {
  digitalWrite(reds, HIGH);
  Serial.print(code);
  code = "";

  // press fourth button to complete password
  while(button != 4) {
    button = buttonPress();

    // record only presses of buttons one, two, and three
    if (button != 0 && button != 4) {
      code += button;
    } // if()
  } // while()
  releaseButtons();
  digitalWrite(reds, LOW);
  return code;
} // newPassword()
