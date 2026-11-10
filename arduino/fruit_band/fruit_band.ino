/*
  Fruit Band - Arduino side
  Reads 5 fruits with capacitive touch and sends the values to the laptop.

  Library: CapacitiveSensor by Paul Badger
  (Arduino IDE -> Tools -> Manage Libraries -> search "CapacitiveSensor" -> Install)

  WIRING (one Arduino Uno for all 5 fruits)
  ------------------------------------------
  Pin 2 = SEND pin, shared by all fruits.

  For each fruit:
    Pin 2 ---[ 1M ohm resistor ]--- receive pin ---> alligator clip ---> fruit

    Tomato       -> receive pin 4
    Pomegranate  -> receive pin 5
    Persimmon    -> receive pin 6
    Pear         -> receive pin 7
    Lemon        -> receive pin 8

  Tip: push a short piece of wire (or a paper clip) into the fruit,
  then clip the alligator clip onto that wire. It holds better than
  clipping the skin.

  OUTPUT
  ------
  One line about 30 times per second, 5 numbers separated by commas:
    tomato,pomegranate,persimmon,pear,lemon
  Example: 112,95,130,88,101
  Open Tools -> Serial Monitor (9600 baud) to check the numbers.
  The number should jump up a lot when you touch a fruit.
*/

#include <CapacitiveSensor.h>

const int SEND_PIN = 2;
const int NUM_FRUITS = 5;

// Higher = more stable but slower. 30 is a good start.
const int SAMPLES = 30;

CapacitiveSensor tomato(SEND_PIN, 4);
CapacitiveSensor pomegranate(SEND_PIN, 5);
CapacitiveSensor persimmon(SEND_PIN, 6);
CapacitiveSensor pear(SEND_PIN, 7);
CapacitiveSensor lemon(SEND_PIN, 8);

CapacitiveSensor* fruits[NUM_FRUITS] = { &tomato, &pomegranate, &persimmon, &pear, &lemon };

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < NUM_FRUITS; i++) {
    // Turn off auto-calibration. The laptop program does the calibration.
    fruits[i]->set_CS_AutocaL_Millis(0xFFFFFFFF);
  }
}

void loop() {
  for (int i = 0; i < NUM_FRUITS; i++) {
    long value = fruits[i]->capacitiveSensor(SAMPLES);
    if (value < 0) value = 0;   // -2 means a timeout (check the wiring)
    Serial.print(value);
    if (i < NUM_FRUITS - 1) Serial.print(',');
  }
  Serial.println();
  delay(20);
}
