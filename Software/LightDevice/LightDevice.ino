// use preprocessor directives to define constants in the code
#define BUTTON_IN 2
#define LED_OUT 6
#define PWM_MAX 255

// global variables representing "state" across all functions
// operating mode: 0 - off, 1 - bright, 2 - mid-bright, 3 - dim, 4 - flash
int operating_mode = 0;
int button_state = 0;
int previous_button_state = 0;
int PWM_OUT = 0;

unsigned long PrevTime = 0;
unsigned long lag_time = 0;
int lag_delay = 250;

bool BUTTON_PUSHED = false;

// executed one-time at device startup
void setup() {
   attachInterrupt(digitalPinToInterrupt(BUTTON_IN), button_pushed, FALLING);
 // define output (PWM) pin connected to LED
  pinMode(LED_OUT, OUTPUT);
}

// continually-running loop
// calls functions that are named as "actions"
void loop() {
  check_button_press();

  set_pwm_based_on_operating_mode();

  shine_led();

}

void set_pwm_based_on_operating_mode() {

  switch (operating_mode) {
    case 0:
      PWM_OUT = 0;
      break;
    case 1:
      PWM_OUT = PWM_MAX;
      break;
    case 2:
      PWM_OUT = int(PWM_MAX / 2);
      break;
    case 3:
      PWM_OUT = int(PWM_MAX / 4);
      break;
    case 4:
    flash_the_light();
      break;
  }

}

void button_pushed() {
  BUTTON_PUSHED = true;
}

void flash_the_light() {
  unsigned long CurrentTime = millis();
  if(CurrentTime - PrevTime >= 500){
    PrevTime = CurrentTime;
    if (PWM_OUT == 0){
      PWM_OUT=PWM_MAX;
    }else{
      PWM_OUT=0;
    }
  }
}

void shine_led() {
  analogWrite(LED_OUT, PWM_OUT);
}

void check_button_press() {
  if (BUTTON_PUSHED == true) {
    if (millis() - lag_time > lag_delay) {
      operating_mode = operating_mode + 1;
      if (operating_mode == 5) {
        operating_mode = 0;
      }
      lag_time = millis();
    }
  }
  BUTTON_PUSHED = false;
}
