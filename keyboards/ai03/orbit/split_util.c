#include "split_util.h"
#include "matrix.h"
#include "keyboard.h"
#include "config.h"
#include "timer.h"
#include "split_flags.h"
#include "transport.h"
#include "quantum.h"

#ifdef EE_HANDS
#   include "tmk_core/common/eeprom.h"
#   include "eeconfig.h"
#endif

volatile bool isLeftHand = true;

__attribute__((weak))
bool is_keyboard_left(void) {
  #ifdef SPLIT_HAND_PIN
    // Test pin SPLIT_HAND_PIN for High/Low, if low it's right hand
    setPinInput(SPLIT_HAND_PIN);
    return readPin(SPLIT_HAND_PIN);
  #else
    #ifdef EE_HANDS
      return eeprom_read_byte(EECONFIG_HANDEDNESS);
    #else
      #ifdef LEADER_RIGHT
        return !is_keyboard_leader();
      #else
        return is_keyboard_leader();
      #endif
    #endif
  #endif
}

bool is_keyboard_leader(void)
{
#ifdef __AVR__
  static enum { UNKNOWN, LEADER, FOLLOWER } usbstate = UNKNOWN;

  // only check once, as this is called often
  if (usbstate == UNKNOWN)
  {
    USBCON |= (1 << OTGPADE);  // enables VBUS pad
    wait_us(5);

    usbstate = (USBSTA & (1 << VBUS)) ? LEADER : FOLLOWER;  // checks state of VBUS
  }

  return (usbstate == LEADER);
#else
  return true;
#endif
}

static void keyboard_leader_setup(void) {
#if defined(USE_I2C)
  #ifdef SSD1306OLED
    matrix_leader_OLED_init ();
  #endif
#endif
  transport_leader_init();

  // For leader the Backlight info needs to be sent on startup
  // Otherwise the salve won't start with the proper info until an update
  BACKLIT_DIRTY = true;
}

static void keyboard_follower_setup(void)
{
  transport_follower_init();
}

// this code runs before the usb and keyboard is initialized
void matrix_setup(void)
{
  isLeftHand = is_keyboard_left();

  if (is_keyboard_leader())
  {
    keyboard_leader_setup();
  }
  else
  {
    keyboard_follower_setup();
  }
}
