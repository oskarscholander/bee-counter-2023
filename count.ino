#include <light_CD74HC4067.h>

CD74HC4067 mux(8, 9, 10, 11);

const int signal_pin_mux_1 = A1;
const int signal_pin_mux_2 = A0;

int ins = 0; // Total amount of bees entered
int outs = 0; // Total amount of bees exited

int count = 0;
int last_count = 0;

byte last_a[22]; // Sensor history - the value before
byte last_b[22]; // Sensor history - the value before

int gates[] = { 0, 2, 4, 6, 8, 10 }; // First sensor channel of each gate
int number_of_gates = 6; // Hard coded amount of gates on the circuit board

byte gate_a_sensor[21]; // Current status of first sensor of gate
byte gate_b_sensor[21]; // Current status of second sensor gate

byte gate_a_byte[200]; // In and out data being stored from first gate sensor
byte gate_b_byte[200]; // In and out data being stored from second gate sensor

int gate_a; // Reference MUX1 gate number for gates byte array
int gate_b; // Reference MUX1 gate number for gates byte array

int gate_aa; // Reference MUX2 gate number for gates byte array
int gate_bb; // Reference MUX2 gate number for gates byte array

int gate_a_channel; // Current first gate sensor being targeted in the loop iteration
int gate_b_channel; // Current second gate sensor being targeted in the loop iteration

int x;

void setup() {
  x = 1;
  Serial.begin(9600);
  pinMode(signal_pin_mux_1, INPUT);
  pinMode(signal_pin_mux_2, INPUT);
}

void loop() {
  for (int i = 0; i < number_of_gates; i++) {
    gate_a = gates[i];
    gate_b = gates[i];

    gate_aa = gate_a + number_of_gates;
    gate_bb = gate_b + number_of_gates;

    gate_a_channel = gate_a;
    gate_b_channel = gate_a_channel + 1;

    // Read first sensor of gate on MUX1
    mux.channel(gate_a_channel);
    delay(1);
    gate_a_sensor[gate_a] = analogRead(signal_pin_mux_1) >= 1020 ? 0 : 1;

    // Read first sensor of gate on MUX2
    if (i < 6) {
      delay(1);
      gate_a_sensor[gate_aa] = analogRead(signal_pin_mux_2) >= 1020 ? 0 : 1;
    }

    // Read second sensor of gate on MUX1
    mux.channel(gate_b_channel);
    delay(1);
    gate_b_sensor[gate_b] = analogRead(signal_pin_mux_1) >= 1020 ? 0 : 1;

    // Read second sensor of gate on MUX2
    if (i < 6) {
      delay(1);
      gate_b_sensor[gate_bb] = analogRead(signal_pin_mux_2) >= 1020 ? 0 : 1;
    }

    last_a[gate_a] = gate_a_byte[gate_a]&0x1;
    last_b[gate_b] = gate_b_byte[gate_b]&0x1;

    last_a[gate_aa] = gate_a_byte[gate_aa]&0x1;
    last_b[gate_bb] = gate_b_byte[gate_bb]&0x1;

    for (int x = 0; x < 2; x++) {
      int y = gate_a;
      int z = gate_b;

      if (x == 1) {
        int y = gate_aa;
        int z = gate_bb;
      }

      if (abs(last_a[y] - gate_a_sensor[y]) + abs(last_b[z] - gate_b_sensor[z]) == 1) {
        gate_a_byte[y] = gate_a_byte[y]<<1|gate_a_sensor[y];
        gate_b_byte[z] = gate_b_byte[z]<<1|gate_b_sensor[z];

        int a = gate_a_byte[y]&0x1f;
        int b = gate_b_byte[z]&0x1f;

        if (a == 6 && b == 12) { outs++; }
        if (b == 6 && a == 12) { ins++; }

        count = ins + outs;

        if (last_count != count) {
          printAmount(ins, outs);
          last_count = count;
        }
      }
    }
  }
}

void printAmount(int ins, int outs) {
    Serial.print("IN:  ");
    Serial.print(ins);
    Serial.print(" - UT:  ");
    Serial.println(outs);
}