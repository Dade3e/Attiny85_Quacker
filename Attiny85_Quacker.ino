// ATMEL ATTINY 25/45/85 / ARDUINO
//
//                  +-\/-+
// Ain0 (D 5) PB5  1|    |8  Vcc
// Ain3 (D 3) PB3  2|    |7  PB2 (D 2) Ain1
// Ain2 (D 4) PB4  3|    |6  PB1 (D 1) pwm1
//            GND  4|    |5  PB0 (D 0) pwm0
//                  +----+


#include <avr/sleep.h> //Needed for sleep_mode
#include <avr/power.h>    // Power management
#include "avr_sound.h"
#include "Quack_Sound_Effect.h"

// ATtiny Pin 5 = SDA                   ATtiny Pin 6 = (D1) to LED2
// ATtiny Pin 7 = SCK                    ATtiny Pin 8 = VCC (2.7-5.5V)


const byte SWITCH = 4; // pin 3 / PCINT4

void setup() {
  //analogReference(INTERNAL);      // use precise internal reference
  //ADCSRA &= ~(1<<ADEN); //Disable ADC, saves ~230uA

  pinMode (SWITCH, INPUT);
  digitalWrite (SWITCH, HIGH);  // internal pull-up

  pinMode (0, OUTPUT);

  PCMSK  |= bit (PCINT4);  // want pin D4 / pin 3
  GIFR   |= bit (PCIF);    // clear any outstanding interrupts
  GIMSK  |= bit (PCIE);    // enable pin change interrupts
  avr_sound_init();
}

//This runs each time the watch dog wakes us up from sleep
ISR(PCINT0_vect) {
  //watchdog_counter++;
}

void loop() {

  play_Quack_Sound_Effect();

  goToSleep ();
}

void goToSleep ()
  {
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  ADCSRA = 0;            // turn off ADC
  power_all_disable ();  // power off ADC, Timer 0 and 1, serial interface
  sleep_enable();
  sleep_cpu();                             
  sleep_disable();   
  power_all_enable();    // power everything back on
  //pinMode (1, OUTPUT);
}  // end of goToSleep

