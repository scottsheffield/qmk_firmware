
# Build Options
#   change to "no" to disable the options, or define them in the Makefile in
#   the appropriate keymap folder that will get included automatically
#
# Virtual DIP switch configuration(+1000)
BOOTMAGIC_ENABLE = no
# Mouse keys(+4700)
MOUSEKEY_ENABLE = no
# Audio control and System control(+450)
EXTRAKEY_ENABLE = yes
# Console for debug(+400)
CONSOLE_ENABLE = yes
# Commands for debug and configuration
COMMAND_ENABLE = no
# Nkey Rollover - if this doesn't work, see here: https://github.com/tmk/tmk_keyboard/wiki/FAQ#nkro-doesnt-work
NKRO_ENABLE = no
# Enable keyboard backlight functionality
BACKLIGHT_ENABLE = no
# MIDI controls
MIDI_ENABLE = no
# Audio output on port C6
AUDIO_ENABLE = no
# Unicode
UNICODE_ENABLE = no
# Enable Bluetooth with the Adafruit EZ-Key HID
BLUETOOTH_ENABLE = no
# Enable WS2812 RGB underlight. 
RGBLIGHT_ENABLE = yes
# Enable one-hand typing
SWAP_HANDS_ENABLE = no
# # New oled driver
# OLED_DRIVER_ENABLE = yes
# # New i2c comms
# SPLIT_KEYBOARD = yes

# Do not enable SLEEP_LED_ENABLE. it uses the same timer as BACKLIGHT_ENABLE
SLEEP_LED_ENABLE = no    # Breathing sleep LED during USB suspend

EXTRAFLAGS += -flto
# If you want to change the display of OLED, you need to change here
SRC +=  ./franzfont.c \
        ./lib/host_led_state_reader.c \
