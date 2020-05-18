#ifndef I2C_H
#define I2C_H

#include <stdint.h>

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#define I2C_READ 1
#define I2C_WRITE 0

#define I2C_ACK 1
#define I2C_NACK 0

#define FOLLOWER_BUFFER_SIZE 0x10

// i2c SCL clock frequency
#define SCL_CLOCK  400000L

extern volatile uint8_t i2c_follower_buffer[FOLLOWER_BUFFER_SIZE];

void i2c_leader_init(void);
uint8_t i2c_leader_start(uint8_t address);
void i2c_leader_stop(void);
uint8_t i2c_leader_write(uint8_t data);
uint8_t i2c_leader_read(int);
void i2c_reset_state(void);
void i2c_follower_init(uint8_t address);

#endif
