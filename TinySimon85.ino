/* Tiny Simon - with ATTiny85

    Based on: #1 Simon (Genius) game -by Hasbro
              #2 GameBone Handheld Electronic Game -by David Johnson-Davies
              #3 Simon game with ATtiny13 -by Vojtak

    Simon game developed on ATTiny85 microcontroller to Arduino Day SP 2019.

    -----------------------------------------------------

    Mini Genius - com ATTiny85

    Inspirado em: #1 Simon (Genius) game -by Hasbro
                  #2 GameBone Handheld Electronic Game -by David Johnson-Davies
                  #3 Simon game with ATtiny13 -by Vojtak

    Jogo Genius desenvolvido no microcontrolador ATTiny85 para o Arduino Day SP 2019.

            ATMEL ATTINY85

        Reset  1| \/ |8  5Vcc
    Ain3  PB3  2|    |7  PB2  Ain1
    Ain2  PB4  3|    |6  PB1  pwm1
          GND  4|    |5  PB0  pwm0


    by Bruno Silveira
    Mar/2019
*/

#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/interrupt.h>

const int beat = 250;
const int maximum = 32;

// Buttons:         Blue  Yellow   Green  Red
int pins[] = { 2,     0,     3,     4};

// Notes:             E4  C#4 A4  E3
int notes[] = { 52, 49, 57, 40 };
//int buzzer = 2;

int sequence[maximum];

// Simple Tones **********************************************

const uint8_t scale[] PROGMEM = {239, 226, 213, 201, 190, 179, 169, 160, 151, 142, 134, 127};

void note (int n, int octave) {
  DDRB = DDRB | 1<<DDB1;                     // PB1 (Arduino 1) as output
  TCCR1 = 0x90 | (8-octave); // for 1MHz clock
  // TCCR1 = 0x90 | (11-octave); // for 8MHz clock
  OCR1C = pgm_read_byte(&scale[n % 12]) - 1;
  delay(duration);
  TCCR1 = 0x90;              // stop the counter
}

// Button routines **********************************************

// Pin change interrupt wakes us up
ISR (PCINT0_vect) {
}

// Turn on specified button
void button_on (int button) {
  int p = pins[button];
  pinMode(p, OUTPUT);
  digitalWrite(p, LOW);
}

// Turn off specified button
void button_off (int button) {
  int p = pins[button];
  pinMode(p, INPUT_PULLUP);
}

void allon() {
  for (int b = 0; b < 4; b++) {
    button_on(b);
  }
}

void alloff() {
  for (int b = 0; b < 4; b++) {
    button_off(b);
  }
}

// Flash an LED and play the corresponding note of the correct frequency
void flashbeep (int button) {
  button_on(button);
  note(notes[button], 0);
  delay(beat);
  note(0, 0);
  button_off(button);
}


// Wait until a button is pressed and play it
int check () {
  GIMSK = GIMSK | 1 << PCIE;          // Enable pin change interrupt
  sleep();
  GIMSK = GIMSK & ~(1 << PCIE);       // Disable pin change interrupt
  int button = 0;
  do {
    button = (button + 1) % 4;
  } while (digitalRead(pins[button]));
  flashbeep(button);
  return button;
}

void success_sound () {
  note(48, 0); delay(125);
  note(0, 0);  delay(125);
  note(52, 0); delay(125);
  note(0, 0);  delay(125);
  note(55, 0); delay(125);
  note(0, 0);  delay(125);
  note(60, 0); delay(375);
  note(0, 0);
}

void fail_sound () {
  allon();
  note(51, 0); delay(125);
  alloff();
  note(0, 0);  delay(125);

  allon();
  note(48, 0); delay(375);
  alloff();
  note(0, 0);
}

// Simon **********************************************

void simon () {
  int turn = 0;
  sequence[0] = random(4);
  do {
    for (int n = 0; n < = turn; n++) {
      delay(beat);
      flashbeep(sequence[n]);
    }
    for (int n = 0; n <= turn; n++) {
      if (check() != sequence[n]) {
        fail_sound();
        return;
      }
    }
    sequence[turn + 1] = (sequence[turn] + random(0, 2) + 1) % 4;
    turn++;
    delay(beat);
  } while (turn < maximum);
  success_sound();
}

// Echo **********************************************

void echo () {
  int turn = 0;
  sequence[turn] = check();
  do {
    for (int n = 0; n <= turn; n++) {
      if (check() != sequence[n]) {
        fail_sound();
        return;
      }
    }
    sequence[turn + 1] = check();
    turn++;
    delay(beat);
  } while (turn < maximum);
  success_sound();
}

// Quiz **********************************************

void quiz () {
  do {
    int button = check();
    button_on(button);
    delay(3000);
    button_off(button);
  } while (1);
}


// Setup and loop **********************************************

void sleep()
{
  sleep_enable();
  sleep_cpu();
}

void setup() {

  // Set up pin change interrupts for buttons
  PCMSK = 1 << PINB2 | 1 << PINB0 | 1 << PINB3 | 1 << PINB4;

  // Disable ADC - saves about 324.5uA in sleep mode!
  ADCSRA = 0;
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);

  // After reset flash all four lights
  for (int b = 0; b < 4; b++) {
    button_on(b);
    delay(100);
    button_off(b);
  }

  // Wait for button to select game
  int game = 0;
  do {
    game = (game + 1) % 4;
    if (millis() > 5000) sleep();
  } while (digitalRead(pins[game]));

  randomSeed(millis());
  delay(250);

  switch (game) {
    case 0:     //blue
      simon();
      break;
    case 1:     //yellow
      quiz();
      break;
    case 2:   //green
      echo();
      break;
  }
}

// Only reset should wake us now
void loop() {
  sleep();
}
