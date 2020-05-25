
#include "transport.h"

#include "config.h"
#include "matrix.h"
#include "quantum.h"

#include "orbit.h"

#define ROWS_PER_HAND (MATRIX_ROWS/2)

#ifdef RGBLIGHT_ENABLE
#   include "rgblight.h"
#endif

#ifdef BACKLIGHT_ENABLE
# include "backlight.h"
  extern backlight_config_t backlight_config;
#endif

#if defined(USE_I2C)

#include "i2c.h"

#ifndef FOLLOWER_I2C_ADDRESS
#  define FOLLOWER_I2C_ADDRESS           0x32
#endif

#if (MATRIX_COLS > 8)
#  error "Currently only supports 8 COLS"
#endif

// Get rows from other half over i2c
bool transport_leader(matrix_row_t matrix[]) {
  int err = 0;

  // write backlight info
#ifdef BACKLIGHT_ENABLE
  if (BACKLIT_DIRTY) {
    err = i2c_leader_start(FOLLOWER_I2C_ADDRESS + I2C_WRITE);
    if (err) { goto i2c_error; }

    // Backlight location
    err = i2c_leader_write(I2C_BACKLIT_START);
    if (err) { goto i2c_error; }

    // Write backlight
    i2c_leader_write(get_backlight_level());

    BACKLIT_DIRTY = false;
  }
#endif

  err = i2c_leader_start(FOLLOWER_I2C_ADDRESS + I2C_WRITE);
  if (err) { goto i2c_error; }

  // start of matrix stored at I2C_KEYMAP_START
  err = i2c_leader_write(I2C_KEYMAP_START);
  if (err) { goto i2c_error; }

  // Start read
  err = i2c_leader_start(FOLLOWER_I2C_ADDRESS + I2C_READ);
  if (err) { goto i2c_error; }

  if (!err) {
    int i;
    for (i = 0; i < ROWS_PER_HAND-1; ++i) {
      matrix[i] = i2c_leader_read(I2C_ACK);
    }
    matrix[i] = i2c_leader_read(I2C_NACK);
    i2c_leader_stop();
  } else {
i2c_error: // the cable is disconnceted, or something else went wrong
    i2c_reset_state();
    return false;
  }

#ifdef RGBLIGHT_ENABLE
  if (RGB_DIRTY) {
    err = i2c_leader_start(FOLLOWER_I2C_ADDRESS + I2C_WRITE);
    if (err) { goto i2c_error; }

    // RGB Location
    err = i2c_leader_write(I2C_RGB_START);
    if (err) { goto i2c_error; }

    uint32_t dword = eeconfig_read_rgblight();

    // Write RGB
    err = i2c_leader_write_data(&dword, 4);
    if (err) { goto i2c_error; }

    RGB_DIRTY = false;
    i2c_leader_stop();
  }
#endif

  return true;
}

void transport_follower(matrix_row_t matrix[]) {

  for (int i = 0; i < ROWS_PER_HAND; ++i)
  {
    i2c_follower_buffer[I2C_KEYMAP_START + i] = matrix[i];
  }
  // Read Backlight Info
  #ifdef BACKLIGHT_ENABLE
  if (BACKLIT_DIRTY)
  {
    backlight_set(i2c_follower_buffer[I2C_BACKLIT_START]);
    BACKLIT_DIRTY = false;
  }
  #endif
  #ifdef RGBLIGHT_ENABLE
  if (RGB_DIRTY)
  {
    // Disable interupts (RGB data is big)
    cli();
    // Create new DWORD for RGB data
    uint32_t dword;

    // Fill the new DWORD with the data that was sent over
    uint8_t * dword_dat = (uint8_t *)(&dword);
    for (int i = 0; i < 4; i++)
    {
      dword_dat[i] = i2c_follower_buffer[I2C_RGB_START + i];
    }

    // Update the RGB now with the new data and set RGB_DIRTY to false
    rgblight_update_dword(dword);
    RGB_DIRTY = false;
    // Re-enable interupts now that RGB is set
    sei();
  }
  #endif
}

void transport_leader_init(void) {
  i2c_leader_init();
}

void transport_follower_init(void) {
  i2c_follower_init(FOLLOWER_I2C_ADDRESS);
}

#else // USE_SERIAL

#include "serial.h"



volatile Serial_s2m_buffer_t serial_s2m_buffer = {};
volatile Serial_m2s_buffer_t serial_m2s_buffer = {};
uint8_t volatile status0 = 0;

SSTD_t transactions[] = {
  { (uint8_t *)&status0,
    sizeof(serial_m2s_buffer), (uint8_t *)&serial_m2s_buffer,
    sizeof(serial_s2m_buffer), (uint8_t *)&serial_s2m_buffer
  }
};

uint8_t follower_layer_cache;
uint8_t follower_nlock_cache;
uint8_t follower_clock_cache;
uint8_t follower_slock_cache;

void transport_leader_init(void)
{ soft_serial_initiator_init(transactions, TID_LIMIT(transactions)); }

void transport_follower_init(void)
{ 
	soft_serial_target_init(transactions, TID_LIMIT(transactions)); 
	follower_layer_cache = 255;
	follower_nlock_cache = 255;
	follower_clock_cache = 255;
	follower_slock_cache = 255;
}

bool transport_leader(matrix_row_t matrix[]) {

  if (soft_serial_transaction()) {
    return false;
  }

  // TODO:  if MATRIX_COLS > 8 change to unpack()
  for (int i = 0; i < ROWS_PER_HAND; ++i) {
    matrix[i] = serial_s2m_buffer.smatrix[i];
  }

  #if defined(RGBLIGHT_ENABLE) && defined(RGBLIGHT_SPLIT)
    // Code to send RGB over serial goes here (not implemented yet)
  #endif

  #ifdef BACKLIGHT_ENABLE
    // Write backlight level for follower to read
    serial_m2s_buffer.backlight_level = backlight_config.enable ? backlight_config.level : 0;
  #endif

  return true;
}

void transport_follower(matrix_row_t matrix[]) {

  // TODO: if MATRIX_COLS > 8 change to pack()
  for (int i = 0; i < ROWS_PER_HAND; ++i)
  {
    serial_s2m_buffer.smatrix[i] = matrix[i];
  }
  #ifdef BACKLIGHT_ENABLE
    backlight_set(serial_m2s_buffer.backlight_level);
  #endif
  #if defined(RGBLIGHT_ENABLE) && defined(RGBLIGHT_SPLIT)
  // Add serial implementation for RGB here
  #endif
  
  if (follower_layer_cache != serial_m2s_buffer.current_layer) {  
	follower_layer_cache = serial_m2s_buffer.current_layer;
	set_layer_indicators(follower_layer_cache);
  }
  
  if (follower_nlock_cache != serial_m2s_buffer.nlock_led) {
	follower_nlock_cache = serial_m2s_buffer.nlock_led;
	led_toggle(3, follower_nlock_cache);
  }
  if (follower_clock_cache != serial_m2s_buffer.clock_led) {
	follower_clock_cache = serial_m2s_buffer.clock_led;
	led_toggle(4, follower_clock_cache);
  }
  if (follower_slock_cache != serial_m2s_buffer.slock_led) {
	follower_slock_cache = serial_m2s_buffer.slock_led;
	led_toggle(5, follower_slock_cache);
  }
  
}

#endif
