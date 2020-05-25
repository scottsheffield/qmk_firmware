#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/power.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/eeprom.h>
#include "split_util.h"
#include "matrix.h"
#include "keyboard.h"
#include "config.h"

#ifdef USE_MATRIX_I2C
#  include "i2c.h"
#else
#  include "serial.h"
#endif

volatile bool isLeftHand = true;

static void setup_handedness(void) {
  #ifdef EE_HANDS
    isLeftHand = eeprom_read_byte(EECONFIG_HANDEDNESS);
  #else
    // I2C_LEADER_RIGHT is deprecated, use LEADER_RIGHT instead, since this works for both serial and i2c
    #if defined(I2C_LEADER_RIGHT) || defined(LEADER_RIGHT)
      isLeftHand = !has_usb();
    #else
      isLeftHand = has_usb();
    #endif
  #endif
}

static void keyboard_leader_setup(void) {
#ifdef USE_I2C
#ifdef SSD1306OLED
    matrix_leader_OLED_init ();
#endif
#endif
#ifdef USE_MATRIX_I2C
    i2c_leader_init();
#else
    serial_leader_init();
#endif
}

static void keyboard_follower_setup(void) {
#ifdef USE_MATRIX_I2C
    i2c_follower_init(FOLLOWER_I2C_ADDRESS);
#else
    serial_follower_init();
#endif
}

bool has_usb(void) {
   USBCON |= (1 << OTGPADE); //enables VBUS pad
   _delay_us(5);
   return (USBSTA & (1<<VBUS));  //checks state of VBUS
}

void split_keyboard_setup(void) {
   setup_handedness();

   if (has_usb()) {
      keyboard_leader_setup();
   } else {
      keyboard_follower_setup();
   }
   sei();
}

void keyboard_follower_loop(void) {
   matrix_init();

   while (1) {
      matrix_follower_scan();
   }
}

// this code runs before the usb and keyboard is initialized
void matrix_setup(void) {
    split_keyboard_setup();

    if (!has_usb()) {
        keyboard_follower_loop();
    }
}
