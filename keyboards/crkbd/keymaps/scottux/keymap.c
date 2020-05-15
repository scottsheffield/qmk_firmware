#include QMK_KEYBOARD_H
#include "bootloader.h"
#ifdef PROTOCOL_LUFA
#include "lufa.h"
#include "split_util.h"
#endif
#ifdef SSD1306OLED
#include "ssd1306.h"
#endif
#ifdef CONSOLE_ENABLED
#include <print.h>
#endif

extern keymap_config_t keymap_config;

#ifdef RGBLIGHT_ENABLE
//Following line allows macro to read current RGB settings
extern rgblight_config_t rgblight_config;
#endif

extern uint8_t is_leader;

typedef union {
    uint32_t raw;
    struct {
      uint8_t gradient_stops : 8;
      uint8_t mode : 8;
      uint8_t hue : 8;
      uint8_t underglow : 8;
    };
} rgblight_theme_t;

// Each layer gets a name for readability, which is then used in the keymap matrix below.
// The underscores don't mean anything - you can have a layer called STUFF or any other name.
// Layer names don't all need to be of the same length, obviously, and you can also skip them
// entirely and just use numbers.
#define _QWERTY 0
#define _LOWER 1
#define _RAISE 2
#define _ADJUST 3
#define _GAMER 4
#define _GAMER_MOD 5

enum custom_keycodes {
  QWERTY = SAFE_RANGE,
  LOWER,
  RAISE,
  ADJUST,
  GAMER,
  GAMER_MOD,
  BACKLIT,
  RGBRST,
  TOGTHM,
  UPUNDR,
  DNUNDR,
};

enum macro_keycodes {
  KC_SAMPLEMACRO,
};

#define KC______ KC_TRNS
#define KC_XXXXX KC_NO
#define KC_LOWER LOWER
#define KC_RAISE RAISE
#define KC_GAMER GAMER
#define KC_GAMER_MOD GAMER_MOD
#define KC_TOGTHM TOGTHM
#define KC_UPUNDR UPUNDR
#define KC_DNUNDR DNUNDR
#define KC_RST   RESET
#define KC_LRST  RGBRST
#define KC_LTOG  RGB_TOG
#define KC_LHUI  RGB_HUI
#define KC_LHUD  RGB_HUD
#define KC_LSAI  RGB_SAI
#define KC_LSAD  RGB_SAD
#define KC_LVAI  RGB_VAI
#define KC_LVAD  RGB_VAD
#define KC_LMOD  RGB_MOD
#define KC_CTLTB CTL_T(KC_TAB)
#define KC_GUIESC GUI_T(KC_ESC)
// #define KC_ALTKN ALT_T(KC_LANG1)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_QWERTY] = LAYOUT_kc( \
  //,-----------------------------------------.                ,-----------------------------------------.
        TAB,     Q,     W,     E,     R,     T,                      Y,     U,     I,     O,     P,  BSPC,\
  //|------+------+------+------+------+------|                |------+------+------+------+------+------|
       LCTL,     A,     S,     D,     F,     G,                      H,     J,     K,     L,  SCLN,  QUOT,\
  //|------+------+------+------+------+------|                |------+------+------+------+------+------|
       LSFT,     Z,     X,     C,     V,     B,                      N,     M,  COMM,   DOT,  SLSH,   ENT,\
  //|------+------+------+------+------+------+------|  |------+------+------+------+------+------+------|
                                 GUIESC, LOWER,   SPC,      SPC, RAISE,  LALT \
                              //`--------------------'  `--------------------'
  ),

  [_LOWER] = LAYOUT_kc( \
  //,-----------------------------------------.                ,-----------------------------------------.
        TAB,  EXLM,    AT,  HASH,   DLR,  PERC,                   CIRC,  AMPR,  ASTR,  LPRN,  RPRN,  BSPC,\
  //|------+------+------+------+------+------|                |------+------+------+------+------+------|
       TILD,  UNDS,  MINS,    LT,  LCBR,  LBRC,                   RBRC,  RCBR,    GT,  PLUS,   EQL,  PIPE,\
  //|------+------+------+------+------+------|                |------+------+------+------+------+------|
       LSFT,   ESC,  MUTE,  VOLD,  VOLU, XXXXX,                  XXXXX,  MPLY,  MPRV,  MNXT,  BSLS, XXXXX,\
  //|------+------+------+------+------+------+------|  |------+------+------+------+------+------+------|
                                 GUIESC, LOWER,   SPC,      SPC, RAISE,  LALT \
                              //`--------------------'  `--------------------'
  ),

  [_RAISE] = LAYOUT_kc( \
  //,-----------------------------------------.                ,-----------------------------------------.
        TAB,     1,     2,     3,     4,     5,                      6,     7,     8,     9,     0,  BSPC,\
  //|------+------+------+------+------+------|                |------+------+------+------+------+------|
        GRV, XXXXX,  HOME,  PGDN,  PGUP,   END,                  XXXXX,  LEFT,  DOWN,    UP, RIGHT, XXXXX,\
  //|------+------+------+------+------+------|                |------+------+------+------+------+------|
       LSFT,   F11,   F12,   F13,   F14,   F15,                    F16,   F17,   F18,   F19,   F20, XXXXX,\
  //|------+------+------+------+------+------+------|  |------+------+------+------+------+------+------|
                                 GUIESC, LOWER,   SPC,      SPC, RAISE,  LALT \
                              //`--------------------'  `--------------------'
  ),

  [_ADJUST] = LAYOUT_kc( \
  //,-----------------------------------------.                ,-----------------------------------------.
     TOGTHM,   RST,  LRST, XXXXX, XXXXX, XXXXX,                     F1,    F2,    F3,    F4,    F5,    F6,\
  //|------+------+------+------+------+------|                |------+------+------+------+------+------|
       CAPS,  LTOG,  LHUI,  LSAI,  LVAI,UPUNDR,                     F7,    F8,    F9,   F10,   F11,   F12,\
  //|------+------+------+------+------+------|                |------+------+------+------+------+------|
      XXXXX,  LMOD,  LHUD,  LSAD,  LVAD,DNUNDR,                  XXXXX, XXXXX, XXXXX, XXXXX, XXXXX, XXXXX,\
  //|------+------+------+------+------+------+------|  |------+------+------+------+------+------+------|
                                 GUIESC, LOWER,   SPC,      SPC, RAISE,  LALT \
                              //`--------------------'  `--------------------'
  ),
  [_GAMER] = LAYOUT_kc( \
  //,-----------------------------------------.                ,-----------------------------------------.
        TAB,   TAB,     Q,     W,     E,     R,                      Y,     U,     I,     O,     P,  BSPC,\
  //|------+------+------+------+------+------|                |------+------+------+------+------+------|
       LCTL,  LSFT,     A,     S,     D,     F,                      H,     J,     K,     L,  SCLN,  QUOT,\
  //|------+------+------+------+------+------|                |------+------+------+------+------+------|
       LSFT,     Z,     X,     C,     V,     B,                      N,     M,  COMM,   DOT,  SLSH,   ENT,\
  //|------+------+------+------+------+------+------|  |------+------+------+------+------+------+------|
                              GAMER_MOD, SPC,   SPC,      SPC, RAISE,  LALT \
                              //`--------------------'  `--------------------'
  ),

  [_GAMER_MOD] = LAYOUT_kc( \
  //,-----------------------------------------.                ,-----------------------------------------.
        TAB,     1,     2,     3,     4,     5,                      6,     7,     8,     9,     0,  BSPC,\
  //|------+------+------+------+------+------|                |------+------+------+------+------+------|
        GRV, XXXXX,  HOME,  PGDN,  PGUP,   END,                   LEFT,  DOWN,    UP, RIGHT, XXXXX, XXXXX,\
  //|------+------+------+------+------+------|                |------+------+------+------+------+------|
       LSFT,   F11,   F12,   F13,   F14,   F15,                    F16,   F17,   F18,   F19,   F20, XXXXX,\
  //|------+------+------+------+------+------+------|  |------+------+------+------+------+------+------|
                              GAMER_MOD, SPC,   SPC,      SPC, RAISE,  LALT \
                              //`--------------------'  `--------------------'
  ),
};

int RGB_current_mode;

void persistent_default_layer_set(uint16_t default_layer) {
  eeconfig_update_default_layer(default_layer);
  default_layer_set(default_layer);
}

// Setting ADJUST layer RGB back to default
void update_tri_layer_RGB(uint8_t layer1, uint8_t layer2, uint8_t layer3) {
  if (IS_LAYER_ON(layer1) && IS_LAYER_ON(layer2)) {
    layer_on(layer3);
  } else {
    layer_off(layer3);
  }
}

#ifdef RGBLIGHT_ENABLE
bool using_theme = false;
bool selected_theme = false;
uint8_t current_underglow = 0;
const uint8_t cyber_sat = 255;
const uint8_t cyber_val = 180;
rgblight_theme_t cybersunset = {
  .mode=5,
  .hue=35,
  .underglow=10,
  .gradient_stops=3,
};
rgblight_theme_t cyberdawn = {
  .mode=4,
  .hue=110,
  .underglow=210,
  .gradient_stops=4,
};

void set_underglow(uint8_t underglow_hue, uint8_t sat, uint8_t val) {
  current_underglow = underglow_hue;
  int i;
  for (i = 0; i < 6; i++) {
    rgblight_sethsv_at(underglow_hue, sat, val + ((6 - i) * 5), i);
  }
}

void set_theme(uint8_t theme_mode, uint8_t theme_hue, uint8_t theme_sat, uint8_t theme_val, uint8_t theme_underglow_hue) {
  rgblight_mode_noeeprom(theme_mode);
  rgblight_sethsv_noeeprom(theme_hue, theme_sat, theme_val);
  set_underglow(theme_underglow_hue, theme_sat, theme_val);
}

void increase_underglow_hue(void) {
  set_underglow(
    (current_underglow + RGBLIGHT_HUE_STEP) % 255,
    rgblight_config.sat,
    rgblight_config.val
  );
}

void decrease_underglow_hue(void) {
  set_underglow(
    (current_underglow - RGBLIGHT_HUE_STEP) % 255,
    rgblight_config.sat,
    rgblight_config.val
  );
}

void set_theme_cybersunset(void) {
  set_theme(
    cybersunset.mode,
    cybersunset.hue,
    cyber_sat,
    rgblight_config.val,
    cybersunset.underglow
  );
}
void set_theme_cyberdawn(void) {
  set_theme(
    cyberdawn.mode,
    cyberdawn.hue,
    cyber_sat,
    rgblight_config.val,
    cyberdawn.underglow
  );
}
void toggle_theme(void) {
  if (selected_theme) {
    set_theme_cyberdawn();
  } else {
    set_theme_cybersunset();
  }
  selected_theme = !selected_theme;
}
void toggle_theme_and_eeprom(void) {
  toggle_theme();
  eeconfig_update_rgblight(rgblight_config.raw);
}

void manual_led_persist(void) {

}
#endif

void matrix_init_user(void) {
    #ifdef RGBLIGHT_ENABLE
      RGB_current_mode = rgblight_config.mode;
      if (rgblight_config.hue == cybersunset.hue) {
        using_theme = true;
        selected_theme = true;
        set_theme_cybersunset();
      } else if (rgblight_config.hue == cyberdawn.hue) {
        using_theme = true;
        selected_theme = false;
        set_theme_cyberdawn();
      }
    #endif
    //SSD1306 OLED init, make sure to add #define SSD1306OLED in config.h
    #ifdef SSD1306OLED
        iota_gfx_init(!has_usb());   // turns on the display
    #endif
}



//SSD1306 OLED update loop, make sure to add #define SSD1306OLED in config.h
#ifdef SSD1306OLED

// When add source files to SRC in rules.mk, you can use functions.
const char *read_layer_state(void);
const char *read_logo(void);
void set_keylog(uint16_t keycode, keyrecord_t *record);
const char *read_keylog(void);
const char *read_keylogs(void);

const char *read_mode_icon(bool swap);
const char *read_host_led_state(void);
void set_timelog(void);
const char *read_timelog(void);
char buffer[4];

void matrix_scan_user(void) {
   iota_gfx_task();
}

void debug_backlight_info(struct CharacterMatrix *matrix) {
  itoa(rgblight_get_hue(), buffer, 10);
  matrix_write(matrix, "h: ");
  matrix_write(matrix, buffer);
  itoa(rgblight_get_mode(), buffer, 10);
  matrix_write(matrix, " m: ");
  matrix_write(matrix, buffer);
  itoa(rgblight_get_sat(), buffer, 10);
  matrix_write(matrix, " s: ");
  matrix_write_ln(matrix, buffer);
  itoa(rgblight_get_val(), buffer, 10);
  matrix_write(matrix, "v: ");
  matrix_write(matrix, buffer);
  itoa(current_underglow, buffer, 10);
  matrix_write(matrix, " u: ");
  matrix_write_ln(matrix, buffer);
}

void matrix_render_user(struct CharacterMatrix *matrix) {
  if (is_leader) {
    // If you want to change the display of OLED, you need to change here
    matrix_write_ln(matrix, read_layer_state());
    // matrix_write_ln(matrix, read_keylog());
    // matrix_write_ln(matrix, read_keylogs());
    //matrix_write_ln(matrix, read_mode_icon(keymap_config.swap_lalt_lgui));
    debug_backlight_info(matrix);
    matrix_write_ln(matrix, read_timelog());
  } else {
    // matrix_write(matrix, read_logo());
    matrix_write_ln(matrix, read_timelog());
  }
}

void matrix_update(struct CharacterMatrix *dest, const struct CharacterMatrix *source) {
  if (memcmp(dest->display, source->display, sizeof(dest->display))) {
    memcpy(dest->display, source->display, sizeof(dest->display));
    dest->dirty = true;
  }
}

void iota_gfx_task_user(void) {
  struct CharacterMatrix matrix;
  matrix_clear(&matrix);
  matrix_render_user(&matrix);
  matrix_update(&display, &matrix);
}
#endif//SSD1306OLED

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  if (record->event.pressed) {
#ifdef SSD1306OLED
    set_keylog(keycode, record);
#endif
    set_timelog();
  }
#ifdef CONSOLE_ENABLE
  uprintf("KL: kc: %u, col: %u, row: %u, pressed: %u\n", keycode, record->event.key.col, record->event.key.row, record->event.pressed);
#endif 

  switch (keycode) {
    case QWERTY:
      if (record->event.pressed) {
        persistent_default_layer_set(1UL<<_QWERTY);
      }
      return false;
    case GAMER:
      if (record->event.pressed) {
        persistent_default_layer_set(1UL<<_GAMER);
      }
      return false;
    case GAMER_MOD:
      if (record->event.pressed) {
        layer_on(_GAMER_MOD);
        update_tri_layer_RGB(_LOWER, _RAISE, _ADJUST);
      } else {
        layer_off(_GAMER_MOD);
        update_tri_layer_RGB(_LOWER, _RAISE, _ADJUST);
      }
      return false;
    case LOWER:
      if (record->event.pressed) {
        layer_on(_LOWER);
        update_tri_layer_RGB(_LOWER, _RAISE, _ADJUST);
      } else {
        layer_off(_LOWER);
        update_tri_layer_RGB(_LOWER, _RAISE, _ADJUST);
      }
      return false;
    case RAISE:
      if (record->event.pressed) {
        layer_on(_RAISE);
        update_tri_layer_RGB(_LOWER, _RAISE, _ADJUST);
      } else {
        layer_off(_RAISE);
        update_tri_layer_RGB(_LOWER, _RAISE, _ADJUST);
      }
      return false;
    case ADJUST:
      if (record->event.pressed) {
        layer_on(_ADJUST);
      } else {
        layer_off(_ADJUST);
      }
      return false;
    case RGB_MOD:
      #ifdef RGBLIGHT_ENABLE
      if (record->event.pressed) {
        rgblight_mode(RGB_current_mode);
        rgblight_step();
        RGB_current_mode = rgblight_config.mode;
      }
      #endif
      return false;
    case RGBRST:
      #ifdef RGBLIGHT_ENABLE
      if (record->event.pressed) {
        eeconfig_update_rgblight_default();
        rgblight_enable();
        RGB_current_mode = rgblight_config.mode;
      }
      #endif
    case TOGTHM:
      #ifdef RGBLIGHT_ENABLE
      if (record->event.pressed) {
        toggle_theme();
      }
      #endif
      return false;
    case UPUNDR:
      #ifdef RGBLIGHT_ENABLE
      if (record->event.pressed) {
        increase_underglow_hue();
      }
      #endif
      return false;
    case DNUNDR:
      #ifdef RGBLIGHT_ENABLE
      if (record->event.pressed) {
        decrease_underglow_hue();
      }
      #endif
      return false;
  }
  return true;
}
