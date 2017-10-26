
#include <iarduino_IR_RX.h>

#define IS_DEBUG 0

#define SUB_BUTTON 2
#define SUB_RELEY_1 3
#define SUB_IR 11
#define SUB_IR_ON 0xFF6897
#define SUB_IR_ON_T758 0xE13EA45B
#define SUB_IR_OFF 0xFF9867
#define SUB_IR_OFF_T758 0xE13E13EC
#define FUN_IR_UP 0xFF629D
#define FUN_IR_DOWN 0xFFA857
#define BUTTON_OK 0xFF02FD

#define FUN_PWM 5
#define FUN_ERROR 4
#define ERROR_SPEAKER 6
#define FUN_ERROR_POINTS 400

#define SUB_RUN_LOOP_DELAY 5
#define SUB_DELAY_POINT_ON 60
#define SUB_DELAY_POINT_OVERALL 200


bool _isOn;
bool _button;
bool _isOffButton;
int _counter = SUB_DELAY_POINT_ON;
int _counter_Fun_Error;

int _fun_pwm = 255;//0 - 255
bool _fun_Error;

iarduino_IR_RX IR(SUB_IR);

void setup() {
  
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(SUB_BUTTON, INPUT_PULLUP);
  pinMode(SUB_RELEY_1, OUTPUT);

  pinMode(FUN_PWM, OUTPUT);
  pinMode(ERROR_SPEAKER, OUTPUT);
  pinMode(FUN_ERROR, INPUT_PULLUP);

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

  if (_isOn) {
    checkPWMError();
  }

  _counter += _counter == SUB_DELAY_POINT_OVERALL? - SUB_DELAY_POINT_OVERALL + SUB_DELAY_POINT_ON : 1;

  if (_counter > 1000) {
    _counter = 0;
  }
  
  delay(SUB_RUN_LOOP_DELAY);
  
}

void checkIR() {
  
  if(IR.check()){
    if (IS_DEBUG) {
      Serial.println(IR.data, HEX);
      Serial.println(IR.length);
      if (IR.data == FUN_IR_UP) {
        setPWM(_fun_pwm + 10);
      }else if (IR.data == FUN_IR_DOWN) {
        setPWM(_fun_pwm - 10);
      }

      if (IR.data == BUTTON_OK) {
        setFunError(!_fun_Error);
      }
    }
    if (IR.data == SUB_IR_ON || IR.data == SUB_IR_ON_T758) {
      setOn(true);
    }else if (IR.data == SUB_IR_OFF || IR.data == SUB_IR_OFF_T758) {
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
    if (!_isOn) {
      setFunError(false);
    }
  }
  _counter = 0;
  
}

void setPWM(int fun_pwm) {
  
  if (_fun_pwm != fun_pwm) {
    _fun_pwm = fun_pwm;
    analogWrite(FUN_PWM, _fun_pwm);
    if (IS_DEBUG) {
      Serial.print("_fun_pwm: ");
      Serial.println(_fun_pwm);
    }
  }
  
}

void setFunError(bool fun_Error) {
  if (_fun_Error != fun_Error) {
     _fun_Error = fun_Error;
    if (IS_DEBUG) {
      Serial.print("fun_Error: ");
      Serial.println(fun_Error);
    } 
    if (_fun_Error) {
      analogWrite(ERROR_SPEAKER, 7);
    } else {
      analogWrite(ERROR_SPEAKER, 0);
    }
  }
}

void checkPWMError() {
  
  static int funRead = 0;
  funRead += !digitalRead(FUN_ERROR);
  if (_counter_Fun_Error == FUN_ERROR_POINTS * SUB_RUN_LOOP_DELAY) {

    setFunError(!funRead);
    
    _counter_Fun_Error = 0;
    funRead = 0;
  }

  _counter_Fun_Error += SUB_RUN_LOOP_DELAY;

}


