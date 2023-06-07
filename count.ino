#include <light_CD74HC4067.h>

CD74HC4067 mux(8, 9, 10, 11);
const int signal_pin = A1; // Pin A0 - Connected to Sig pin of CD74HC4067

int INS = 0;
int OUTS = 0;

int analogReading;

bool GATE_1_A;
bool GATE_1_B;

bool LAST_GATE_1_A;
bool LAST_GATE_1_B;

int COUNT = 0;
int LAST_COUNT = 0;

void setup() {
  Serial.begin(9600);
  pinMode(signal_pin, INPUT);
}

bool getSensorStatus(int analogReading) {
  return analogReading >= 1020 ? false : true;
}

void loop() {
  /**
   * GATE 1 - First sensor status
   */
  mux.channel(0);
  int GATE_1_A = getSensorStatus(analogRead(signal_pin));

  /**
   * GATE 1 - Second sensor status
   */
  mux.channel(1);
  int GATE_1_B = getSensorStatus(analogRead(signal_pin));

  // First sensor status changed and is active together with second sensor
  if ((LAST_GATE_1_A != GATE_1_A) && (GATE_1_A && GATE_1_B)) {
    OUTS++;
  }

  // Second sensor status changed and is active together with first sensor
  if ((LAST_GATE_1_B != GATE_1_B) && (GATE_1_B && GATE_1_A)) {
    INS++;
  }

  // Save sensor status for next loop iteration
  LAST_GATE_1_A = GATE_1_A;
  LAST_GATE_1_B = GATE_1_B;

  // Counted bees
  COUNT = INS + OUTS;

  // Short delay to give the bees some breating room
  delay(50);

  if (LAST_COUNT != COUNT) {
    Serial.print("In:  ");
    Serial.println(INS);
    Serial.print("Out:  ");
    Serial.println(OUTS);
    LAST_COUNT = COUNT;
  }
}
