#pragma once

/* Soft Serial defines */
#define SERIAL_PIN_DDR DDRD
#define SERIAL_PIN_PORT PORTD
#define SERIAL_PIN_INPUT PIND
#define SERIAL_PIN_MASK _BV(PD2)
#define SERIAL_PIN_INTERRUPT INT2_vect

#define SERIAL_FOLLOWER_BUFFER_LENGTH MATRIX_ROWS/2
#define SERIAL_LEADER_BUFFER_LENGTH MATRIX_ROWS/2
