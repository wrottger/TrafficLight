#include <BlockNot.h>

#define T1_RED 2
#define T1_YELLOW 3
#define T1_GREEN 4

#define T2_RED 5
#define T2_YELLOW 6
#define T2_GREEN 7

#define GREEN_PHASE 2000
#define YELLOW_PHASE 1000
#define OVERLAP 200

unsigned long time_now = 0;
int active_light = 1;

BlockNot greenTimer(GREEN_PHASE);
BlockNot yellowTimer1(YELLOW_PHASE);
BlockNot yellowTimer2(YELLOW_PHASE);
BlockNot phaseTimer(GREEN_PHASE + YELLOW_PHASE * 2 + OVERLAP);


void setup() {
  pinMode(T1_RED, OUTPUT);
  pinMode(T1_YELLOW, OUTPUT);
  pinMode(T1_GREEN, OUTPUT);


  pinMode(T2_RED, OUTPUT);
  pinMode(T2_YELLOW, OUTPUT);
  pinMode(T2_GREEN, OUTPUT);

  digitalWrite(T1_GREEN, HIGH);
  digitalWrite(T2_GREEN, HIGH);
  digitalWrite(T1_YELLOW, LOW);
  digitalWrite(T2_YELLOW, HIGH);
  digitalWrite(T1_RED, HIGH);
  digitalWrite(T2_RED, HIGH);

  phaseTimer.START_RESET;
  yellowTimer1.START_RESET;
  yellowTimer2.STOP;
  greenTimer.STOP;
  
  Serial.begin(9600);
  while (!Serial)
    ;
}

void allRed() {
  digitalWrite(T1_GREEN, HIGH);
  digitalWrite(T2_GREEN, HIGH);

  digitalWrite(T1_YELLOW, HIGH);
  digitalWrite(T2_YELLOW, HIGH);

  digitalWrite(T1_RED, LOW);
  digitalWrite(T2_RED, LOW);
}


void loop() {
  if (Serial.read() == '1') {
    digitalWrite(LED_BUILTIN, HIGH);
    allRed();
    while (Serial.read() != '0')
      ;
    active_light = 1;
    phaseTimer.START_RESET;
    yellowTimer1.START_RESET;
    yellowTimer2.STOP;
    greenTimer.STOP;
    digitalWrite(T1_YELLOW, LOW);
    digitalWrite(T1_RED, HIGH);

  }

  // YELLOW_1 OVER,  STARTING GREEN
  if (yellowTimer1.TRIGGERED) {
    yellowTimer1.STOP;
    if (active_light == 1) {
      digitalWrite(T1_YELLOW, HIGH);
      digitalWrite(T1_GREEN, LOW);
    } else {
      digitalWrite(T2_YELLOW, HIGH);
      digitalWrite(T2_GREEN, LOW);
    }
    greenTimer.START_RESET;
  }

  // GREEN OVER, STARTING YELLOW_2
  if (greenTimer.TRIGGERED) {
    greenTimer.STOP;
    if (active_light == 1) {
      digitalWrite(T1_GREEN, HIGH);
      digitalWrite(T1_YELLOW, LOW);
    } else {
      digitalWrite(T2_GREEN, HIGH);
      digitalWrite(T2_YELLOW, LOW);
    }
    yellowTimer2.START_RESET;
  }

  // YELLOW_2 OVER, STARTING RED
  if (yellowTimer2.TRIGGERED) {
    yellowTimer2.STOP;
    if (active_light == 1) {
      digitalWrite(T1_RED, LOW);
      digitalWrite(T1_YELLOW, HIGH);
    } else {
      digitalWrite(T2_RED, LOW);
      digitalWrite(T2_YELLOW, HIGH);
    }
 }

  // CHANGING TRAFFIC LIGHT
  if (phaseTimer.TRIGGERED) {
    yellowTimer1.START_RESET;
    if (active_light == 1) {
      active_light = 2;
    } else {
      active_light = 1;
    }
    if (active_light == 1) {
      digitalWrite(T1_RED, HIGH);
      digitalWrite(T1_YELLOW, LOW);
    } else {
      digitalWrite(T2_RED, HIGH);
      digitalWrite(T2_YELLOW, LOW);
    }
  }
}
