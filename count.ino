#include <light_CD74HC4067.h>

CD74HC4067 mux(8, 9, 10, 11);
const int signal_pin_mux_1 = A1;
const int signal_pin_mux_2 = A0;

int ins = 0;
int outs = 0;

int count = 0;
int last_count = 0;

int i = 0;
int channel;

byte last_a;
byte last_b;

byte gate_1_a_sensor;
byte gate_1_b_sensor;

byte gate_1_a_byte;
byte gate_1_b_byte;

void setup() {
  Serial.begin(9600);
  pinMode(signal_pin_mux_1, INPUT);
}

void loop() {
  gate_1_a_sensor = getSensorStatus(0);
  gate_1_b_sensor = getSensorStatus(1);

  last_a = gate_1_a_byte&0x1;
  last_b = gate_1_b_byte&0x1;
  int validated_state = abs(last_a - gate_1_a_sensor) + abs(last_b - gate_1_b_sensor);

  if (validated_state == 1) {
    gate_1_a_byte = gate_1_a_byte<<1|gate_1_a_sensor;
    gate_1_b_byte = gate_1_b_byte<<1|gate_1_b_sensor;

    int a = gate_1_a_byte&0x1f;
    int b = gate_1_b_byte&0x1f;
    if (a == 6 && b == 12) { outs++; }
    if (b == 6 && a == 12) { ins++; }

    count = ins + outs;

    if (last_count != count) {
      Serial.print("In:  ");
      Serial.println(ins);
      Serial.print("Out:  ");
      Serial.println(outs);
      last_count = count;
    }
  }
}

byte getSensorStatus(int channel) {
  mux.channel(channel);
  delay(30);
  return analogRead(signal_pin_mux_1) >= 1020 ? 0 : 1;
}