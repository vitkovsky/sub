
#include <iarduino_IR_RX.h>

#define IS_DEBUG 0

#define SUB_BUTTON 2
#define SUB_RELEY_1 3
#define SUB_RELEY_2 4
#define SUB_IR 12
#define SUB_IR_ON 0xFF6897
#define SUB_IR_OFF 0xFF9867

#define SUB_RUN_LOOP_DELAY 20
#define SUB_DELAY_POINT_ON 30
#define SUB_DELAY_POINT_OVERALL 100


bool _isOn;
bool _button;
bool _isOffButton;
int _counter = SUB_DELAY_POINT_ON;

iarduino_IR_RX IR(SUB_IR);

void setup() {
  
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(SUB_BUTTON, INPUT_PULLUP);
  pinMode(SUB_RELEY_1, OUTPUT);
  pinMode(SUB_RELEY_2, OUTPUT);

  if (IS_DEBUG) {
    Serial.begin(9600);
  }
  
  IR.begin();
  
}

// the loop function runs over and over again forever
void loop() {

  bool button = !digitalRead(SUB_BUTTON);

  if (_button && !button) {
    //On/Off
    setOn(!_isOn);
    _isOffButton = !_isOn;
  }
  _button = button;

  //if button click off, then on only button
  if (!_isOffButton) {
    checkIR();
  }

  _counter += _counter == SUB_DELAY_POINT_OVERALL? - SUB_DELAY_POINT_OVERALL + SUB_DELAY_POINT_ON : 1;
  
  delay(SUB_RUN_LOOP_DELAY);
  
}

void checkIR() {
  
  if(IR.check()){
    if (IS_DEBUG) {
      Serial.println(IR.data, HEX);
      Serial.println(IR.length);
    }
    if (IR.data == SUB_IR_ON) {
      setOn(true);
    }else if (IR.data == SUB_IR_OFF) {
      setOn(false);
    }
  }
  
}

void setOn(bool isOn) {
  
  if (isOn != _isOn && _counter >= SUB_DELAY_POINT_ON) {
    _isOn = isOn;
    if (IS_DEBUG) {
      Serial.print("_isOn: ");
      Serial.println(_isOn);
    }
    digitalWrite(LED_BUILTIN, _isOn);
    digitalWrite(SUB_RELEY_1, _isOn);
    digitalWrite(SUB_RELEY_2, _isOn);
  }
  _counter = 0;
  
}
