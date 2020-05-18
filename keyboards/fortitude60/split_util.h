#ifndef SPLIT_KEYBOARD_UTIL_H
#define SPLIT_KEYBOARD_UTIL_H

#include <stdbool.h>
#include "eeconfig.h"

extern volatile bool isLeftHand;

// follower version of matix scan, defined in matrix.c
void matrix_follower_scan(void);

void split_keyboard_setup(void);
bool has_usb(void);
void keyboard_follower_loop(void);

void matrix_leader_OLED_init (void);

#endif
